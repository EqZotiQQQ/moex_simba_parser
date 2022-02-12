#pragma once

#include <iostream>
#include <optional>
#include "types.h"
#include "market_data.h"
#include "parsers.h"

struct SimbaBinaryDecoder { // Packet layer
    SimbaBinaryDecoder(u64 len ): packet_length(len) {}
    u64 packet_length {};
    MarketDataPacketHeader market_data_packet_header {};
    std::optional<IncrementalPacket> incremental_packet {};
    std::optional<SnapshotPacket> snapshot_packet {};

    void decode(std::ifstream& file, Endian endian) {
        market_data_packet_header = MarketDataPacketHeader::parse_market_data_packet_header(file, endian);
        packet_length -= MarketDataPacketHeader::size_bytes;
        std::cout << market_data_packet_header << '\n';

        if ((market_data_packet_header.msg_flags & MarketDataPacketHeader::incremental_message) == MarketDataPacketHeader::incremental_message) {
            std::vector<SBEMessage> messages {};
            incremental_packet = IncrementalPacket{};
            incremental_packet.value().header = IncrementalPacketHeader::parse(file, endian);
            packet_length -= IncrementalPacketHeader::size_bytes;
            while (packet_length) {
                SBEMessage sbe {file, endian};
                packet_length -= sbe.parsed_bytes;
                incremental_packet.value().sbe_messages.push_back(std::move(sbe));
            }
            UnsupportedMessageType::skip(file, packet_length);
        } else { // snapshot
            snapshot_packet = SnapshotPacket {file, endian};
            snapshot_packet.value();
            SBEMessage sbe {file, endian};
            packet_length -= sbe.parsed_bytes;
            UnsupportedMessageType::skip(file, packet_length);
        }
    }
};

std::ostream& operator<<(std::ostream& os, const SimbaBinaryDecoder& pcap) {
    if (pcap.incremental_packet) {
        os << "====================  Incremental packet: ===================\n";
        os << pcap.market_data_packet_header << '\n';
        for (int i = 0; i < pcap.incremental_packet.value().sbe_messages.size(); i++) {
            os << "SBE Message number " << i << " of " << pcap.incremental_packet.value().sbe_messages.size() << '\n';
            os << pcap.incremental_packet.value().header << '\n';
            os << pcap.incremental_packet.value().sbe_messages[i].header << '\n';
            os << pcap.incremental_packet.value().sbe_messages[i].body << '\n';
        }
        os << "+++++++++++++++++++++ Incremental packet end: +++++++++++++++\n";
    } else {
        os << "====================  Snapshot packet: ===================\n";
        os << pcap.snapshot_packet.value().sbe_message.header << '\n';
        os << pcap.snapshot_packet.value().sbe_message.body << '\n';
        os << "+++++++++++++++++++++ Snapshot packet end: +++++++++++++++\n";
    }
    return os;
}