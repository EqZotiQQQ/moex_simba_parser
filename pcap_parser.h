#pragma once

#include "exceptions.h"
#include "pcap_header.h"
#include "parsers.h"
#include "utils.h"
#include "packet_header.h"
#include "constants.h"
#include "market_data.h"
#include "simba_bin_decoder.h"

#include <fstream>
#include <array>
#include <vector>

//todo: tests, benchmarks, maybe improve structure


class PcapParser {
private:
    std::ifstream file;
    PcapHeader header;
    Endian endian;

public:
    explicit PcapParser(const std::string& path): file(path, std::ios::in | std::ios::out | std::ios::binary) {
        if (!file.is_open()) {
            throw FileNotFoundException();
        }

        std::cout << "Header info:\n";
        header = PcapHeader::parse_pcap_header(file);
        endian = PcapHeader::validate_endians(header);
        std::cout << header << '\n';
        i32 n {};
        while (!file.eof()) {
            n++;
            std::cout << "_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+_+__________" << n << '\n';
            auto pcap_frame_header = PacketHeader::parse_pcap_frame_header(file, endian); // OK
            u64 packet_length = pcap_frame_header.pack_length - 42;
            std::cout << pcap_frame_header << '\n';   // OK
            UnsupportedMessageType::skip(file, 24);
            auto pcap_frame = PcapBody::parse(file, endian); // OK
            UnsupportedMessageType::skip(file, 4);
            std::cout << pcap_frame << '\n';
            SimbaBinaryDecoder simba_decoder(packet_length);
            simba_decoder.decode(file, endian);
            // SimbaBinaryDecoder::parse() ???
        }
    }

};
