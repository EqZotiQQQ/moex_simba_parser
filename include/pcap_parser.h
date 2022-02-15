#pragma once

#include "exceptions.h"
#include "ip.h"
#include "utils/parsers.h"
#include "udp.h"
#include "types/constants.h"
#include "packet.h"

#include <fstream>
#include <array>
#include <vector>

//todo: tests, benchmarks, maybe improve structure


class PcapParser {
private:
    std::ifstream in;
    std::ofstream out;
    IPPacket ip;

public:
    explicit PcapParser(const std::string& in_path, const std::string& out_path, u32 bound):
            in(in_path, std::ios::in | std::ios::out | std::ios::binary),
            out(out_path),
            ip(bound) {
        if (!in.is_open()) {
            throw FileNotFoundException();
        }
        if (!out.is_open()) {
            throw FileNotFoundException();
        }
    }

    void parse() {
        ip.parse(in, out);
    }
};
