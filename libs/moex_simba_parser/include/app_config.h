#pragma once

#include "argparse/argparse.hpp"

struct Config {
    std::string encoded_file_path {};
    std::string decoded_file_path {};
    size_t packets_to_parse {};

    Config() {}

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