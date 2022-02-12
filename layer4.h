#pragma once

#include <array>
#include <vector>
#include "types.h"

struct PcapBody {
    u32 check_sum {};
    std::array<u16, 4> source_ip {};
    std::array<u16, 4> dest_ip {};
    u32 source_port {};
    u32 destination_port {};

    static std::array<u16, 4> parse_IP(std::ifstream& file) {
        std::array<u16, 4> a {};
        for (int i = 0; i < 4; i++) {
            a[i] = file.get();
        }
        std::cout << std::endl;
        return a;
    }

    static PcapBody parse(std::ifstream& file, Endian endian) {
        return PcapBody {
                .check_sum = Parsers::parse_u16(file, Endian::little_endian),
                .source_ip = parse_IP(file),
                .dest_ip = parse_IP(file),
                .source_port = Parsers::parse_u16(file, Endian::little_endian),
                .destination_port = Parsers::parse_u16(file, Endian::little_endian),
        };
    }
};

std::ostream& operator<<(std::ostream& os, const PcapBody& frame) {
    os << "=========================== PcapBody: ==============================\n";
    os << "Source IP: "        << std::dec << static_cast<u16>(frame.source_ip[0]) << ":" << static_cast<u16>(frame.source_ip[1]) << ":" << static_cast<u16>(frame.source_ip[2]) << ":" << static_cast<u16>(frame.source_ip[3]) << '\n';
    os << "Destination IP: "   << std::dec << static_cast<u16>(frame.dest_ip[0]) << ":" << static_cast<u16>(frame.dest_ip[1]) << ":" << static_cast<u16>(frame.dest_ip[2]) << ":" << static_cast<u16>(frame.dest_ip[3]) << '\n';
    os << "Source port: "      << std::dec << static_cast<u16>(frame.source_port) << '\n';
    os << "Destination port: " << std::dec << static_cast<u16>(frame.destination_port) << '\n';
    os << "Check sum: "        << std::dec << static_cast<u16>(frame.check_sum) << '\n';
    os << "=========================== PcapBody end ============================\n";
    return os;
}
