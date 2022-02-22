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
    std::array<u8, 4> source_ip {};
    std::array<u8, 4> dest_ip {};
    u16 source_port {};
    u16 destination_port {};


    u16 length {};
    u16 check_sum_udp {};
public:
    constexpr static u8 size {26};
public:

    u8 parse(BufferedReader& parser) {
        check_sum = parser.next<u16>(Endian::big_endian);
        source_ip = parser.next_ip();
        dest_ip = parser.next_ip();
        source_port = parser.next<u16>(Endian::big_endian);
        destination_port = parser.next<u16>(Endian::big_endian);
        length = parser.next<u16>(Endian::big_endian);
        check_sum_udp = parser.next<u16>(Endian::big_endian);
        return size;
    }
};

class UDPPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const UDPPacket& udp_packet);
private:
    IpHeader ip_header;
    UDPHeader udp_header;
    MarketDataPacket payload;
public:
    UDPPacket() {}

    u64 parse(BufferedReader& parser, u32 recorded) {
        u64 parsed_bytes {};

        parsed_bytes += ip_header.parse(parser);

        parsed_bytes += udp_header.parse(parser);

        payload.set_len(recorded - (16 + UDPHeader::size));

        parsed_bytes += payload.parse(parser);

        return parsed_bytes;
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const UDPPacket& udp_packet) {
    os << udp_packet.ip_header << std::endl;
    os << udp_packet.udp_header << std::endl;
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
