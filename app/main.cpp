#include <iostream>
#include <binary_parser/buffered_reader.h>

#include "global_pcap_header.h"
#include "record_header.h"
#include "ip_header.h"
#include "udp_header.h"
#include "moex/market_data_packet.h"

const std::string encoded_file_path = "../../assets/sample.pcap";

int main() {
    BufferedReader parser{encoded_file_path};
    GlobalPcapHeader global_pcap_header{parser};
    std::cout << global_pcap_header << '\n';

    for (int i = 0; i < 1000000; i++) {

        std::cout << "Packet number " << i+1 << '\n';
        RecordHeader record_header{parser, global_pcap_header.magic_number.is_ns()}; // TODO pass config or smth like that
        std::cout << record_header << '\n';
        uint32_t len = record_header.pack_length;

        IpHeader ip_header{parser};
        std::cout << ip_header << '\n';
        len -= IpHeader::SIZE;

        UdpHeader udp_header{parser};
        std::cout << udp_header << '\n';
        len -= UdpHeader::SIZE;

        MarketDataPacket market_data_packet{parser, len};
        std::cout << market_data_packet << '\n';
        std::cout << "\n\n\n\n";
    }
}