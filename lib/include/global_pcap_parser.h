#pragma once

#include <cstdint>
#include <fstream>
#include <array>
#include <iomanip>
#include <unordered_map>

#include "types/enumerations.h"
#include "types/typenames.h"
#include "types/constants.h"
#include "utils/parsers.h"
#include "udp_parser.h"
#include "pcap_packet_parser.h"


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

    Endian endian {};

public:
    static constexpr u8 size = {24};

    int parse(BufferedReader& parser) {
        magic_number = parser.next<u32>();
        parser.endian = validate_endians();
        version_major = parser.next<u16>();
        version_minor = parser.next<u16>();
        time_zone = parser.next<i32>();
        sig_figs = parser.next<u32>();
        snap_len = parser.next<u32>();
        network = parser.next<u32>();
        return size;
    };

    Endian validate_endians() {
        switch (magic_number) {
            case Constants::little_endian_milliseconds:
            case Constants::little_endian_nanoseconds: return Endian::little_endian;
            case Constants::big_endian_milliseconds:
            case Constants::big_endian_nanoseconds: return Endian::big_endian;
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

    BufferedReader parser;

    u32 bound {};

    Endian endian {};

public:
    GlobalPcapPacket(u32 bound): parser{}, bound(bound) {}

    void parse(std::ifstream& file, std::optional<std::ofstream>& out, OutputFromat out_format) {

        parser.file = std::move(file);
        parser.endian = Endian::big_endian;

        global_pcap_header.parse(parser);

        u32 parsed_packets {};

        switch (out_format) {
            case OutputFromat::file: {
                out.value() << global_pcap_header;
                break;
            }
            case OutputFromat::console: {
                std::cout << global_pcap_header;
                break;
            }
            default: break;
        }

        while (parsed_packets++ < bound) {
            pcap_parser.parse(parser);
            switch (out_format) {
                case OutputFromat::file: {
                    out.value() << "Packet number " << parsed_packets << '\n';
                    out.value() << pcap_parser << std::endl;
                    break;
                }
                case OutputFromat::console: {
                    std::cout << "Packet number " << parsed_packets << '\n';
                    std::cout << pcap_parser << std::endl;
                    break;
                }
                default: break;
            }
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
    os << "Endian: " << endian_type[ip_header.magic_number] << '\n';
    os << "Version Major: " << ip_header.version_major << '\n';
    os << "Version Minor: " << ip_header.version_minor << '\n';
    os << "Time zone: " << ip_header.time_zone << '\n';
    os << "Sig figs: " << ip_header.sig_figs << '\n';
    os << "Snap len: " << ip_header.snap_len << '\n';
    os << "Network: " << ip_header.network << '\n';
    os << "== Global Pcap Header end ==\n";
    return os;
}
