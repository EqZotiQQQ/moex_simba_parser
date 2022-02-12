#pragma once

#include "exceptions.h"
#include "pcap_header.h"
#include "parsers.h"
#include "utils.h"
#include "frame.h"
#include "constants.h"

#include <fstream>
#include <array>
#include <vector>


class PcapParser {
private:
    std::ifstream file;
    PcapHeader header;
    Endian endian;
    TimeStamp time_stamp;
    std::size_t parsed {};
    std::vector<Frame> frames {};

public:
    explicit PcapParser(const std::string& path): file(path, std::ios::in | std::ios::out | std::ios::binary) {
        if (!file.is_open()) {
            throw FileNotFoundException();
        }

        parse_header();
        parse_frames();
    }

    void parse_header() {
        std::array<int, 4> buffer {};
        for (int i = 0; i < 4; i++) {
            buffer[i] = file.get();
        }
        header.magic_number = buffer[0] << 24 | buffer[1] << 16 | buffer[2] << 8 | buffer[3];
        validate_endians();

        // Dream: parse<u16>();
        header.version_major = Parsers::parse_u16(file, endian);
        header.version_minor = Parsers::parse_u16(file, endian);
        header.time_zone = Parsers::parse_i32(file, endian);
        header.sig_figs = Parsers::parse_u32(file, endian);
        header.snap_len = Parsers::parse_u32(file, endian);
        header.network = Parsers::parse_u32(file, endian);
    }

    void parse_frames() {
        while (!file.eof()) {
            Frame frame;
            //move out of this... like frame.header = fill();
            frame.header.timestamp_ms = Parsers::parse_i32(file, endian);
            frame.header.timestamp_ns = Parsers::parse_i32(file, endian);
            frame.header.pack_length = Parsers::parse_i32(file, endian);
            frame.header.real_length = Parsers::parse_i32(file, endian);

            frame.l2.fill(file, endian);
            frame.l3.fill(file, endian);
            frame.l4.fill(file, endian);


            std::cout << frame.l2;
            std::cout << frame.l3;
            std::cout << frame.l4;
            break;
        }
    }

    void validate_endians() {
        switch (header.magic_number) {
            case little_endian_milliseconds: {
                endian = Endian::little_endian;
                time_stamp = TimeStamp::milliseconds;
                break;
            }
            case big_endian_milliseconds: {
                endian = Endian::big_endian;
                time_stamp = TimeStamp::milliseconds;
                break;
            }
            case little_endian_nanoseconds: {
                endian = Endian::little_endian;
                time_stamp = TimeStamp::nanoseconds;
                break;
            }
            case big_endian_nanoseconds: {
                endian = Endian::big_endian;
                time_stamp = TimeStamp::nanoseconds;
                break;
            }
            default: {
                throw InvalidMagicNumberException();
            }
        }
    }

//    template<typename T, std::size_t N>
//    T parse_bits() {
//        T result;
//        std::array<T, N> bytes;
//        for (int i = 0; i < N; i++) {
//            bytes[i] = file.get();
//        }
//        if (endian == Endian::big_endian) {
//            std::reverse(bytes.begin(), bytes.end());
//        }
//        for (int i = 0; i < N; i++) {
//            result |= bytes[i] << N * 8 - i * 8;
//        }
//    }



};
