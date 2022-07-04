#pragma once

#include <cstdint>
#include <binary_parser/buffered_reader.h>

struct RecordHeader {
    uint32_t ts_ms;
    uint32_t ts_ns;
    uint32_t pack_length;
    uint32_t real_length;

    explicit RecordHeader(BufferedReader& reader);
    void parse(BufferedReader& reader);

    friend std::ostream& operator<<(std::ostream& os, const RecordHeader& header);

    std::string to_string() const;
};
