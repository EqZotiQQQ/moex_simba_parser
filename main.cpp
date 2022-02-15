#include <iostream>
#include "pcap_parser.h"

int main(int argc, char** argv) {
    const std::string in = "../../pcap_parser/assets/sample.pcap";
    const std::string out = "../decoded.txt";

//    if (argc < 2) {
//        std::cerr << "Usage: ./another_pcap_parser file_in file_out\n";
//        return 1;
//    }

    PcapParser parser(in, out);
    parser.parse();
    return 0;
}
