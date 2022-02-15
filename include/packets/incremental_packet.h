#pragma once

#include <fstream>
#include "types/typenames.h"
#include "utils/parsers.h"
#include "sbe/sbe_message.h"

class IncrementalPacketHeader { // Little endian
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const IncrementalPacketHeader& header);
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

class IncrementalPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const IncrementalPacket& packet);
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

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const IncrementalPacket& packet) {
    os << "== IncrementalPacket ==\n";
    os << packet.header << '\n';
    u32 i = {};
    for (const auto & sbe_message : packet.sbe_messages) {
        os << "Номер SBE сообщения: " << ++i << '\n';
        os << sbe_message << '\n';
    }
    os << "== IncrementalPacket end ==\n";
    return os;
}

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const IncrementalPacketHeader& header) {
    os << "== IncrementalPacketHeader ==\n";
    os << "Transaction time: "            << std::dec << static_cast<u64>(header.transact_time) << '\n';
    os << "Exchange trading session ID: " << std::dec << static_cast<u32>(header.exchange_trading_session_ID) << '\n';
    os << "== IncrementalPacketHeader end ==\n";
    return os;
}
