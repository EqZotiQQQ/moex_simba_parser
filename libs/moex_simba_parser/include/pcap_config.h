#pragma once

#include <bit>
#include <cstdint>
#include <stdexcept>
#include <unordered_map>

enum class Time {
    MICROSECONDS,
    NANOSECONDS,
};

struct PcapConfig {
    std::endian endian;
    Time time_format;

    PcapConfig(): endian(std::endian::big), time_format(Time::MICROSECONDS) {}

    explicit PcapConfig(uint32_t value) {
        init(value);
    }

    void init(uint32_t value) {
        switch(value) {
            case 0xA1B2C3D4: {
                time_format = Time::NANOSECONDS;
                endian = std::endian::big;
                break;
            }
            case 0xA1B23C4D: {
                time_format = Time::MICROSECONDS;
                endian = std::endian::big;
                break;
            }
            case 0xD4C3B2A1: {
                time_format = Time::NANOSECONDS;
                endian = std::endian::little;
                break;
            }
            case 0x4D3CB2A1: {
                time_format = Time::MICROSECONDS;
                endian = std::endian::little;
                break;
            }
            default: {
                throw std::runtime_error("Bad magic number");
            }
        }
    }

    std::string to_string_time() const {
        switch(time_format) {
            case Time::MICROSECONDS: {
                return "Milliseconds";
            }
            case Time::NANOSECONDS: {
                return "Nanoseconds";
            }
        }
    }

    std::string to_string_endian() {
        switch(endian) {
            case std::endian::big: {
                return "Big endian";
            }
            case std::endian::little: {
                return "Little endian";
            }
        }
    }

    std::string to_string() {
        return to_string_endian() + '\t' + to_string_time();
    }
};
