#pragma once

#include <cstdint>

#include <binary_parser/buffered_reader.h>

#include <fmt/format.h>
#include "pcap_config.h"


struct LinkType {
    enum class LinkTypeE {
        LINKTYPE_ETHERNET = 1,
        //
    } value;

    LinkType(): value(LinkTypeE::LINKTYPE_ETHERNET) {}

    explicit LinkType(uint32_t d) {
        switch(d) {
            case 1: {
                value = LinkTypeE::LINKTYPE_ETHERNET;
                break;
            }
            // Many other link types
            default: {
                throw std::runtime_error("Unsupported linktype, refer https://www.ietf.org/archive/id/draft-gharris-opsawg-pcap-01.html");
            }
        }
    }

    std::string to_string() const {
        switch(value) {
            case LinkTypeE::LINKTYPE_ETHERNET: {
                return "IEEE 802.3 Ethernet";
            }
            // Many other link types
            default: {
                throw std::runtime_error("Unsupported linktype, refer https://www.ietf.org/archive/id/draft-gharris-opsawg-pcap-01.html");
            }
        }
    }
};

struct GlobalPcapHeader {
    uint32_t magic_number;
    uint16_t version_major;
    uint16_t version_minor;
    int32_t time_zone;
    uint32_t sig_figs;
    uint32_t snap_len;
    LinkType network;

    GlobalPcapHeader(BufferedReader& reader, PcapConfig& pcap_config);
    std::string to_string() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const GlobalPcapHeader& header);
};
