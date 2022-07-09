#include <iostream>
#include <gtest/gtest.h>
#include <fmt/format.h>
#include <chrono>

#include "binary_parser/buffered_reader.h"

#include "global_pcap_header.h"
#include "record_header.h"
#include "ip_header.h"
#include "udp_header.h"
#include "moex/market_data_packet.h"
#include "utility.h"
#include "app_config.h"

const std::string encoded_file_path = "../../../../assets/sample.pcap";

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(MoexSimbaParserTest, parse_global_pcap_header) {
    Config config{encoded_file_path, 1};

    BufferedReader parser{config.encoded_file_path};
    PcapConfig pcap_config;
    GlobalPcapHeader global_pcap_header{parser, pcap_config};
    std::cout << global_pcap_header << '\n';

    for (size_t i = 0; i < config.packets_to_parse; i++) {

        std::cout << "Packet number " << i+1 << '\n';
        RecordHeader record_header{parser, global_pcap_header.magic_number.is_ns()};
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

TEST(time_tranforms, transfroms_sec_us) {
    int s_1 = 1636559040;
    int us_1 = 84;
    auto res_1 = sec_us_to_ns(s_1, us_1);
    ASSERT_EQ(res_1, 1636559040000084);

    int64_t s_2 = 799268719000;
    int64_t us_2 = 0;
    auto res = sec_us_to_ns(s_2, us_2);
    ASSERT_EQ(res, 799268719000000000);
}

TEST(time_tranforms, transfroms_sec_ns) {
    int s_1 = 1;
    int ns_1 = 1;
    auto res_1 = sec_ns_to_ns(s_1, ns_1);
    ASSERT_EQ(res_1, 1000000001);

    int64_t s_2 = 5;
    int64_t ns_2 = 0;
    auto res = sec_ns_to_ns(s_2, ns_2);
    ASSERT_EQ(res, 5000000000);
}

