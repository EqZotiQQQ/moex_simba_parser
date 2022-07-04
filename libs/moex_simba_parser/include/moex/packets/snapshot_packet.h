#pragma once

#include "binary_parser/buffered_reader.h"
#include "moex/sbe/sbe_message.h"

struct SnapshotPacket {

    SbeMessage sbe_message;

    explicit SnapshotPacket(BufferedReader& reader, uint32_t packet_length);
    void parse(BufferedReader& reader);

    std::string to_string() const;
    size_t get_parsed_bytes() const { return sbe_message.get_parsed_bytes(); }
    friend std::ostream& operator<<(std::ostream& os, const SnapshotPacket& header);

};
