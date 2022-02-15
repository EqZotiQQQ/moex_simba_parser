#pragma once

#include <fstream>
#include "../utils.h"

class SnapshotPacket {
friend std::ostream& operator<<(std::ostream& os, const SnapshotPacket& packet);
friend std::ofstream& operator<<(std::ofstream& os, const SnapshotPacket& packet);
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

std::ostream& operator<<(std::ostream& os, const SnapshotPacket& snapshot_packet) {
    os << "====================  Snapshot packet layer: ===================\n";
    os << snapshot_packet.sbe_message << '\n';
    os << "====================  Snapshot packet end ===================\n";
    return os;
}

std::ofstream& operator<<(std::ofstream& os, const SnapshotPacket& snapshot_packet) {
    os << "====================  Snapshot packet layer: ===================\n";
    os << snapshot_packet.sbe_message;
    os << "====================  Snapshot packet end ===================\n";
    return os;
}
