#pragma once

#include "types/typenames.h"
#include "types/constants.h"
#include "utils/buffered_reader.h"

#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>

class IpHeader {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const IpHeader& header);
private:
    std::array<u8, 6> destination_mac {};
    std::array<u8, 6> source_mac {};
    u16 protocol_version {};

    u8 strange_field {};
    u8 differentiated_services_field {};
    u16 total_length {};
    u16 identification {};
    u16 flags_and_fragment_offset {};
    u8 ttl {};
    u8 udp_protocol {};

public:
    constexpr static u16 size = {14};
public:
    IpHeader() = default;
    void parse(BufferedReader& parser) {
        destination_mac = parser.next_mac();
        source_mac = parser.next_mac();
        protocol_version = parser.next<u16>(std::endian::big);
        strange_field = parser.next<u8>();
        differentiated_services_field = parser.next<u8>();
        total_length = parser.next<u16>(std::endian::big);
        identification = parser.next<u16>(std::endian::big);
        flags_and_fragment_offset = parser.next<u16>(std::endian::big);
        ttl = parser.next<u8>();
        udp_protocol = parser.next<u8>();
    }
};

static const char* get_type(u16 type) {
    switch (type) {
        case 0x0800: // type
            return "IPv4";
        default:
            return "Bad type";
    }
}

static const u8 get_version(u8 type) {
    if ((type & 0x04) == 0x04) {
        return 4;
    } else {
        throw UnsupportedVersionException();
    }
}

static const u8 get_header_length(u8 type) {
    if ((type & 0x05) == 0x05) {
        return 20;
    } else {
        throw BadHeaderLengthException();
    }
}

static const char* get_diff_serv(u8 type) {
    if ((type & 0x0000) == 0x0000) {
        return "CS0";
    } else {
        throw BadHeaderLengthException();
    }
}

static const char* reserved_bit(u16 type) {
    if ((type >> 1) & 1) {
        return "Set";
    } else {
        return "Not set";
    }
}

static const char* dont_fragment(u16 type) {
    if ((type >> 2) & 1) {
        return "Not set";
    } else {
        return "Set";
    }
}

static const char* get_protocol(u16 type) {
    if (type == 17) {
        return "UDP";
    } else {
        throw UnsupportedProtocolException();
    }
}

static const char* more_fragments(u16 type) {
    if ((type >> 3) & 1) {
        return "Set";
    } else {
        return "Not set";
    }
}

static const int get_length(u16 type) {
    return type & 0x023C;
}

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const IpHeader& header) {
    os << "== IpHeader ==\n";
    os << std::setw(2) << std::right << std::hex;
    os << "Destination mac: " << static_cast<u16>(header.destination_mac[0]) << ":"
                              << static_cast<u16>(header.destination_mac[1]) << ":"
                              << static_cast<u16>(header.destination_mac[2]) << ":"
                              << static_cast<u16>(header.destination_mac[3]) << ":"
                              << static_cast<u16>(header.destination_mac[4]) << ":"
                              << static_cast<u16>(header.destination_mac[5]) << "\n";
    os << "Source mac: " << static_cast<u16>(header.source_mac[0]) << ":"
                         << static_cast<u16>(header.source_mac[1]) << ":"
                         << static_cast<u16>(header.source_mac[2]) << ":"
                         << static_cast<u16>(header.source_mac[3]) << ":"
                         << static_cast<u16>(header.source_mac[4]) << ":"
                         << static_cast<u16>(header.source_mac[5]) << "\n";
    os << std::dec;
    os << "Protocol version: " << get_type(header.protocol_version) << '\n';
    os << "Version: " << static_cast<u16>(get_version(header.strange_field)) << '\n';
    os << "Header length: " << static_cast<u16>(get_header_length(header.strange_field)) << '\n';
    os << "Differential Services Field: " << get_diff_serv(header.differentiated_services_field) << '\n';
    os << "Total Length: " << header.total_length << '\n';
    os << "Reserved bit: "   << reserved_bit(header.flags_and_fragment_offset) << '\n';
    os << "Don't fragment: " << dont_fragment(header.flags_and_fragment_offset) << '\n';
    os << "More fragments: " << more_fragments(header.flags_and_fragment_offset) << '\n';
    os << "Length: " << get_length(header.flags_and_fragment_offset) << '\n';

    os << "Identification : 0x" << std::hex << header.identification << '\n';
    os << "Time to live: " << std::dec << static_cast<u16>(header.ttl) << '\n';
    os << "UDP protocol: " << get_protocol(header.udp_protocol) << '\n';
    os << "== IpHeader end ==\n";
    return os;
}

