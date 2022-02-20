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

    Endian parse(std::ifstream& file) {
        std::array<int, 4> buffer {};
        for (int i = 0; i < 4; i++) {
            buffer[i] = file.get();
        }
        magic_number = buffer[0] << 24
                       | buffer[1] << 16
                       | buffer[2] << 8
                       | buffer[3];
        validate_endians();
        // Dream: parse<u16>();
        version_major = Parsers::parse_u16(file, endian);
        version_minor = Parsers::parse_u16(file, endian);
        time_zone = Parsers::parse_i32(file, endian);
        sig_figs = Parsers::parse_u32(file, endian);
        snap_len = Parsers::parse_u32(file, endian);
        network = Parsers::parse_u32(file, endian);
//
//        std::vector<u8> buffer = Parsers::parse(file, 24);
//        magic_number = buffer[0] << 24
//                     | buffer[1] << 16
//                     | buffer[2] << 8
//                     | buffer[3];
//        validate_endians();
//        version_major = Parsers::collapse<u16>(buffer, endian, 4, 5);
//        version_minor = Parsers::collapse<u16>(buffer, endian, 6, 7);
//        time_zone = Parsers::collapse<i32>(buffer, endian, 8, 11);
//        sig_figs = Parsers::collapse<u32>(buffer, endian, 12, 15);
//        snap_len = Parsers::collapse<u32>(buffer, endian, 16, 19);
//        network = Parsers::collapse<u32>(buffer, endian, 20, 23);
        return endian;
    };

    void validate_endians() {
        switch (magic_number) {
            case Constants::little_endian_milliseconds:
            case Constants::little_endian_nanoseconds: endian = Endian::little_endian; break;
            case Constants::big_endian_milliseconds:
            case Constants::big_endian_nanoseconds: endian = Endian::big_endian; break;
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
    Endian endian {};
    u32 i {};
public:
    GlobalPcapPacket(u32 bound): bound(bound) {}

    void parse(std::ifstream& file, std::optional<std::ofstream>& out) {
        endian = global_pcap_header.parse(file);

        if (out.has_value()) {
            out.value() << global_pcap_header;
        }
        while (i < bound && !file.eof()) {
            i++;
            pcap_parser.parse(file, endian);
            if (out.has_value()) {
                out.value() << "Packet number " << ++i << '\n';
                out.value() << pcap_parser << std::endl;
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
