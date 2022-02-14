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
    std::ifstream file;
    u64 parsed_packages {};

public:
    explicit PcapParser(const std::string& path): file(path, std::ios::in | std::ios::out | std::ios::binary) {
        if (!file.is_open()) {
            throw FileNotFoundException();
        }
    }

    u64 parse() {
        IPPacket ip;
        ip.parse(file);
        std::cout << ip << '\n';
        return 0;
    }
};
