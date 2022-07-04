#include <variant>
#include "market_data_packet_header.h"
#include "packets/incremental_packet.h"
#include "packets/snapshot_packet.h"

struct MarketDataPacket {
    MarketDataPacketHeader header;
    std::variant<std::monostate, IncrementalPacket, SnapshotPacket> packet;

    explicit MarketDataPacket(BufferedReader& reader, uint32_t packet_length);
//    explicit MarketDataPacket(BufferedReader& reader);
    void parse(MarketDataPacket& reader);

    friend std::ostream& operator<<(std::ostream& os, const MarketDataPacket& header);
};
