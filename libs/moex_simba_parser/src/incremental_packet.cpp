#include "moex/packets/incremental_packet.h"

IncrementalPacketHeader::IncrementalPacketHeader(buffered_reader::BufferedReader &reader) {
    transaction_time = reader.next<uint64_t>();
    exchange_trading_session_id = reader.next<uint32_t>();
}

std::ostream &operator<<(std::ostream &os, const IncrementalPacketHeader &packet) {
    os << packet.to_string();
    return os;
}

std::string IncrementalPacketHeader::to_string() const {
    return fmt::format(
            "Transaction time: {}.{}\n"
            "Exchange trading session ID: {}\n",
            to_human_readable_time(ns_to_sec(transaction_time)),
            floor_ns(transaction_time),
            exchange_trading_session_id
    );
}

IncrementalPacket::IncrementalPacket(buffered_reader::BufferedReader &reader, size_t packet_length):
        header{IncrementalPacketHeader{reader}} {
    parsed += IncrementalPacketHeader::SIZE;

    while (packet_length > parsed) {
        parsed += sbe_messages.emplace_back(reader).parsed;
    }
}

std::ostream &operator<<(std::ostream &os, const IncrementalPacket &header) {
    os << header.to_string();
    return os;
}

std::string IncrementalPacket::to_string() const {
    std::string out;
    out += header.to_string();
    for (const auto& item : sbe_messages) {
        out += item.to_string() + '\n';
    }
    return out;
}
