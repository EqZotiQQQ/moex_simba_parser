#pragma once

#include "exceptions.h"
#include "ip.h"
#include "parsers.h"
#include "utils.h"
#include "udp.h"
#include "constants.h"
#include "simba_bin_decoder.h"

#include <fstream>
#include <array>
#include <vector>

//todo: tests, benchmarks, maybe improve structure


class PcapParser {
private:
    std::ifstream in;
    std::ofstream out;
//    u64 parsed_packages {};

public:
    explicit PcapParser(const std::string& in_path, const std::string& out_path):
            in(in_path, std::ios::in | std::ios::out | std::ios::binary),
            out(out_path) {
        if (!in.is_open()) {
            throw FileNotFoundException();
        }
        if (!out.is_open()) {
            throw FileNotFoundException();
        }
    }

    u64 parse() {
        IPPacket ip;
        ip.parse(in, out);
        return 0;
    }
};
