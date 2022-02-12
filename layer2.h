#pragma  once

#include <array>
#include <fstream>
#include "types.h"
#include "utils.h"
#include "parsers.h"

//looks like its not my case.

struct layer2 {
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

std::ostream& operator<<(std::ostream& os, const layer2& frame) {
    os  << "Destination MAC: "  << std::dec << static_cast<u16>(frame.destination_mac[0]) << ':'
                                << std::dec << static_cast<u16>(frame.destination_mac[1]) << ':'
                                << std::dec << static_cast<u16>(frame.destination_mac[2]) << ':'
                                << std::dec << static_cast<u16>(frame.destination_mac[3]) << ':'
                                << std::dec << static_cast<u16>(frame.destination_mac[4]) << ':'
                                << std::dec << static_cast<u16>(frame.destination_mac[5]) << '\n';
    os  << "Source MAC: "       << std::dec << static_cast<u16>(frame.source_mac[0]) << ':'
                                << std::dec << static_cast<u16>(frame.source_mac[1]) << ':'
                                << std::dec << static_cast<u16>(frame.source_mac[2]) << ':'
                                << std::dec << static_cast<u16>(frame.source_mac[3]) << ':'
                                << std::dec << static_cast<u16>(frame.source_mac[4]) << ':'
                                << std::dec << static_cast<u16>(frame.source_mac[5]) << '\n';
    os  << "Layer 3 protocol: " << std::dec << static_cast<u16>(frame.layer_3_protocol) << '\n';
    return os;
}
