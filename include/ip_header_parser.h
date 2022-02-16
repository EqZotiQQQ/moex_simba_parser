#pragma once

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include "types/typenames.h"
#include "types/constants.h"
#include "utils/parsers.h"

static const char* get_type(u16 type) {
    switch (type) {
        case 0x0800: // type
            return "IPv4";
        default:
            return "Bad type";
    }
}

class IpHeader {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const IpHeader& header);
private:
    std::array<u16, 6> destination_mac {};
    std::array<u16, 6> source_mac {};
    u16 protocol_version {};

    u8 strange_field {};
    u8 differentiated_services_field {};
    u16 total_length {};
    u16 identification {};
    u16 flags_and_fragment_offset {};
    u8 ttl {};
    u8 udp_protocol {};

    std::array<u16, 6> parse_mac(std::ifstream& file) {
        std::array<u16, 6> a {};
        for (int i = 0; i < 6; i++) {
            a[i] = file.get();
        }
        return a;
    }
public:
    constexpr static u16 size = {14};
public:
    IpHeader() {}
    void parse(std::ifstream& file, Endian endian) {
        destination_mac = parse_mac(file);
        source_mac = parse_mac(file);
        protocol_version = Parsers::parse_u16(file, Endian::big_endian);
        strange_field = Parsers::parse_u8(file);
        differentiated_services_field = Parsers::parse_u8(file);
        total_length = Parsers::parse_u16(file, Endian::big_endian);
        identification = Parsers::parse_u16(file, Endian::big_endian);
        flags_and_fragment_offset = Parsers::parse_u16(file, Endian::big_endian);
        ttl = Parsers::parse_u8(file);
        udp_protocol = Parsers::parse_u8(file);
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const IpHeader& header) {
    os << "== IpHeader ==\n";
    os << std::setw(2) << std::right << std::hex;
    os << "Destination mac: "        << static_cast<u16>(header.destination_mac[0]) << ":"        << static_cast<u16>(header.destination_mac[1]) << ":"    << static_cast<u16>(header.destination_mac[2]) << ":"        << static_cast<u16>(header.destination_mac[3]) << ":"        << static_cast<u16>(header.destination_mac[4]) << ":"        << static_cast<u16>(header.destination_mac[5]) << "\n";
    os << "Source mac: "        << static_cast<u16>(header.source_mac[0]) << ":"        << static_cast<u16>(header.source_mac[1]) << ":"        << static_cast<u16>(header.source_mac[2]) << ":"        << static_cast<u16>(header.source_mac[3]) << ":"        << static_cast<u16>(header.source_mac[4]) << ":"        << static_cast<u16>(header.source_mac[5]) << "\n";
    os << std::dec;
    os << "Protocol version: " << get_type(header.protocol_version) << '\n';
    os << "Strange field: (TODO)" << header.strange_field << '\n';
    os << "Differential Services Field: (TODO)" << header.differentiated_services_field << '\n';
    os << "Total Length: " << header.total_length << '\n';
    os << "Flags and Fragment offset: (TODO) " << header.flags_and_fragment_offset << '\n';
    os << "Identification : 0x" << std::hex << header.identification << '\n';
    os << "Time to live: " << std::dec << static_cast<u16>(header.ttl) << '\n';
    os << "UDP protocol: " << header.udp_protocol << '\n';
    os << "== IpHeader end ==\n";
    return os;
}

