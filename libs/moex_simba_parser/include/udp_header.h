#pragma once

#include <array>
#include <cstdint>
#include "binary_parser/buffered_reader.h"
#include "ip_addr.h"

struct UdpHeader {
    static constexpr size_t SIZE = 26;

    uint16_t check_sum {};
    IpAddr source_ip {};
    IpAddr dest_ip {};
    uint16_t source_port {};
    uint16_t destination_port {};
    uint16_t length {};
    uint16_t check_sum_udp {};

    UdpHeader(BufferedReader& reader);

    std::string to_string() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const UdpHeader& header);
};
