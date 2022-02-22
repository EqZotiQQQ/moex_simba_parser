#pragma once

#include <iostream>
#include <optional>
#include "types/typenames.h"
#include "utils/parsers.h"
#include "packets/market_data_packet.h"
#include "packets/incremental_packet.h"
#include "packets/snapshot_packet.h"
#include "packets/sbe/sbe_message.h"

class MarketDataPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const MarketDataPacket& packet);
private:
    u64 packet_length {};
    MarketDataPacketHeader market_data_packet_header {};
    std::optional<IncrementalPacket> incremental_packet;
    std::optional<SnapshotPacket> snapshot_packet;
public:

    MarketDataPacket() {}

    void set_len(u64 len) {
        packet_length = len;
    }

    void parse(BufferedReader& parser) {
        market_data_packet_header.parse(parser); // little endian only
        packet_length -= MarketDataPacketHeader::size;
        if (market_data_packet_header.is_incremental()) {
            incremental_packet = IncrementalPacket {packet_length};
            packet_length = incremental_packet->parse(parser);
        } else {
            snapshot_packet = SnapshotPacket {packet_length};
            packet_length = snapshot_packet->parse(parser);
        }
        parser.skip(packet_length);
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const MarketDataPacket& packet) {
    os << "== Packet layer: ==\n";
    os << packet.market_data_packet_header << std::endl;
    if (packet.market_data_packet_header.is_incremental()) {
        os << packet.incremental_packet.value() << std::endl;
    } else {
        os << packet.snapshot_packet.value() << std::endl;
    }
    os << "== Packet layer end ==\n";
    return os;
}
