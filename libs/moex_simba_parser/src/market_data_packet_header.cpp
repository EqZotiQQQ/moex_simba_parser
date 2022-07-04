#include "moex/market_data_packet_header.h"

MarketDataPacketHeader::MarketDataPacketHeader(BufferedReader &reader) {
    parse(reader);
}

void MarketDataPacketHeader::parse(BufferedReader& reader) {
    msg_seq_number = reader.next<uint32_t>();
    msg_size = reader.next<uint16_t>();
    flags = reader.next<uint16_t>();
    sending_time = reader.next<uint64_t>();
}

std::ostream& operator<<(std::ostream& os, const MarketDataPacketHeader& header) {
    os << fmt::format(
            "Sequential number: {}\n"
            "Message size: {}\n"
            "Flags: {} TODO\n"
            "Sending time: {} TODO\n",
            header.msg_seq_number,
            header.msg_size,
            header.flags,
            header.sending_time
    );
    return os;
}
