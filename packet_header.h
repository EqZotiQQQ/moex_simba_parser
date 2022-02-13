#pragma once

#include <ostream>
#include "types.h"
#include "layer4.h"


struct PacketHeader {
    u32 timestamp_ms {};
    u32 timestamp_us {};
    u32 pack_length {}; // Number of bytes that were captured
    u32 real_length {}; // Number of bytes that were sent

    constexpr static u8 size_bytes {8};

    static PacketHeader parse_pcap_frame_header(std::ifstream& file, Endian endian) {
        return PacketHeader {
                .timestamp_ms = Parsers::parse_u32(file, endian),
                .timestamp_us = Parsers::parse_u32(file, endian),
                .pack_length = Parsers::parse_u32(file, endian),
                .real_length = Parsers::parse_u32(file, endian),
        };
    }
};

std::ostream& operator<<(std::ostream& os, const PacketHeader& frame_header) {
    os << "PacketHeader: =======================================\n";
    os << "Real length: "            << std::dec << static_cast<u32>(frame_header.real_length) << '\n';
    os << "Packet length: "          << std::dec << static_cast<u32>(frame_header.pack_length) << '\n';
    os << "Timestamp milliseconds: " << std::dec << static_cast<u32>(frame_header.timestamp_ms) << '\n';
    os << "Timestamp nanoseconds: "  << std::dec << static_cast<u32>(frame_header.timestamp_us) << '\n';
    os << "PacketHeader end: ++++++++++++++++++++++++++++++++++++\n";
    return os;
}

//struct Frame {
//    PacketHeader pcap_frame_header;
//    layer2 l2;
//    Layer3 l3;
//    Layer4 l4;
//};

//std::ostream& operator<<(std::ostream& os, const Frame& frame) {
//    os << "frame header:  " << frame.header
//       << "\nframe.offset: "  << std::dec << frame.offset;
//    return os;
//}

//// Packet header
//typedef struct pcaprec_hdr_s {
//    guint32 ts_sec;         /* timestamp seconds */
//    guint32 ts_usec;        /* timestamp microseconds */
//    guint32 incl_len;       /* number of octets of packet saved in file */
//    guint32 orig_len;       /* actual length of packet */
//} pcaprec_hdr_t;
//
//Packet header size = 16 bytes
//
// ts_sec = 4 bytes (85 AD C7 50) *This is the number of seconds since the start of 1970, also known as Unix Epoch
// ts_usec = 4 bytes (AC 97 05 00) *microseconds part of the time at which the packet was captured
// incl_len = 4 bytes (E0 04 00 00) = 1248 *contains the size of the saved packet data in our file in bytes (following the header)
// orig_len = 4 bytes (E0 04 00 00) *Both fields' value is same here, but these may have different values in cases where we set the maximum packet length (whose value is 65535 in the global header of our file) to a smaller size.