#pragma once

#include <vector>
#include "binary_parser/buffered_reader.h"
#include "moex/sbe/sbe_message.h"

struct IncrementalPacketHeader {
    uint64_t transaction_time;
    uint32_t exchange_trading_session_id;

    static constexpr size_t SIZE {12};

    explicit IncrementalPacketHeader(buffered_reader::BufferedReader& reader);

    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const IncrementalPacketHeader& header);
};

struct IncrementalPacket {

    IncrementalPacketHeader header;
    std::vector<SbeMessage> sbe_messages {};

    size_t parsed = 0;

    explicit IncrementalPacket(buffered_reader::BufferedReader& reader, size_t packet_length);

    size_t get_parsed_bytes() const { return parsed; }
    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const IncrementalPacket& header);
};
