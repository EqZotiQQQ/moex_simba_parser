#include "global_pcap_header.h"

GlobalPcapHeader::GlobalPcapHeader(BufferedReader& reader) {
    parse(reader);
}

void GlobalPcapHeader::parse(BufferedReader& reader) {
    magic_number.value = reader.next<uint32_t>(std::endian::big);

    reader.set_endian(magic_number.identify_endian());

    version_major = reader.next<uint16_t>();
    version_minor = reader.next<uint16_t>();
    time_zone = reader.next<int32_t>();
    sig_figs = reader.next<uint32_t>();
    snap_len = reader.next<uint32_t>();
    network = reader.next<uint32_t>();
}

std::string GlobalPcapHeader::to_string() const noexcept {
    return fmt::format(
            "Magic number: {}\n"
            "Version major: {}\n"
            "Version minor: {}\n"
            "Time zone: {}\n"
            "Sig figs: {}\n"
            "Snap len: {}\n"
            "Network: {}\n",
            magic_number.to_string(),
            version_major,
            version_minor,
            time_zone,
            sig_figs,
            snap_len,
            network
    );
}

std::ostream& operator<<(std::ostream& os, const GlobalPcapHeader& header) {
    os << header.to_string();
    return os;
}
