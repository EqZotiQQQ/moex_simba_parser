#pragma once

#include <cstdint>

#include <binary_parser/buffered_reader.h>

#include <fmt/format.h>
#include "magic_number.h"

struct GlobalPcapHeader {
    MagicNumber magic_number;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t time_zone;
    uint32_t sig_figs;
    uint32_t snap_len;
    uint32_t network;

    GlobalPcapHeader(BufferedReader& reader);
    void parse(BufferedReader& reader);
    std::string to_string() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const GlobalPcapHeader& header);
};
