#include <stdexcept>
#include "magic_number.h"

MagicNumber::MagicNumber(): value(0) {}

MagicNumber::MagicNumber(uint32_t value): value(value) {}

std::string MagicNumber::to_string() const {
    std::string ordering{std::to_string(value)};
    switch(value) {
        case 0xA1B2C3D4: {
            ordering += " => Big endian; Nanoseconds";
            break;
        }
        case 0xA1B23C4D: {
            ordering += " => Big endian; Milliseconds";
            break;
        }
        case 0xD4C3B2A1: {
            ordering += " => Little endian; Nanoseconds";
            break;
        }
        case 0x4D3CB2A1: {
            ordering += " => Little endian; Milliseconds";
            break;
        }
        default: {
            throw std::runtime_error("Bad magic number");
        }
    }
    return ordering;
}

std::endian MagicNumber::identify_endian() const {
    std::endian endian;
    switch(value) {
        case 0xA1B2C3D4:
        case 0xA1B23C4D: {
            endian = std::endian::big;
            break;
        }
        case 0xD4C3B2A1:
        case 0x4D3CB2A1: {
            endian = std::endian::little;
            break;
        }
        default: {
            throw std::runtime_error("Bad magic number");
        }
    }
    return endian;
}

std::ostream& operator<<(std::ostream& os, const MagicNumber& header) {
    os << header.to_string();
    return os;
}
