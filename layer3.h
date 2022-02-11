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

