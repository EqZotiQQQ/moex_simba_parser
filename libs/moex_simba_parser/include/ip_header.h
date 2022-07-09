#pragma once

#include <cstdint>

#include <binary_parser/buffered_reader.h>
#include "ip_addr.h"
#include "protocol_version.h"
#include "fragment_and_offset.h"
#include "protocol.h"

struct IpHeader {
    static constexpr size_t SIZE = 16;

    MacAddr destination_mac {};
    MacAddr source_mac {};
    ProtocolVersion protocol_version {};
    uint8_t strange_field {};
    uint8_t differentiated_services_field {};
    uint16_t total_length {};
    uint16_t identification {};
    FragmentAndOffset flags_and_fragment_offset {};
    uint8_t ttl {};
    Protocol udp_protocol {};

    explicit IpHeader(BufferedReader& reader);
    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& os, const IpHeader& header);
};
