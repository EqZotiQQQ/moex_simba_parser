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
    std::optional<std::ofstream> out;
    BufferedReader parser;
    GlobalPcapPacket pcap_packet;
    OutputFromat out_format;
public:
    PcapParser(const std::string& in_path, const std::string& out_path, u32 bound):
            out(out_path),
            pcap_packet(bound),
            out_format(OutputFromat::file),
            parser(in_path, Endian::big_endian) {
        if (!out->is_open()) {
            throw FileNotFoundException();
        }
    }

    PcapParser(const std::string& in_path, u32 bound, OutputFromat out_format = OutputFromat::console):
            out({}),
            pcap_packet(bound),
            out_format(out_format),
            parser(in_path, Endian::big_endian) {
    }

    void parse() {
        pcap_packet.parse(parser, out, out_format);
    }
};
