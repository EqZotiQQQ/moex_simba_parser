#pragma once

#include <fstream>

class SnapshotPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const SnapshotPacket& packet);
private:
    SBEMessage sbe_message;
    u64 size {};
public:
    SnapshotPacket(u64 size): size(size) {}

    u64 parse(std::ifstream& file, Endian endian) {
        size -= sbe_message.parse(file, endian);
        return size;
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const SnapshotPacket& snapshot_packet) {
    os << "== Snapshot packet layer: ==\n";
    os << snapshot_packet.sbe_message << std::endl;
    os << "== Snapshot packet end ==\n";
    return os;
}

