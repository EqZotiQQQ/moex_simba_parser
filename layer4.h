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


