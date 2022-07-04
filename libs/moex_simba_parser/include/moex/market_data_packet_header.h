#pragma once

#include "binary_parser/buffered_reader.h"
#include "packets/incremental_packet.h"
#include "packets/snapshot_packet.h"

struct MarketDataPacketHeader {
    uint32_t msg_seq_number;
    uint32_t msg_size;
    uint16_t flags;
    uint64_t sending_time;

    explicit MarketDataPacketHeader(BufferedReader& reader);
    void parse(BufferedReader& reader);

    bool is_incremental() const noexcept {
        return (flags & 0x8) == 0x8;
    }

    friend std::ostream& operator<<(std::ostream& os, const MarketDataPacketHeader& header);
};
