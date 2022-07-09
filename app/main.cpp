#include <iostream>
#include <binary_parser/buffered_reader.h>

#include "global_pcap_header.h"
#include "record_header.h"
#include "ip_header.h"
#include "udp_header.h"
#include "moex/market_data_packet.h"
#include "pcap_config.h"
#include "app_config.h"

int main(int argc, char* argv[]) {
    Config config = Config(argc, argv);

    BufferedReader parser{config.encoded_file_path};
    PcapConfig pcap_config;
    GlobalPcapHeader global_pcap_header{parser, pcap_config};
    std::cout << global_pcap_header << '\n';

    for (size_t i = 0; i < config.packets_to_parse; i++) {
        RecordHeader record_header{parser, pcap_config};
        IpHeader ip_header{parser};
        UdpHeader udp_header{parser};
        size_t headerless_len = record_header.pack_length - IpHeader::SIZE - UdpHeader::SIZE;
        MarketDataPacket market_data_packet{parser, headerless_len};


        std::cout << "Packet number " << i+1 << '\n';
        std::cout << record_header << '\n';
        std::cout << ip_header << '\n';
        std::cout << udp_header << '\n';
        std::cout << market_data_packet << '\n';
        std::cout << "\n\n\n\n";
    }
}