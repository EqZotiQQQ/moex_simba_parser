#pragma once

#include <cstdint>
#include <binary_parser/buffered_reader.h>
#include "pcap_config.h"

struct RecordHeader {
    uint32_t seconds;       // seconds
    uint32_t secondary_time; // us or ns -> depends on magic number
    uint32_t pack_length;
    uint32_t real_length;

    Time time_format;

    explicit RecordHeader(BufferedReader& reader, const PcapConfig& pcap_config);
    void parse(BufferedReader& reader);

    friend std::ostream& operator<<(std::ostream& os, const RecordHeader& header);

    std::string to_string() const;
};
