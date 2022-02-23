#pragma once

#include <array>
#include <cstdint>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>

#include "types/typenames.h"


class BufferedReader {
    friend std::ostream& operator<<(std::ostream& os, const BufferedReader& reader);
private:
    static constexpr u64 buffer_size {2048};
    u16 buffer_pos {};
    u64 parsed_bytes {};
    std::array<u8, buffer_size> buffer {0};
    bool is_init = false;

private:
    u64 parse_by_byte() {
        u64 left = parsed_bytes - buffer_pos;
        for (int i = 0; i < left; i++) {
            buffer[i] = buffer[buffer_size - left + i];
        }
        for (u64 i = left; i < buffer_size; i++) {
            buffer[i] = file.get();
        }
        parsed_bytes = buffer_size;
        return buffer_size;
    }

    u64 parse_using_read() {
        u64 left = parsed_bytes - buffer_pos;
        for (int i = 0; i < left; i++) {
            buffer[i] = buffer[buffer_size - left + i];
        }
        file.read((char*) &buffer[left], buffer_size - left);  // it .get() reads N-1 bytes from file and place to the end '/0'; get reads untill '/n' symbol, lol

        if (!file.good()) {
            std::cerr << "Bits: " << file.good() << file.bad() << file.fail() << file.eof() << std::endl;
        }
        u64 gcount = file.gcount();
        parsed_bytes = gcount + left;
        return parsed_bytes;
    }
public:
    std::ifstream file;
    Endian endian {};
public:
    BufferedReader(const std::string& in, Endian endian):
        file(in, std::ios::in | std::ios::out | std::ios::binary),
        endian(endian) {}

    BufferedReader() = default;

    u64 default_parse_method() {
//        return parse_by_byte();
        return parse_using_read();
    }

    template<typename T>
    T next(Endian provided_endian) {
        u8 t_size = sizeof(T);
         u64 diapason = parsed_bytes - buffer_pos;
        if (t_size > diapason) {
            default_parse_method();
            buffer_pos = 0;
        }
        T value {};
        if (provided_endian == Endian::big_endian) {
            std::reverse(buffer.begin() + buffer_pos, buffer.begin() + buffer_pos + t_size);
        }
        for (int i = 0; i < t_size; i++) {
            u8 byte = buffer[buffer_pos + i];
            T shift = static_cast<T>(byte) << 8 * i;
            value = value | shift;
        }
        buffer_pos += t_size;
        return value;
    }

    void skip(i64 n) {
        if (n == 0) return;
        if (buffer_size > buffer_pos + n) {
            buffer_pos += n;
        } else {
            u64 left_in_buffer = buffer_size - buffer_pos;
            u64 current_file_pos = file.tellg();
            u64 shift = current_file_pos + n - left_in_buffer;
            file.seekg(shift);
            file.read((char*) &buffer[0], buffer_size);
            buffer_pos = 0;
        }
    }

    template<typename T>
    T next() {
        return next<T>(endian);
    }

    std::array<u8, 6> next_mac() {
        std::array<u8, 6> mac_addr {};
        for (int i = 0; i < 6; i++) {
            mac_addr[i] = next<u8>();
        }
        return mac_addr;
    }

    std::array<u8, 4> next_ip() {
        std::array<u8, 4> ip_addr {};
        for (int i = 0; i < 4; i++) {
            ip_addr[i] = next<u8>();
        }
        return ip_addr;
    }

    u64 get_parsed_pos() const {
        return parsed_bytes;
    }
};

std::ostream& operator<<(std::ostream& os, const BufferedReader& reader) {
    os << '[';
    for (int i = 0; i < reader.buffer.size() - 1; i++) {
        os << static_cast<u32>(reader.buffer[i]) << ' ';
    }
    os << static_cast<u32>(reader.buffer[reader.buffer.size() - 1]) << "]\n";
    return os;
}