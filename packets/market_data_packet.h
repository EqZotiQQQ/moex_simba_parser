#pragma once

#include "../types.h"
#include "../messages/order_book_snapshot.h"
#include "../messages/order_execution.h"
#include "../messages/order_update.h"
#include "../parsers.h"

class MarketDataPacketHeader { // Little endian
    friend std::ostream& operator<<(std::ostream& os, const MarketDataPacketHeader& header);
    friend std::ofstream& operator<<(std::ofstream& os, const MarketDataPacketHeader& header);
private:
    u32 msg_seq_number {};          // Счётчик. ++ когда отправляет сообщение. Ресет раз в сутки
    u16 msg_size {};                // Длина сообщения в байтах
    u16 msg_flags {};               // Флаги. Лучше читать в доке про флаги
    u64 sending_time {};            // Время отправки сообщения шлюзом
public:
    constexpr static u8 size {16};

    constexpr static u8 message_fragmentation {0x1};
    constexpr static u8 first_message {0x2};
    constexpr static u8 last_message {0x4};
    constexpr static u8 incremental_message {0x8};
    constexpr static u8 pos_dup_flag {0x10};

    void parse(std::ifstream& file, Endian endian) {
        msg_seq_number = Parsers::parse_u32(file, endian);
        msg_size = Parsers::parse_u16(file, endian);
        msg_flags = Parsers::parse_u16(file, endian);
        sending_time = Parsers::parse_u64(file, endian);
    }

    bool is_incremental() const noexcept {
        return (msg_flags & MarketDataPacketHeader::incremental_message) == MarketDataPacketHeader::incremental_message;
    }
};

std::ostream& operator<<(std::ostream& os, const MarketDataPacketHeader& header) {
    os << "== MarketDataPacketHeader: ==\n";
    os << "Порядковый номер сообщения: " << std::dec << static_cast<u32>(header.msg_seq_number) << '\n';
    os << "Размер сообщения: "           << std::dec << static_cast<u16>(header.msg_size) << '\n';
    os << "Флаги сообщения:\n";//            << std::dec << static_cast<u16>(header.msg_flags) << '\n';
    if ((header.msg_flags & MarketDataPacketHeader::message_fragmentation) == MarketDataPacketHeader::message_fragmentation) {
        os << "* 0x1 Это последний пакет фрагментированного сообщения или сообщение не фрагментировано\n";
    } else {
        os << "* !0x1 Это НЕ последний пакет фрагментированного сообщения\n";
    }
    if ((header.msg_flags & MarketDataPacketHeader::first_message) == MarketDataPacketHeader::first_message) {
        os << "* 0x2 Первое сообщение в снапшоте по инструменту\n";
    }
    if ((header.msg_flags & MarketDataPacketHeader::last_message) == MarketDataPacketHeader::last_message) {
        os << "* 0x4 Последнее сообщение в снапшоте по инструменту\n";
    }
    if ((header.msg_flags & MarketDataPacketHeader::incremental_message) == MarketDataPacketHeader::incremental_message) {
        os << "* 0x8 Incremental пакет\n";
    } else {
        os << "* !0x8 Snapshot пакет\n";
    }
    if ((header.msg_flags & MarketDataPacketHeader::pos_dup_flag) == MarketDataPacketHeader::pos_dup_flag) {
        os << "* 0x10 Трансляция полных стаканов в виде пакетов Incremental\n";
    } else {
        os << "* !0x10 Трансляция онлайн сообщений\n";
    }
    os << "Отправлено в: " << std::dec << static_cast<u64>(header.sending_time) << '\n';
    os << "== MarketDataPacketHeader end =\n";
    return os;
}


std::ofstream& operator<<(std::ofstream& os, const MarketDataPacketHeader& header) {
    os << "== MarketDataPacketHeader: ==\n";
    os << "Порядковый номер сообщения: " << std::dec << static_cast<u32>(header.msg_seq_number) << '\n';
    os << "Размер сообщения: "           << std::dec << static_cast<u16>(header.msg_size) << '\n';
    os << "Флаги сообщения:\n";//            << std::dec << static_cast<u16>(header.msg_flags) << '\n';
    if ((header.msg_flags & MarketDataPacketHeader::message_fragmentation) == MarketDataPacketHeader::message_fragmentation) {
        os << "* 0x1 Это последний пакет фрагментированного сообщения или сообщение не фрагментировано\n";
    } else {
        os << "* !0x1 Это НЕ последний пакет фрагментированного сообщения\n";
    }
    if ((header.msg_flags & MarketDataPacketHeader::first_message) == MarketDataPacketHeader::first_message) {
        os << "* 0x2 Первое сообщение в снапшоте по инструменту\n";
    }
    if ((header.msg_flags & MarketDataPacketHeader::last_message) == MarketDataPacketHeader::last_message) {
        os << "* 0x4 Последнее сообщение в снапшоте по инструменту\n";
    }
    if ((header.msg_flags & MarketDataPacketHeader::incremental_message) == MarketDataPacketHeader::incremental_message) {
        os << "* 0x8 Incremental пакет\n";
    } else {
        os << "* !0x8 Snapshot пакет\n";
    }
    if ((header.msg_flags & MarketDataPacketHeader::pos_dup_flag) == MarketDataPacketHeader::pos_dup_flag) {
        os << "* 0x10 Трансляция полных стаканов в виде пакетов Incremental\n";
    } else {
        os << "* !0x10 Трансляция онлайн сообщений\n";
    }
    os << "Отправлено в: " << std::dec << static_cast<u64>(header.sending_time) << '\n';
    os << "== MarketDataPacketHeader end ==\n";
    return os;
}

