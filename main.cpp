#include <iostream>
#include <limits.h>

#include "include/pcap_parser.h"

int main(int argc, char** argv) {
    const std::string in = "../sample.pcap";
    const std::string out = "../decoded.txt";
    int bound = 1000;


//    if (argc < 2) {
//        std::cerr << "Simple cli\nUsage: ./another_pcap_parser(string) file_in file_out(string) packets_to_parse(int)\n";
//        return 1;
//    }
//    const std::string in(argv[1]);
//    const std::string out(argv[2]);
//    unsigned int bound {};
//    if (argc < 3) {
//        bound = UINT_MAX;
//    } else {
//        bound = std::stoi(argv[3]);
//    }

    PcapParser parser(in, out, bound);
    parser.parse();

    return 0;
}

// Debug:
//./another_pcap_parser  0.00s user 0.02s system 74% cpu 0.035 total - 1_000 пакетов
//./another_pcap_parser  1.12s user 1.01s system 95% cpu 2.231 total - 100_000
//./another_pcap_parser  4.00s user 2.89s system 99% cpu 6.943 total - 300_000

// Release:
// ./another_pcap_parser  0.79s user 0.92s system 95% cpu 1.790 total - 100_000
// ./another_pcap_parser  2.45s user 2.91s system 98% cpu 5.434 total - 300_000