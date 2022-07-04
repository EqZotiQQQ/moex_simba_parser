#pragma once

#include <cstdint>
#include <string>

enum class Bit {
    UNSET = 0,
    SET = 1,
};

enum class IdentityBits {
    IDENTITY_RESERVE_BIT = 1,
    IDENTITY_DONT_FRAGMENT_BIT = 2,
    IDENTITY_MORE_FRAGMENTS_BIT = 3,
};

struct FragmentAndOffset {

    uint16_t value {};


    FragmentAndOffset(): value(0) {}

    explicit FragmentAndOffset(uint16_t fo): value(fo) {}

    std::string to_string() const {
        std::string out = "\n";
        if (is_bit_set(IdentityBits::IDENTITY_RESERVE_BIT) == Bit::SET) {
            out += "* Reserve bit set\n";
        } else {
            out += "* Reserve bit unset\n";
        }
        if (is_bit_set(IdentityBits::IDENTITY_DONT_FRAGMENT_BIT) == Bit::SET) {
            out += "* Don't fragment bit set\n";
        } else {
            out += "* Don't fragment bit unset\n";
        }
        if (is_bit_set(IdentityBits::IDENTITY_MORE_FRAGMENTS_BIT) == Bit::SET) {
            out += "* More fragments bit set\n";
        } else {
            out += "* More fragments bit unset\n";
        }
        out += "* Length bit: " + std::to_string(get_length_bit());
        return out;
    }

    Bit is_bit_set(IdentityBits bit) const {
        return ((value >> static_cast<uint8_t>(bit)) & 1) == 1 ? Bit::SET : Bit::UNSET;
    }

    uint16_t get_length_bit() const {
        return value & 0x023C;
    }

    friend std::ostream& operator<<(std::ostream& os, const FragmentAndOffset& header) {
        os << header.to_string();
        return os;
    }
};
