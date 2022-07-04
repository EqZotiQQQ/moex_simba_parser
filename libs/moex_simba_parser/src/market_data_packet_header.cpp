#include "moex/market_data_packet_header.h"

MarketDataPacketHeader::MarketDataPacketHeader(BufferedReader& reader):
        msg_seq_number{reader.next<uint32_t>()},
        msg_size{reader.next<uint16_t>()},
        flags{reader.next<uint16_t>()},
        sending_time{reader.next<uint64_t>()}
{}

void MarketDataPacketHeader::parse(BufferedReader& reader) {
}

std::ostream& operator<<(std::ostream& os, const MarketDataPacketHeader& header) {
    os << "<Market Data Packet Header>\n";
    os << header.to_string();
    os << "\n</Market Data Packet Header>\n";
    return os;
}

std::string MarketDataPacketHeader::to_string() const {
    return fmt::format(
            "Sequential number: {}\n"
            "Message size: {}\n"
            "Flags: {} TODO\n"
            "Sending time: {} TODO",
            msg_seq_number,
            msg_size,
            flags.to_string(),
            sending_time
    );
}
