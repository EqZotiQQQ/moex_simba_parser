#pragma once

#include <cstdint>
#include <array>
#include <fstream>
#include "utils.h"

using i32 = int32_t;
using u32 = uint32_t;
using i16 = int16_t;
using u16 = uint16_t;

class Parsers {
public:
    static u16 parse_u16(std::ifstream& file, Endian endian) {
        u16 u16_val {};
        std::array<i32, 2> bytes {};
        for (int i = 0; i < 2; i++) {
            bytes[i] = file.get();
        }
        if (endian == Endian::big_endian) {
            std::reverse(bytes.begin(), bytes.end());
        }
        u16_val = static_cast<i32>(bytes[0]) << 8 | static_cast<i32>(bytes[1]);
        return u16_val;
    }

    static i32 parse_i32(std::ifstream& file, Endian endian) {
        i32 i32_val {};
        std::array<i32, 4> bytes {};
        for (int i = 0; i < 4; i++) {
            bytes[i] = file.get();
        }
        if (endian == Endian::big_endian) {
            std::reverse(bytes.begin(), bytes.end());
        }
        i32_val = static_cast<i32>(bytes[0]) << 24 | static_cast<i32>(bytes[1]) << 16 | static_cast<i32>(bytes[2] << 8 | static_cast<i32>(bytes[3]));
        return i32_val;
    }

    static u32 parse_u32(std::ifstream& file, Endian endian) {
        u32 u32_val {};
        std::array<u32, 4> bytes {};
        for (int i = 0; i < 4; i++) {
            bytes[i] = file.get();
        }
        if (endian == Endian::big_endian) {
            std::reverse(bytes.begin(), bytes.end());
        }
        u32_val = static_cast<u32>(bytes[0]) << 24 | static_cast<u32>(bytes[1]) << 16 | static_cast<u32>(bytes[2] << 8 | static_cast<u32>(bytes[3]));
        return u32_val;
    }
};