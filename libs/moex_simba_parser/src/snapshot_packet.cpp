#include "moex/packets/snapshot_packet.h"

SnapshotPacket::SnapshotPacket(BufferedReader& reader, uint32_t packet_length):
        sbe_message{reader}{
}

void SnapshotPacket::parse(BufferedReader& reader) {

}

std::ostream &operator<<(std::ostream& os, const SnapshotPacket& header) {
    os << fmt::format(
            "sbe_message: {}\n"
            "size: {}\n",
            header.sbe_message.to_string(),
            header.size
    );
    return os;
}
