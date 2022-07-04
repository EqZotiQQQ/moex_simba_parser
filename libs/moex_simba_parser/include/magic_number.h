#pragma once

#include <string>

struct MagicNumber {
    uint32_t value;

    MagicNumber();

    explicit MagicNumber(uint32_t value);

    std::string to_string() const;

    std::endian identify_endian() const;
    bool is_ns() const;


    friend std::ostream& operator<<(std::ostream& os, const MagicNumber& header);
};
