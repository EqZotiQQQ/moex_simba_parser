#pragma once

#include <ostream>
#include <optional>
#include "types/typenames.h"
#include "packets/market_data_packet.h"
#include "packets/incremental_packet.h"
#include "packets/snapshot_packet.h"
#include "packet.h"

class RecordHeader {
    friend std::ostream& operator<<(std::ostream& os, const RecordHeader& record_header);
    friend std::ofstream& operator<<(std::ofstream& os, const RecordHeader& record_header);
private:
    u32 timestamp_ms {};
    u32 timestamp_us {};
    u32 pack_length {}; // Number of bytes that were captured
    u32 real_length {}; // Number of bytes that were sent
public:
    constexpr static u8 size {16};
public:
    void parse(std::ifstream& file, Endian endian) {
        timestamp_ms = Parsers::parse_u32(file, endian);
        timestamp_us = Parsers::parse_u32(file, endian);
        pack_length = Parsers::parse_u32(file, endian);
        real_length = Parsers::parse_u32(file, endian);
    }

    u32 get_length() const noexcept {
        return pack_length;
    }
};

class UDPHeader {
    friend std::ostream& operator<<(std::ostream& os, const UDPHeader& header);
    friend std::ofstream& operator<<(std::ofstream& os, const UDPHeader& header);
private:
    u32 check_sum {};
    std::array<u16, 4> source_ip {};
    std::array<u16, 4> dest_ip {};
    u32 source_port {};
    u32 destination_port {};
public:
    constexpr static u8 size {26};
public:
    std::array<u16, 4> parse_ip(std::ifstream& file) {
        std::array<u16, 4> a {};
        for (int i = 0; i < 4; i++) {
            a[i] = file.get();
        }
        return a;
    }

    void parse(std::ifstream& file, Endian endian) {
        Parsers::skip(file, 24);
        check_sum = Parsers::parse_u16(file, Endian::little_endian);
        source_ip = parse_ip(file);
        dest_ip = parse_ip(file);
        source_port = Parsers::parse_u16(file, Endian::little_endian);
        destination_port = Parsers::parse_u16(file, Endian::little_endian);
        Parsers::skip(file, 4);
    }
};

class UDPPacket {
    friend std::ostream& operator<<(std::ostream& os, const UDPPacket& udp_packet);
    friend std::ofstream& operator<<(std::ofstream& os, const UDPPacket& udp_packet);
private:
    RecordHeader record_header;
    UDPHeader header;
    Packet payload;
public:
    UDPPacket() {}

    void parse(std::ifstream& file, Endian endian) {
        record_header.parse(file, endian);
        header.parse(file, endian);
        payload.set_len(record_header.get_length() - (RecordHeader::size + UDPHeader::size));
        payload.parse(file, endian);
    }
};


std::ostream& operator<<(std::ostream& os, const UDPPacket& udp_packet) {
    os << udp_packet.record_header << '\n';
    os << udp_packet.header << '\n';
    os << udp_packet.payload << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, const RecordHeader& record_header) {
    os << "== Record Header ==\n";
    os << std::dec;
    os << "Real length: "            << static_cast<u32>(record_header.real_length) << '\n';
    os << "Packet length: "          << static_cast<u32>(record_header.pack_length) << '\n';
    os << "Timestamp milliseconds: " << static_cast<u32>(record_header.timestamp_ms) << '\n';
    os << "Timestamp nanoseconds: "  << static_cast<u32>(record_header.timestamp_us) << '\n';
    os << "== Record Header end ==\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const UDPHeader& header) {
    os << "== UDP Header: ==\n";
    os << std::dec;
    os << "Source IP: "        << static_cast<u16>(header.source_ip[0]) << ":" << static_cast<u16>(header.source_ip[1]) << ":" << static_cast<u16>(header.source_ip[2]) << ":" << static_cast<u16>(header.source_ip[3]) << '\n';
    os << "Destination IP: "   << static_cast<u16>(header.dest_ip[0]) << ":" << static_cast<u16>(header.dest_ip[1]) << ":" << static_cast<u16>(header.dest_ip[2]) << ":" << static_cast<u16>(header.dest_ip[3]) << '\n';
    os << "Source port: "      << static_cast<u16>(header.source_port) << '\n';
    os << "Destination port: " << static_cast<u16>(header.destination_port) << '\n';
    os << "Check sum: "        << static_cast<u16>(header.check_sum) << '\n';
    os << "== UDP Header end ==\n";
    return os;
}

std::ofstream& operator<<(std::ofstream& os, const UDPPacket& udp_packet) {
    os << udp_packet.record_header << std::endl;
    os << udp_packet.header << std::endl;
    os << udp_packet.payload << std::endl;
    return os;
}

std::ofstream& operator<<(std::ofstream& os, const RecordHeader& record_header) {
    os << "== Record Header ==\n";
    os << std::dec;
    os << "Real length: "            << static_cast<u32>(record_header.real_length) << '\n';
    os << "Packet length: "          << static_cast<u32>(record_header.pack_length) << '\n';
    os << "Timestamp milliseconds: " << static_cast<u32>(record_header.timestamp_ms) << '\n';
    os << "Timestamp nanoseconds: "  << static_cast<u32>(record_header.timestamp_us) << '\n';
    os << "== Record Header end ==\n";
    return os;
}

std::ofstream& operator<<(std::ofstream& os, const UDPHeader& header) {
    os << "== UDP Header: ==\n";
    os << std::dec;
    os << "Source IP: "        << static_cast<u16>(header.source_ip[0]) << ":" << static_cast<u16>(header.source_ip[1]) << ":" << static_cast<u16>(header.source_ip[2]) << ":" << static_cast<u16>(header.source_ip[3]) << '\n';
    os << "Destination IP: "   << static_cast<u16>(header.dest_ip[0]) << ":" << static_cast<u16>(header.dest_ip[1]) << ":" << static_cast<u16>(header.dest_ip[2]) << ":" << static_cast<u16>(header.dest_ip[3]) << '\n';
    os << "Source port: "      << static_cast<u16>(header.source_port) << '\n';
    os << "Destination port: " << static_cast<u16>(header.destination_port) << '\n';
    os << "Check sum: "        << static_cast<u16>(header.check_sum) << '\n';
    os << "== UDP Header end ==\n";
    return os;
}
