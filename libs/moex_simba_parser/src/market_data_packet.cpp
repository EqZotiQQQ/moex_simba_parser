#include "moex/market_data_packet.h"

/*
MarketDataPacket::MarketDataPacket(BufferedReader& reader):
    header{MarketDataPacketHeader{reader}} {
    if (header.is_incremental()) {
        packet = IncrementalPacket{reader};
    } else {
        packet = SnapshotPacket{reader};
    }
    reader.skip(length);
}
*/

MarketDataPacket::MarketDataPacket(BufferedReader &reader, uint32_t packet_length):
        header{MarketDataPacketHeader{reader}} {
    packet_length -= 16;
    if (header.is_incremental()) {
        packet = IncrementalPacket{reader, packet_length};
    } else {
        packet = SnapshotPacket{reader, packet_length};
    }
    reader.skip(packet_length);
}

void MarketDataPacket::parse(MarketDataPacket& reader) {

}

std::ostream& operator<<(std::ostream& os, const MarketDataPacket& packet) {
    os << packet.header << '\n';
    std::visit([&os](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr(std::is_same_v<T, IncrementalPacket>) {
            os << "Incremental packet\n";
            os << arg;
        } else if constexpr(std::is_same_v<T, SnapshotPacket>) {
            os << "Snapshot packet\n";
            os << arg;
        }
    }, packet.packet);
    return os;
}

