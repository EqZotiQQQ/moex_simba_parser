#pragma once

#include <cstdint>
#include <binary_parser/buffered_reader.h>

#include "utility.h"

struct OrderExecution {
    int64_t md_entry_id;
    int64_t md_entry_px;
    int64_t md_entry_size;
    int64_t last_px;
    int64_t last_qty;
    int64_t trade_id;
    int64_t md_flags;
    int32_t security_id;
    uint32_t rpt_seq;
    MDUpdateAction md_update_action;
    MDEntryType md_entry_type;

    static constexpr size_t SIZE = 66;

    explicit OrderExecution(BufferedReader& reader);
    void parse(BufferedReader& reader);

    static size_t get_parsed_bytes() {return SIZE;}
    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& os, const OrderExecution& execution_order);
};