#include "ip_header.h"

IpHeader::IpHeader(BufferedReader &reader) {
    parse(reader);
}

void IpHeader::parse(BufferedReader& reader) {
    for (int i = 0; i < 6; i++) {
        destination_mac.ip[i] = reader.next<uint8_t>();
    }

    for (int i = 0; i < 6; i++) {
        source_mac.ip[i] = reader.next<uint8_t>();
    }

    protocol_version.value = reader.next<uint16_t>(std::endian::big);
    strange_field = reader.next<uint8_t>();
    differentiated_services_field = reader.next<uint8_t>();
    total_length = reader.next<uint16_t>(std::endian::big);

    identification = reader.next<uint16_t>(std::endian::big);
    flags_and_fragment_offset = FragmentAndOffset{reader.next<uint16_t>(std::endian::big)};
    ttl = reader.next<uint8_t>();
    udp_protocol = Protocol{reader.next<uint8_t>()};
}

std::ostream& operator<<(std::ostream& os, const IpHeader& header) {
    os << fmt::format(
            "Destination MAC: {}\n"
            "Source MAC: {}\n"
            "Protocol Version: {}\n"
            "Strange field: {}\n"
            "Differentiated service field: {}\n"
            "Total length: {}\n"
            "Identification: {}\n"
            "Flags and fragment offset: {}\n"
            "TTL: {}\n"
            "UDP protocol: {}\n",
            header.destination_mac.to_string(),
            header.source_mac.to_string(),
            header.protocol_version.to_string(),
            header.strange_field,
            header.differentiated_services_field,
            header.total_length,
            header.identification,
            header.flags_and_fragment_offset.to_string(),
            header.ttl,
            header.udp_protocol.to_string()
    );
    return os;
}
