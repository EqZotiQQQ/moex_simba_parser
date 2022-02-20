#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <vector>
#include <iostream>

#include "types/typenames.h"


class Parsers {
    // TODO parse group of bytes to reduce fragmentation's
public:

    static std::vector<u8> parse(std::ifstream& file, u32 n) {
        std::vector<u8> bytes(n);
        file.get((char*) &bytes[0], n);
        return bytes;
    }

    static void change_endian(std::vector<u8>& bytes, Endian endian) {
        if (endian == Endian::big_endian) {
            std::reverse(bytes.begin(), bytes.end());
        }
    }

    template<typename T>
    static T collapse(std::vector<u8>& buf, Endian endian, u32 beg, u32 end) {
        T val {};
        if (endian == Endian::big_endian) {
            std::reverse(buf.begin() + beg, buf.end() + end);
        }
//        val |= buf[beg];
        u8 range = end - beg;
        for (int i = 0; i <= range; i++) {
            u8 byte = buf[beg + i];
            u32 temp = static_cast<T>(byte) << 8 * i;
            val = val | temp;
        }
        return val;
    }

    static u8 parse_u8(std::ifstream& file) {
        return static_cast<u8>(file.get());
    }

    static u16 parse_u16(std::ifstream& file, Endian endian) {
        u16 u16_val {};
        std::array<u32, 2> bytes {};
        for (int i = 0; i < 2; i++) {
            bytes[i] = file.get();
        }
        if (endian == Endian::big_endian) {
            std::reverse(bytes.begin(), bytes.end());
        }
        u16_val =   static_cast<u16>(bytes[0])
                  | static_cast<u16>(bytes[1]) << 8;
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
        i32_val = static_cast<i32>(bytes[0])
                | static_cast<i32>(bytes[1]) <<  8
                | static_cast<i32>(bytes[2]) << 16
                | static_cast<i32>(bytes[3]) << 24;
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
        u32 u1 = static_cast<u32>(bytes[0]);
        u32 u2 = static_cast<u32>(bytes[1]) <<  8;
        u32 u3 = static_cast<u32>(bytes[2]) << 16;
        u32 u4 = static_cast<u32>(bytes[3]) << 24;
        u32 u32_val_temp = u1
                | u2
                | u3
                | u4;
        u32_val = static_cast<u32>(bytes[0])
                  | static_cast<u32>(bytes[1]) <<  8
                  | static_cast<u32>(bytes[2]) << 16
                  | static_cast<u32>(bytes[3]) << 24;
        return u32_val;
    }

    static u64 parse_u64(std::ifstream& file, Endian endian) {
        u64 u64_val {};
        std::array<u64, 8> bytes {};
        for (int i = 0; i < 8; i++) {
            bytes[i] = file.get();
        }
        if (endian == Endian::big_endian) {
            std::reverse(bytes.begin(), bytes.end());
        }
        u64_val = static_cast<u64>(bytes[0])
                | static_cast<u64>(bytes[1]) <<  8
                | static_cast<u64>(bytes[2]) << 16
                | static_cast<u64>(bytes[3]) << 24
                | static_cast<u64>(bytes[4]) << 32
                | static_cast<u64>(bytes[5]) << 40
                | static_cast<u64>(bytes[6]) << 48
                | static_cast<u64>(bytes[7]) << 56;
        return u64_val;
    }

    static i64 parse_i64(std::ifstream& file, Endian endian) {
        i64 i64_val {};
        std::array<i64, 8> bytes {};
        for (int i = 0; i < 8; i++) {
            bytes[i] = file.get();
        }
        if (endian == Endian::big_endian) {
            std::reverse(bytes.begin(), bytes.end());
        }
        i64_val =  static_cast<i64>(bytes[0])
                 | static_cast<i64>(bytes[1]) <<  8
                 | static_cast<i64>(bytes[2]) << 16
                 | static_cast<i64>(bytes[3]) << 24
                 | static_cast<i64>(bytes[4]) << 32
                 | static_cast<i64>(bytes[5]) << 40
                 | static_cast<i64>(bytes[6]) << 48
                 | static_cast<i64>(bytes[7]) << 56;
        return i64_val;
    }

    static void print_next(std::ifstream& file, Endian endian, int n) {
        for (int i = 0; i < n; i++) {
            std::cout << file.get() << ' ';
        }
        std::cout << '\n';
    }

    static void skip(std::ifstream& file, u64 n) {
        for (int i = 0; i < n; i++) {
            file.get();
        }
    }
};