#pragma once

#include "udp_parser.h"

class RecordHeader {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const RecordHeader& record_header);
private:
    u32 timestamp_ms {};
    u32 timestamp_us {};
    u32 pack_length {}; // Number of bytes that were captured
    u32 real_length {}; // Number of bytes that were sent
public:
    constexpr static u8 size {16};
public:
    u8 parse(BufferedReader& parser) {
        timestamp_ms = parser.next<u32>();
        timestamp_us = parser.next<u32>();
        pack_length = parser.next<u32>();
        real_length = parser.next<u32>();
        return size;
    }

    u32 get_length() const noexcept {
        return pack_length;
    }
};

class PcapPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const PcapPacket& record_header);
private:
    RecordHeader header;
    UDPPacket udp_packet;
public:

    PcapPacket() {}

    u64 parse(BufferedReader& parser) {
        u64 parsed_bytes {};
        parsed_bytes += header.parse(parser);
        parsed_bytes += udp_packet.parse(parser, header.get_length());
        return parsed_bytes;
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const PcapPacket& record) {
    os << "== PcapPacket ==\n";
    os << record.header << std::endl;
    os << record.udp_packet << std::endl;
    os << "== PcapPacket end ==\n";
    return os;
}

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const RecordHeader& record_header) {
    os << "== Record Header ==\n";
    os << std::dec;
    os << "Real length: "            << static_cast<u32>(record_header.real_length) << '\n';
    os << "Packet length: "          << static_cast<u32>(record_header.pack_length) << '\n';
    os << "Timestamp milliseconds: " << static_cast<u32>(record_header.timestamp_ms) << '\n';
    os << "Timestamp nanoseconds: "  << static_cast<u32>(record_header.timestamp_us) << '\n';
    os << "== Record Header end ==\n";
    return os;
}
