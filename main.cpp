#include <iostream>
#include <limits.h>

#include "include/pcap_parser.h"

int main(int argc, char** argv) {
//    const std::string in = "../assets/sample.pcap";
//    const std::string out = "../decoded.txt";
//    int bound = 42;


    if (argc < 2) {
        std::cerr << "Simple cli\nUsage: ./another_pcap_parser(string) file_in file_out(string) packets_to_parse(int)\n";
        return 1;
    }
    const std::string in(argv[1]);
    const std::string out(argv[2]);
    unsigned int bound {};
    if (argc < 3) {
        bound = UINT_MAX;
    } else {
        bound = std::stoi(argv[3]);
    }

    PcapParser parser(in, out, bound);
    parser.parse();
    return 0;
}
