#include "udp_header.h"

UdpHeader::UdpHeader(buffered_reader::BufferedReader& reader) {
    check_sum = reader.next<uint16_t>(std::endian::big);

    for (int i = 0; i < 4; i++) {
        source_ip.ip[i] = reader.next<uint8_t>();
    }

    for (int i = 0; i < 4; i++) {
        dest_ip.ip[i] = reader.next<uint8_t>();
    }

    source_port = reader.next<uint16_t>(std::endian::big);
    destination_port = reader.next<uint16_t>(std::endian::big);
    length = reader.next<uint16_t>(std::endian::big);
    check_sum_udp = reader.next<uint16_t>(std::endian::big);
}

std::string UdpHeader::to_string() const noexcept {
    return fmt::format(
            "Check sum: {}\n"
            "Source IP: {}:{}\n"
            "Destination IP: {}:{}\n"
            "Length: {} bytes\n"
            "Check sum UDP: {}",
            check_sum,
            source_ip.to_string(),
            source_port,
            dest_ip.to_string(),
            destination_port,
            length,
            check_sum_udp
    );
}

std::ostream& operator<<(std::ostream& os, const UdpHeader& header) {
    os << "\n<UDP Header>\n";
    os << header.to_string();
    os << "\n</UDP Header>\n";
    return os;
}
