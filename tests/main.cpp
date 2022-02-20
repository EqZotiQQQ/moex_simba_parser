#include <iostream>

#include "gtest/gtest.h"

#include "pcap_parser.h"

int64_t epoch() {
    // get the current time
    const auto now     = std::chrono::system_clock::now();

    // transform the time into a duration since the epoch
    const auto epoch   = now.time_since_epoch();

    // cast the duration into seconds
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);

    return seconds.count();
}

TEST(ParseSample, Parse_1000) {
    const std::string in = "../../sample.pcap";
    const std::string out = "../../decoded_1000_" + std::to_string(epoch()) + ".txt";
    int bound = 1000;

    PcapParser parser(in, out, bound);
    parser.parse();
}

TEST(ParseSample, Parse_10000) {
    const std::string in = "../../sample.pcap";
    const std::string out = "../../decoded_10000_" + std::to_string(epoch()) + ".txt";
    int bound = 10000;

    PcapParser parser(in, out, bound);
    parser.parse();
}

TEST(ParseSample, Parse_100000) {
    const std::string in = "../../sample.pcap";
    const std::string out = "../../decoded_100000_" + std::to_string(epoch()) + ".txt";
    int bound = 100000;

    PcapParser parser(in, out, bound);
    parser.parse();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
