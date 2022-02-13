#pragma once

#include <fstream>
#include "../utils.h"

struct SnapshotPacket : public PacketBase {
    SBEMessage sbe_message;
    SnapshotPacket(std::ifstream& file, Endian endian): sbe_message(file, endian) {}
};
