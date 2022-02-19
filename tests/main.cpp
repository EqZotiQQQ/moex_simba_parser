#include <iostream>

#include "gtest/gtest.h"

#include "pcap_parser.h"

TEST(ParseSample, Parse_1) {
    const std::string in = "../../sample.pcap";
    const std::string out = "../../decoded.txt";
    int bound = 1000;

    PcapParser parser(in, out, bound);
    parser.parse();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
