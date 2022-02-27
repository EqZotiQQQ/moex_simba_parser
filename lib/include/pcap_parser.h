#pragma once

#include <fstream>
#include <array>
#include <vector>

#include "exceptions.h"
#include "global_pcap_parser.h"
#include "utils/buffered_reader.h"
#include "udp_parser.h"
#include "types/constants.h"
#include "market_data_packet_parser.h"

class PcapParser {
private:
    std::optional<std::ofstream> out;
    OutputFromat out_format;
    GlobalPcapPacket pcap_packet;
    BufferedReader parser;
public:
    PcapParser(const std::string& in_path, const std::string& out_path, u32 bound):
            out(out_path),
            out_format(OutputFromat::file),
            pcap_packet(out_format, bound, out),
            parser(in_path, std::endian::big) {
        if (!out->is_open()) {
            throw FileNotFoundException();
        }
    }

    PcapParser(const std::string& in_path, u32 bound, OutputFromat out_format = OutputFromat::console):
            out({}),
            out_format(out_format),
            pcap_packet(out_format, bound, out),
            parser(in_path, std::endian::big) {
    }

    void parse() {
        pcap_packet.parse(parser);
    }
};
