#pragma once

#include <cstdint>
#include <fstream>
#include <array>
#include <iomanip>
#include <unordered_map>

#include "udp_parser.h"
#include "pcap_packet_parser.h"
#include "types/typenames.h"
#include "utils/buffered_reader.h"

enum OutputFromat {
    console = 0,
    file = 1,
    disabled = 2,
};

const char* to_endian(u32 type) {
    switch (type) {
        case Constants::big_endian_milliseconds: return "Big endian";
        case Constants::little_endian_milliseconds: return "Little endian";
        case Constants::big_endian_nanoseconds: return "Big endian";
        case Constants::little_endian_nanoseconds: return "Little endian";
        default: throw InvalidMagicNumberException();
    }
}

class GlobalPcapHeader {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const GlobalPcapHeader& ip_header);
private:
    u32 magic_number {};
    u16 version_major {};
    u16 version_minor {};
    i32 time_zone {};
    u32 sig_figs {};
    u32 snap_len {};
    u32 network {}; // replace it with LinkType later ...
public:
    static constexpr u8 size = {24};

    int parse(BufferedReader& parser) {
        magic_number = parser.next<u32>();
        parser.set_endian(validate_endians());
        version_major = parser.next<u16>();
        version_minor = parser.next<u16>();
        time_zone = parser.next<i32>();
        sig_figs = parser.next<u32>();
        snap_len = parser.next<u32>();
        network = parser.next<u32>();
        return size;
    };

    std::endian validate_endians() {
        switch (magic_number) {
            case Constants::little_endian_milliseconds:
            case Constants::little_endian_nanoseconds: return std::endian::little;
            case Constants::big_endian_milliseconds:
            case Constants::big_endian_nanoseconds: return std::endian::big;
            default: throw InvalidMagicNumberException();
        }
    }
};

class GlobalPcapPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const GlobalPcapPacket& ip);
private:

    GlobalPcapHeader global_pcap_header;

    PcapPacket pcap_parser;

    u32 bound {};

    template <typename T>
    void out(OutputFromat out_format, T data, std::optional<std::ofstream>& out) {
        switch (out_format) {
            case OutputFromat::file: {
                out.value() << data;
                break;
            }
            case OutputFromat::console: {
                std::cout << data;
                break;
            }
            case OutputFromat::disabled: {
                break;
            }
        }
    }

public:
    GlobalPcapPacket(u32 bound): bound(bound) {}

    void parse(BufferedReader& parser, std::optional<std::ofstream>& out, OutputFromat out_format) {

        global_pcap_header.parse(parser);

        u32 parsed_packets {};

        this->out(out_format, global_pcap_header, out);

        while (parsed_packets++ < bound) {
            pcap_parser.parse(parser);
            this->out(out_format, pcap_parser, out);
        }
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const GlobalPcapPacket& ip) {
    os << ip.global_pcap_header << std::endl;
    os << ip.pcap_parser << std::endl;
    return os;
}

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const GlobalPcapHeader& ip_header) {
    os << "== Global Pcap Header ==\n";
    os << std::setw(2) << std::right << std::hex;
    os << "Magic number: " << ((ip_header.magic_number >> 24)&0xFF) << ' ' << ((ip_header.magic_number >> 16)&0xFF) << ' ' << ((ip_header.magic_number >> 8)&0xFF) << ' ' << ((ip_header.magic_number)&0xFF) << '\n';
    os << std::dec;
    os << "Endian: " << to_endian(ip_header.magic_number) << '\n';
    os << "Version Major: " << ip_header.version_major << '\n';
    os << "Version Minor: " << ip_header.version_minor << '\n';
    os << "Time zone: " << ip_header.time_zone << '\n';
    os << "Sig figs: " << ip_header.sig_figs << '\n';
    os << "Snap len: " << ip_header.snap_len << '\n';
    os << "Network: " << ip_header.network << '\n';
    os << "== Global Pcap Header end ==\n";
    return os;
}
