#pragma once

#include "exceptions.h"
#include "global_pcap.h"
#include "utils/parsers.h"
#include "udp_parser.h"
#include "types/constants.h"
#include "market_data_packet.h"

#include <fstream>
#include <array>
#include <vector>

//todo: tests, benchmarks, maybe improve structure


class PcapParser {
private:
    std::ifstream in;
    std::ofstream out;
    GlobalPcapPacket pcap_packet;

public:
    explicit PcapParser(const std::string& in_path, const std::string& out_path, u32 bound):
            in(in_path, std::ios::in | std::ios::out | std::ios::binary),
            out(out_path),
            pcap_packet(bound) {
        if (!in.is_open()) {
            throw FileNotFoundException();
        }
        if (!out.is_open()) {
            throw FileNotFoundException();
        }
    }

    void parse() {
        pcap_packet.parse(in, out);
    }
};
