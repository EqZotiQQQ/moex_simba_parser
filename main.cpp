#include <iostream>
#include "pcap_parser.h"

int main() {
    const std::string path = "../../pcap_parser/assets/sample.pcap";
    PcapParser parser(path);
    return 0;
}
