#pragma once

#include <fstream>
#include "../types.h"
#include "../utils.h"
#include "../parsers.h"
#include "sbe/sbe_message.h"
#include "packet_base.h"

struct IncrementalPacketHeader { // Little endian
    u64 transact_time {};               // Начало процесинга транзакции в матчинге с момента начала эпохи
    u32 exchange_trading_session_ID {}; // ID торговой сессии
    static constexpr u8 size_bytes {12};

    static IncrementalPacketHeader parse(std::ifstream& file, Endian endian) {
        return IncrementalPacketHeader {
                .transact_time = Parsers::parse_u64(file, endian),
                .exchange_trading_session_ID = Parsers::parse_u32(file, endian),
        };
    }
};

struct IncrementalPacket : public PacketBase {
    IncrementalPacketHeader header {};
    std::vector<SBEMessage> sbe_messages {};
};

std::ostream& operator<<(std::ostream& os, const IncrementalPacket& packet) {
    os << "===================== IncrementalPacket ======================\n";
    for (int i = 0; i < packet.sbe_messages.size(); i++) {
        os << packet.sbe_messages[i] << '\n';
    }
    os << "===================== IncrementalPacket end ======================\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const IncrementalPacketHeader& header) {
    os << "========= IncrementalPacketHeader =========\n";
    os << "Transaction time: "            << std::dec << static_cast<u64>(header.transact_time) << '\n';
    os << "Exchange trading session ID: " << std::dec << static_cast<u32>(header.exchange_trading_session_ID) << '\n';
    os << "========= IncrementalPacketHeader end =========\n";
    return os;
}
