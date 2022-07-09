#include <stdexcept>
#include <bit>
#include "magic_number.h"

MagicNumber::MagicNumber(): value(0) {}

MagicNumber::MagicNumber(uint32_t value): value(value) {}

std::string MagicNumber::to_string() const {
    std::string ordering{std::to_string(value)};
    return ordering;
}

std::ostream& operator<<(std::ostream& os, const MagicNumber& header) {
    os << header.to_string();
    return os;
}
