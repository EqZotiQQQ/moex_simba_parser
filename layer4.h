#pragma once

#include <array>
#include "types.h"

struct Layer4 {
    u16 source_port {};
    u16 destination_port {};
    u16 datagram_length {};
    u16 check_sum {};

    void fill(std::ifstream& file, Endian endian) {
        source_port = Parsers::parse_u16(file, endian);
        destination_port = Parsers::parse_u16(file, endian);
        datagram_length = Parsers::parse_u16(file, endian);
        check_sum = Parsers::parse_u16(file, endian);
    }
};

std::ostream& operator<<(std::ostream& os, const Layer4& frame) {
    os << "Source port: "      << std::dec << static_cast<u16>(frame.source_port) << '\n';
    os << "Destination port: " << std::dec << static_cast<u16>(frame.destination_port) << '\n';
    os << "Datagram length: "  << std::dec << static_cast<u16>(frame.datagram_length) << '\n';
    os << "Check sum: "        << std::dec << static_cast<u16>(frame.check_sum) << '\n';
    return os;
}
