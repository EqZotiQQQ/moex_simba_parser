#include <iostream>
#include <binary_parser/buffered_reader.h>
#include <argparse/argparse.hpp>

#include "global_pcap_header.h"
#include "record_header.h"
#include "ip_header.h"
#include "udp_header.h"
#include "moex/market_data_packet.h"

struct Config {
    std::string encoded_file_path;
    std::string decoded_file_path;
    size_t packets_to_parse;

    Config(int argc, char* argv[]) {
        argparse::ArgumentParser a("MOEX Simba parser");
        a.add_argument("-p", "--path")
                .help("Encoded pcap file path")
                .required();
        a.add_argument("-o", "--out")
                .default_value<std::string>("../../decoded.txt")
                .help("Decoded .txt file path")
                .required();
        a.add_argument("-n", "--n-packets-decode")
                .default_value<int>(42)
                .scan<'i', int>()
                .help("Number of packets to decode")
                .required();
        try {
            a.parse_args(argc, argv);
        }
        catch (const std::runtime_error& err) {
            std::cerr << err.what() << std::endl;
            std::cerr << a;
            std::exit(1);
        }

        encoded_file_path = a.get("-p");
        decoded_file_path = a.get("-o");
        packets_to_parse = a.get<int>("-n");
    }

    template<class O> friend O& operator<<(O& out, const Config& config) {
        out << fmt::format(
                "Decoded file path: {}\n"
                "Encoded file path: {}\n"
                "Number of packets to decode: {}\n",
                config.decoded_file_path,
                config.encoded_file_path,
                config.packets_to_parse
        );
        return out;
    }
};

int main(int argc, char* argv[]) {
    Config config = Config(argc, argv);

    BufferedReader parser{config.encoded_file_path};
    GlobalPcapHeader global_pcap_header{parser};
    std::cout << global_pcap_header << '\n';

    for (int i = 0; i < config.packets_to_parse; i++) {

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