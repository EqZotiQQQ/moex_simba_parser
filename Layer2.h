#pragma  once

#include <array>
#include <fstream>
#include "types.h"
#include "utils.h"
#include "parsers.h"

struct Layer2 {
    std::array<u8, 6> destination_mac {};
    std::array<u8, 6> source_mac {};
    u16 layer_3_protocol {};

    void fill(std::ifstream& file, Endian endian) {
        for (int i = 0; i < 6; i++) {
            destination_mac[i] = file.get();
        }
        for (int i = 0; i < 6; i++) {
            source_mac[i] = file.get();
        }
        layer_3_protocol = Parsers::parse_u16(file, endian);
    }
};
