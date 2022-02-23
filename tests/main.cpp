#include <iostream>

#include "gtest/gtest.h"

#include "pcap_parser.h"
#include "utils/buffered_reader.h"

int64_t epoch() {
    // get the current time
    const auto now     = std::chrono::system_clock::now();

    // transform the time into a duration since the epoch
    const auto epoch   = now.time_since_epoch();

    // cast the duration into seconds
    const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(epoch);

    return seconds.count();
}

TEST(ParseSample, Parse_1000) {
    const std::string in = "../../sample.pcap";
    const std::string out = "../../decoded_1000_" + std::to_string(epoch()) + ".txt";
    int bound = 1000;

    PcapParser parser(in, out, bound);
    parser.parse();
}

TEST(ParseSample, Parse_39089) {
    const std::string in = "../../sample.pcap";
    const std::string out = "../../decoded_39089_" + std::to_string(epoch()) + ".txt";
    int bound = 1000;

    PcapParser parser(in, out, bound);
    parser.parse();
}

TEST(ParseSample, Parse_10000) {
    const std::string in = "../../sample.pcap";
    const std::string out = "../../decoded_10000_" + std::to_string(epoch()) + ".txt";
    int bound = 10000;

    PcapParser parser(in, out, bound);
    parser.parse();
}

TEST(ParseSample, Parse_100000) {
    const std::string in = "../../sample.pcap";
    const std::string out = "../../decoded_100000_" + std::to_string(epoch()) + ".txt";
    int bound = 100000;

    PcapParser parser(in, out, bound);
    parser.parse();
}

TEST(BufferedReaderTest, parse_endian) {
    const std::string in = "../../sample.pcap";
    BufferedReader reader {in, Endian::little_endian};
    auto little_endian_marker = 0xD4C3B2A1;
    u32 endian = reader.next<u32>(Endian::big_endian);
    std::cout << endian << '\n';
    ASSERT_EQ(little_endian_marker, endian);
    u16 major = reader.next<u16>();
    std::cout << major << '\n';
    ASSERT_EQ(major, 2);
    u16 minor = reader.next<u16>();
    std::cout << minor << '\n';
    ASSERT_EQ(minor, 4);
    u32 time_zone = reader.next<i32>();
    std::cout << time_zone << '\n';
    ASSERT_EQ(time_zone, 0);
    u32 sig_figs = reader.next<u32>();
    std::cout << sig_figs << '\n';
    ASSERT_EQ(sig_figs, 0);
    u32 snap_len = reader.next<u32>();
    std::cout << snap_len << '\n';
    ASSERT_EQ(snap_len, 65535);
    u32 network = reader.next<u32>();
    std::cout << network << '\n';
    ASSERT_EQ(network, 1);
}

TEST(BufferedReaderTest, parse_u64) {
    const std::string in = "../../tests/00_to_ff.bin";
    std::ifstream f;
    BufferedReader reader {in, Endian::little_endian};

    std::cout << "Parsed u64: " << reader.next<u64>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u64: " << reader.next<u64>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u64: " << reader.next<u64>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u64: " << reader.next<u64>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u64: " << reader.next<u64>() << std::endl;
    std::cout << reader << '\n';
}


TEST(BufferedReaderTest, parse_u32) {
    const std::string in = "../../tests/00_to_ff.bin";
    std::ifstream f;
    BufferedReader reader {in, Endian::little_endian};
    std::cout << "Parsed u32: " << reader.next<u32>() << std::endl;
    std::cout << reader << '\n';
    std::cout << "Parsed u32: " << reader.next<u32>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u32: " << reader.next<u32>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u32: " << reader.next<u32>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u32: " << reader.next<u32>() << std::endl;
    std::cout << reader << '\n';
}

TEST(BufferedReaderTest, parse_u16) {
    const std::string in = "../../tests/00_to_ff.bin";
    std::ifstream f;
    BufferedReader reader {in, Endian::little_endian};

    std::cout << "Parsed u16: " << reader.next<u16>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u16: " << reader.next<u16>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u16: " << reader.next<u16>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u16: " << reader.next<u16>() << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u16: " << reader.next<u16>() << std::endl;
    std::cout << reader << '\n';
}

TEST(BufferedReaderTest, parse_u8) {
    const std::string in = "../../tests/00_to_ff.bin";
    std::ifstream f;
    BufferedReader reader {in, Endian::little_endian};

    std::cout << "Parsed u8: " << static_cast<u16>(reader.next<u8>()) << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u8: " << static_cast<u16>(reader.next<u8>()) << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u8: " << static_cast<u16>(reader.next<u8>()) << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u8: " << static_cast<u16>(reader.next<u8>()) << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u8: " << static_cast<u16>(reader.next<u8>()) << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u8: " << static_cast<u16>(reader.next<u8>()) << std::endl;
    std::cout << reader << '\n';

    std::cout << "Parsed u8: " << static_cast<u16>(reader.next<u8>()) << std::endl;
    std::cout << reader << '\n';
}


TEST(ParseSample, Parse_1024_bytes) {
    const std::string in = "../../sample.pcap";
    BufferedReader reader {in, Endian::little_endian};
    reader.next<i32>();
    std::cout << reader << '\n';
}

TEST(ParseSample, Parse_00_to_ff) {
    const std::string in = "../../tests/00_to_ff.bin";
    BufferedReader reader {in, Endian::little_endian};
    reader.next<i64>();
    std::cout << reader << '\n';
    reader.next<i64>();
    std::cout << reader << '\n';
    reader.next<i64>();
    std::cout << reader << '\n';
    reader.next<i64>();
    std::cout << reader << '\n';
    reader.next<i64>();
    std::cout << reader << '\n';
    reader.next<i64>();
    std::cout << reader << '\n';
    reader.next<i64>();
    std::cout << reader << '\n';
}

TEST(ParseSample, skip_10) {
    const std::string in = "../../tests/00_to_ff.bin";
    BufferedReader reader {in, Endian::little_endian};
    reader.next<u32>();
    std::cout << reader << '\n';
    reader.skip(10);
    std::cout << reader << '\n';
    reader.next<u32>();
    std::cout << reader << '\n';
}

TEST(ParseSample, skip_1) {
    const std::string in = "../../tests/00_to_ff.bin";
    BufferedReader reader {in, Endian::little_endian};
    reader.next<u32>();
    std::cout << reader << '\n';
    reader.skip(20);
    std::cout << reader << '\n';
    reader.next<u32>();
    std::cout << reader << '\n';
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
