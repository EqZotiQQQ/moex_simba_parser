#pragma once

#include <iostream>
#include "types.h"
#include "market_data.h"
#include "parsers.h"

struct SimbaBinaryDecoder {
    SimbaBinaryDecoder(u64 len ): packet_length(len) {}
    u64 packet_length {};
    std::unique_ptr<MessageBase> message {nullptr};

    void decode(std::ifstream& file, Endian endian) {
        auto market_data_packet_header = MarketDataPacketHeader::parse_market_data_packet_header(file, endian);
        packet_length -= MarketDataPacketHeader::size_bytes;
        std::cout << market_data_packet_header << '\n';

        if ((market_data_packet_header.msg_flags & MarketDataPacketHeader::incremental_message) == MarketDataPacketHeader::incremental_message) {
            std::vector<SBEMessage> messages {};
            IncreamentalPacketHeader incremental_packet_header = IncreamentalPacketHeader::parse(file, endian);
            packet_length -= IncreamentalPacketHeader::size_bytes;
            std::cout << incremental_packet_header << '\n';
            while (packet_length) {
                SBEMessage sbe {file, endian};
                packet_length -= sbe.parsed_bytes;
                messages.push_back(std::move(sbe));
            }
            UnsupportedMessageType::skip(file, packet_length);
        } else {
            SBEMessage sbe {file, endian};
            packet_length -= sbe.parsed_bytes;
            UnsupportedMessageType::skip(file, packet_length);
        }
    }
};

std::ostream& operator<<(std::ostream& os, const SimbaBinaryDecoder& pcap) {
    os << "====================  SimbaBinaryDecoder Header: ===================\n";

    os << "+++++++++++++++++++++ SimbaBinaryDecoder Header end: +++++++++++++++\n";
    return os;
}