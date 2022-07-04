#include "moex/packets/snapshot_packet.h"

SnapshotPacket::SnapshotPacket(BufferedReader& reader, uint32_t packet_length):
        sbe_message{reader} {
}

void SnapshotPacket::parse(BufferedReader& reader) {

}

std::ostream &operator<<(std::ostream& os, const SnapshotPacket& header) {
    os << header.to_string();
    return os;
}

std::string SnapshotPacket::to_string() const {
    return fmt::format(
            "sbe_message: {}\n",
            sbe_message.to_string()
    );
}
