#pragma once

#include <cstdint>
#include <binary_parser/buffered_reader.h>

struct OrderBookSnapshot {
    int64_t md_entry_id;
    uint64_t transact_time;
    int64_t md_entry_px;
    int64_t md_entry_size;
    int64_t trade_id;
    uint64_t md_flags;
    uint8_t md_entry_type;

    explicit OrderBookSnapshot(BufferedReader& reader);
    void parse(BufferedReader& reader);

    size_t get_parsed_bytes() const {return 0;}
    std::string to_string() const {return std::string{"TODO"};}

    friend std::ostream& operator<<(std::ostream& os, const OrderBookSnapshot& header);
};
