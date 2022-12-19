#include "ip_header.h"

IpHeader::IpHeader(buffered_reader::BufferedReader &reader) {
    for (int i = 0; i < 6; i++) {
        destination_mac.ip[i] = reader.next<uint8_t>();
    }

    for (int i = 0; i < 6; i++) {
        source_mac.ip[i] = reader.next<uint8_t>();
    }

    protocol_version = ProtocolVersion(reader.next<uint16_t>(std::endian::big));
    strange_field = reader.next<uint8_t>();
    differentiated_services_field = reader.next<uint8_t>();
    total_length = reader.next<uint16_t>(std::endian::big);

    identification = reader.next<uint16_t>(std::endian::big);
    flags_and_fragment_offset = FragmentAndOffset{reader.next<uint16_t>(std::endian::big)};
    ttl = reader.next<uint8_t>();
    udp_protocol = Protocol{reader.next<uint8_t>()};
}

std::string IpHeader::to_string() const {
    return fmt::format(
            "Destination MAC: {}\n"
            "Source MAC: {}\n"
            "Protocol Version: {}\n"
            "Strange field: {}\n"
            "Differentiated service field: {}\n"
            "Total length: {}\n"
            "Identification: {}\n"
            "Flags and fragment offset: {}\n"
            "TTL: {}\n"
            "UDP protocol: {}",
            destination_mac.to_string(),
            source_mac.to_string(),
            protocol_version.to_string(),
            strange_field,
            differentiated_services_field,
            total_length,
            identification,
            flags_and_fragment_offset.to_string(),
            ttl,
            udp_protocol.to_string()
    );
}

std::ostream& operator<<(std::ostream& os, const IpHeader& header) {
    os << "\n<IP Header>\n";
    os << header.to_string();
    os << "\n</IP Header>\n";
    return os;
}
