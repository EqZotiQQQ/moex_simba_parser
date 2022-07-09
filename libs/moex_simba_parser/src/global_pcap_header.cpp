#include "global_pcap_header.h"

GlobalPcapHeader::GlobalPcapHeader(BufferedReader& reader, PcapConfig& pcap_config):
        magic_number{reader.next<uint32_t>(std::endian::big)} {
    pcap_config.init(magic_number.value);
    reader.set_endian(pcap_config.endian);
    version_major = reader.next<uint16_t>();
    version_minor = reader.next<uint16_t>();
    time_zone = reader.next<int32_t>();
    sig_figs = reader.next<uint32_t>();
    snap_len = reader.next<uint32_t>();
    network = LinkType{reader.next<uint32_t>()};
}

void GlobalPcapHeader::parse(BufferedReader& reader) {
}

std::string GlobalPcapHeader::to_string() const noexcept {
    return fmt::format(
            "Magic number: {:X}\n"
            "Version major: {}\n"
            "Version minor: {}\n"
            "Time zone: {}\n"
            "Sig figs: {}\n"
            "Snap len: {}\n"
            "Network: {}\n",
            magic_number.value,
            version_major,
            version_minor,
            time_zone,
            sig_figs,
            snap_len,
            network.to_string()
    );
}

std::ostream& operator<<(std::ostream& os, const GlobalPcapHeader& header) {
    os << header.to_string();
    return os;
}
