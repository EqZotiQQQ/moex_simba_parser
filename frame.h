#pragma once

#include <cstdint>
#include <ostream>

using u32 = uint32_t;
using u16 = uint16_t;
using i32 = int32_t;
using i64 = int64_t;

struct FrameHeader {
    i32 timestamp_ms;
    i32 timestamp_ns;
    i32 pack_length;
    i32 real_length;
};

std::ostream& operator<<(std::ostream& os, const FrameHeader& frame_header) {
    os  << "frame_header.real_length:    " << std::dec << frame_header.real_length
        << "\nframe_header.pack_length:  " << std::dec << frame_header.pack_length
        << "\nframe_header.timestamp_ms: " << std::dec << frame_header.timestamp_ms
        << "\nframe_header.timestamp_ns: " << std::dec << frame_header.timestamp_ns;
    return os;
}

struct Frame {
    FrameHeader header;
    i64 offset;
    i32 len () const { return header.real_length; }
};

std::ostream& operator<<(std::ostream& os, const Frame& frame) {
    os << "frame header:  " << frame.header
       << "\nframe.offset: "  << std::dec << frame.offset;
    return os;
}
