#pragma once

#include <fstream>
#include "../types.h"
#include "../utils.h"
#include "../parsers.h"
#include "sbe/sbe_message.h"
#include "packet_base.h"

class IncrementalPacketHeader { // Little endian
    friend std::ostream& operator<<(std::ostream& os, const IncrementalPacketHeader& header);
    friend std::ofstream& operator<<(std::ofstream& os, const IncrementalPacketHeader& header);
private:
    u64 transact_time {};               // Начало процесинга транзакции в матчинге с момента начала эпохи
    u32 exchange_trading_session_ID {}; // ID торговой сессии
public:
    static constexpr u8 size {12};

    IncrementalPacketHeader() {}
    void parse(std::ifstream& file, Endian endian) {
        transact_time = Parsers::parse_u64(file, endian);
        exchange_trading_session_ID = Parsers::parse_u32(file, endian);
    }
};

class IncrementalPacket : public PacketBase {
    friend std::ostream& operator<<(std::ostream& os, const IncrementalPacket& packet);
    friend std::ofstream& operator<<(std::ofstream& os, const IncrementalPacket& packet);
private:
    IncrementalPacketHeader header {};
    std::vector<SBEMessage> sbe_messages {};
    u64 size {};
public:
    explicit IncrementalPacket(u64 len) : size(len) {}
    u64 parse(std::ifstream& file, Endian endian) {
        header.parse(file, endian);
        size -= IncrementalPacketHeader::size;
        while (size) {
            SBEMessage sbe_message {};
            size -= sbe_message.parse(file, endian);
            sbe_messages.push_back(sbe_message);
        }
        return size;
    }
};

std::ostream& operator<<(std::ostream& os, const IncrementalPacket& packet) {
    os << "== IncrementalPacket ==\n";
    os << packet.header << '\n';
    for (const auto & sbe_message : packet.sbe_messages) {
        os << sbe_message << '\n';
    }
    os << "== IncrementalPacket end ==\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const IncrementalPacketHeader& header) {
    os << "== IncrementalPacketHeader ==\n";
    os << "Transaction time: "            << std::dec << static_cast<u64>(header.transact_time) << '\n';
    os << "Exchange trading session ID: " << std::dec << static_cast<u32>(header.exchange_trading_session_ID) << '\n';
    os << "== IncrementalPacketHeader end ==\n";
    return os;
}

std::ofstream& operator<<(std::ofstream& os, const IncrementalPacket& packet) {
    os << "== IncrementalPacket ==\n";
    os << packet.header << std::endl;
    for (const auto & sbe_message : packet.sbe_messages) {
        os << sbe_message << std::endl;
    }
    os << "== IncrementalPacket end ==\n";
    return os;
}

std::ofstream& operator<<(std::ofstream& os, const IncrementalPacketHeader& header) {
    os << "== IncrementalPacketHeader ==\n";
    os << "Transaction time: "            << std::dec << static_cast<u64>(header.transact_time) << '\n';
    os << "Exchange trading session ID: " << std::dec << static_cast<u32>(header.exchange_trading_session_ID) << '\n';
    os << "== IncrementalPacketHeader end ==\n";
    return os;
}
