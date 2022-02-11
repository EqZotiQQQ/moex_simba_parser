#pragma once

#include <ostream>
#include "types.h"
#include "Layer2.h"
#include "layer3.h"
#include "layer4.h"


struct FrameHeader {
    i32 timestamp_ms {};
    i32 timestamp_ns {};
    i32 pack_length {};
    i32 real_length {};
};

std::ostream& operator<<(std::ostream& os, const FrameHeader& frame_header) {
    os << "Real length: "            << std::dec << static_cast<u16>(frame_header.real_length) << '\n';
    os << "Packet length: "          << std::dec << static_cast<u16>(frame_header.pack_length) << '\n';
    os << "Timestamp milliseconds: " << std::dec << static_cast<u16>(frame_header.timestamp_ms) << '\n';
    os << "Timestamp nanoseconds: "  << std::dec << static_cast<u16>(frame_header.timestamp_ns) << '\n';
    return os;
}

struct Frame {
    FrameHeader header;
    Layer2 l2;
    Layer3 l3;
    Layer4 l4;
};

//std::ostream& operator<<(std::ostream& os, const Frame& frame) {
//    os << "frame header:  " << frame.header
//       << "\nframe.offset: "  << std::dec << frame.offset;
//    return os;
//}
