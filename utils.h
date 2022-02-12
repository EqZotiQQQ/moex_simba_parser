#pragma once

enum Endian {
    little_endian,
    big_endian,
};

std::ostream& operator<<(std::ostream& os, Endian endian) {
    os << "============================== Endian: =======================================\n";
    if (endian == Endian::little_endian) {
        os << "Little endian\n";
    } else {
        os << "Big endian\n";
    }
    os << "============================== Endian end =======================================\n";
    return os;
}

enum TimeStamp {
    milliseconds,
    nanoseconds,
};
