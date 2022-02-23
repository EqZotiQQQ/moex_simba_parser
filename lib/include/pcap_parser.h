#pragma once

#include "exceptions.h"
#include "global_pcap_parser.h"
#include "utils/buffered_reader.h"
#include "udp_parser.h"
#include "types/constants.h"
#include "market_data_packet_parser.h"

#include <fstream>
#include <array>
#include <vector>


class PcapParser {
private:
    std::ifstream in;
    std::optional<std::ofstream> out;
    GlobalPcapPacket pcap_packet;
    OutputFromat out_format;
public:
    PcapParser(const std::string& in_path, const std::string& out_path, u32 bound):
            in(in_path, std::ios::in | std::ios::out | std::ios::binary),
            out(out_path),
            pcap_packet(bound),
            out_format(OutputFromat::file) {
        if (!in.is_open()) {
            throw FileNotFoundException();
        }
        if (!out->is_open()) {
            throw FileNotFoundException();
        }
    }

    PcapParser(const std::string& in_path, u32 bound, OutputFromat out_format = OutputFromat::console):
            in(in_path, std::ios::in | std::ios::out | std::ios::binary),
            out({}),
            pcap_packet(bound),
            out_format(out_format) {
        if (!in.is_open()) {
            throw FileNotFoundException();
        }
    }

    void parse() {
        pcap_packet.parse(in, out, out_format);
    }
};
