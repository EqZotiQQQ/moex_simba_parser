#pragma once

#include <array>
#include "types.h"

struct Layer3 {
    u8 version {};
    u8 ip_header_len {};
    u8 diff_serv_field {};
    u16 total_len {};
    u16 identification {};
    u16 flags {};
    u8 ttl {};
    u16 header_check_sum {};
    std::array<u8, 4> source_ip {};
    std::array<u8, 4> dest_ip {};

    void fill(std::ifstream& file, Endian endian) {
        version = Parsers::parse_u8(file, endian);
        ip_header_len = Parsers::parse_u8(file, endian);
        diff_serv_field = Parsers::parse_u8(file, endian);
        total_len = Parsers::parse_u16(file, endian);
        identification = Parsers::parse_u16(file, endian);
        flags = Parsers::parse_u16(file, endian);
        ttl = Parsers::parse_u8(file, endian);
        header_check_sum = Parsers::parse_u16(file, endian);
        for (int i = 0; i < 4; i++) {
            source_ip[i] = Parsers::parse_u8(file, endian);
        }
        for (int i = 0; i < 4; i++) {
            dest_ip[i] = Parsers::parse_u8(file, endian);
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Layer3& frame) {
    os << "Version: "           << std::dec << static_cast<u16>(frame.version) << '\n';
    os << "Ip header length: "  << std::dec << static_cast<u16>(frame.ip_header_len) << '\n';
    os << "diff_serv_field: "   << std::dec << static_cast<u16>(frame.diff_serv_field) << '\n';
    os << "total_len: "         << std::dec << static_cast<u16>(frame.total_len) << '\n';
    os << "identification: "    << std::dec << static_cast<u16>(frame.identification) << '\n';
    os << "flags: "             << std::dec << static_cast<u16>(frame.flags) << '\n';
    os << "ttl: "               << std::dec << static_cast<u16>(frame.ttl) << '\n';
    os << "header_check_sum: "  << std::dec << static_cast<u16>(frame.header_check_sum) << '\n';
    os  << "Source MAC: "       << std::dec << static_cast<u16>(frame.source_ip[0]) << ':'
                                << std::dec << static_cast<u16>(frame.source_ip[1]) << ':'
                                << std::dec << static_cast<u16>(frame.source_ip[2]) << ':'
                                << std::dec << static_cast<u16>(frame.source_ip[3]) << '\n';
    os  << "Destination MAC: "  << std::dec << static_cast<u16>(frame.dest_ip[0]) << ':'
                                << std::dec << static_cast<u16>(frame.dest_ip[1]) << ':'
                                << std::dec << static_cast<u16>(frame.dest_ip[2]) << ':'
                                << std::dec << static_cast<u16>(frame.dest_ip[3]) << '\n';
    return os;
}
