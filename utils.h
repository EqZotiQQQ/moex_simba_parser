#pragma once

constexpr unsigned long little_endian_milliseconds = 3569595041;
constexpr unsigned long big_endian_milliseconds    = 2712847316;
constexpr unsigned long little_endian_nanoseconds  = 1295823521;
constexpr unsigned long big_endian_nanoseconds     = 2712812621;

enum Endian {
    big_endian,
    little_endian,
};

enum TimeStamp {
    milliseconds,
    nanoseconds,
};
