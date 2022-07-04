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
    packet_length -= std::visit([]<typename T>(T&& t) -> size_t {
        if constexpr(
                std::is_same_v<std::decay_t<T>, IncrementalPacket> ||
                std::is_same_v<std::decay_t<T>, SnapshotPacket>) {
            return t.get_parsed_bytes();
        } else {
            throw std::runtime_error("Bad market data packet type\n");
        }
        }, packet);
    reader.skip(packet_length);
}

void MarketDataPacket::parse(MarketDataPacket& reader) {

}

std::ostream& operator<<(std::ostream& os, const MarketDataPacket& packet) {
    os << "\n<Market Data Packet>\n";
    os << packet.header << '\n';
    std::visit([&os]<typename T>(T&& arg) {
        if constexpr(
                std::is_same_v<std::decay_t<T>, IncrementalPacket> ||
                std::is_same_v<std::decay_t<T>, SnapshotPacket>) {
            os << arg;
        }
    }, packet.packet);
    os << "\n</Market Data Packet>\n";
    return os;
}

