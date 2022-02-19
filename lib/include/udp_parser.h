#pragma once

#include <ostream>
#include <optional>
#include "types/typenames.h"
#include "packets/market_data_packet.h"
#include "packets/incremental_packet.h"
#include "packets/snapshot_packet.h"
#include "market_data_packet_parser.h"
#include "ip_header_parser.h"


class UDPHeader {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const UDPHeader& header);
private:
    u16 check_sum {};
    std::array<u16, 4> source_ip {};
    std::array<u16, 4> dest_ip {};
    u16 source_port {};
    u16 destination_port {};


    u16 length {};
    u16 check_sum_udp {};
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
        check_sum = Parsers::parse_u16(file, Endian::big_endian);
        source_ip = parse_ip(file);
        dest_ip = parse_ip(file);
        source_port = Parsers::parse_u16(file, Endian::big_endian);
        destination_port = Parsers::parse_u16(file, Endian::big_endian);
        length = Parsers::parse_u16(file, Endian::big_endian);
        check_sum_udp = Parsers::parse_u16(file, Endian::big_endian);
    }
};

class UDPPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const UDPPacket& udp_packet);
private:
    IpHeader ip_header;
    UDPHeader header;
    MarketDataPacket payload;
public:
    UDPPacket() {}

    void parse(std::ifstream& file, Endian endian, u32 recorded) {
        ip_header.parse(file, endian);
        header.parse(file, endian);
        payload.set_len(recorded - (16 + UDPHeader::size));
        payload.parse(file, endian);
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const UDPPacket& udp_packet) {
    os << udp_packet.ip_header << std::endl;
    os << udp_packet.header << std::endl;
    os << udp_packet.payload << std::endl;
    return os;
}

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const UDPHeader& header) {
    os << "== UDP Header: ==\n";
    os << std::dec;
    os << "Source IP: "        << static_cast<u16>(header.source_ip[0]) << ":" << static_cast<u16>(header.source_ip[1]) << ":" << static_cast<u16>(header.source_ip[2]) << ":" << static_cast<u16>(header.source_ip[3]) << '\n';
    os << "Destination IP: "   << static_cast<u16>(header.dest_ip[0]) << ":" << static_cast<u16>(header.dest_ip[1]) << ":" << static_cast<u16>(header.dest_ip[2]) << ":" << static_cast<u16>(header.dest_ip[3]) << '\n';
    os << "Source port: "      << static_cast<u16>(header.source_port) << '\n';
    os << "Destination port: " << static_cast<u16>(header.destination_port) << '\n';
    os << "Check sum: "        << static_cast<u16>(header.check_sum) << '\n';
    os << "Length:  "          << static_cast<u16>(header.length) << '\n';
    os << "Check sum udp: "    << static_cast<u16>(header.check_sum_udp) << '\n';
    os << "== UDP Header end ==\n";
    return os;
}
