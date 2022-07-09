#include "market_data_packet_header.h"
#include "packets/incremental_packet.h"
#include "packets/snapshot_packet.h"

struct MarketDataPacket {
    MarketDataPacketHeader header;
    std::variant<std::monostate, IncrementalPacket, SnapshotPacket> packet;

    explicit MarketDataPacket(BufferedReader& reader, size_t packet_length);

    friend std::ostream& operator<<(std::ostream& os, const MarketDataPacket& header);
};
