#pragma once

#include "binary_parser/buffered_reader.h"
#include "moex/sbe/sbe_message.h"

struct SnapshotPacket {

    SbeMessage sbe_message;
    uint64_t size;

    explicit SnapshotPacket(BufferedReader& reader, uint32_t packet_length);
    void parse(BufferedReader& reader);

    friend std::ostream& operator<<(std::ostream& os, const SnapshotPacket& header);

};