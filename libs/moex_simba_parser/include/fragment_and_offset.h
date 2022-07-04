#pragma once

#include <cstdint>
#include <string>

enum class Bit {
    SET,
    UNSET,
};

struct FragmentAndOffset {

    uint16_t value {};

    FragmentAndOffset(): value(0) {}

    explicit FragmentAndOffset(uint16_t fo): value(fo) {}

    std::string to_string() const {
        std::string out = "\n";
        if (identify_reserve_bit() == Bit::SET) {
            out += "* Reserve bit set\n";
        } else {
            out += "* Reserve bit unset\n";
        }
        if (identify_dont_fragment_bit() == Bit::SET) {
            out += "* Don't fragment bit set\n";
        } else {
            out += "* Don't fragment bit unset\n";
        }
        if (identify_more_fragments_bit() == Bit::SET) {
            out += "* More fragments bit set\n";
        } else {
            out += "* More fragments bit unset\n";
        }
        out += "* Length bit: " + std::to_string(get_length_bit());
        return out;
    }

    Bit identify_reserve_bit() const {
        return ((value >> 1) & 1) == 1 ? Bit::SET : Bit::UNSET;
    }

    Bit identify_dont_fragment_bit() const {
        return ((value >> 2) & 1) == 1 ? Bit::SET : Bit::UNSET;
    }

    Bit identify_more_fragments_bit() const {
        return ((value >> 3) & 1) == 1 ? Bit::SET : Bit::UNSET;
    }

    uint16_t get_length_bit() const {
        return value & 0x023C;
    }

    friend std::ostream& operator<<(std::ostream& os, const FragmentAndOffset& header) {
        os << header.to_string();
        return os;
    }
};

