#pragma once

#include "types/typenames.h"
#include "messages/order_book_snapshot.h"
#include "messages/order_execution.h"
#include "messages/order_update.h"
#include "utils/parsers.h"

class MarketDataPacketHeader {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const MarketDataPacketHeader& header);
private:
    u32 msg_seq_number {};
    u16 msg_size {};
    u16 msg_flags {};
    u64 sending_time {};
public:
    constexpr static u8 size {16};

    constexpr static u8 message_fragmentation {0x1};
    constexpr static u8 first_message {0x2};
    constexpr static u8 last_message {0x4};
    constexpr static u8 incremental_message {0x8};
    constexpr static u8 pos_dup_flag {0x10};

    void parse(BufferedReader& parser) {
        msg_seq_number = parser.next<u32>(Endian::little_endian);
        msg_size = parser.next<u16>(Endian::little_endian);
        msg_flags = parser.next<u16>(Endian::little_endian);
        sending_time = parser.next<u64>(Endian::little_endian);
    }

    bool is_incremental() const noexcept {
        return (msg_flags & MarketDataPacketHeader::incremental_message) == MarketDataPacketHeader::incremental_message;
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const MarketDataPacketHeader& header) {
    os << "== MarketDataPacketHeader: ==\n";
    os << std::dec;
    os << "Порядковый номер сообщения: " << static_cast<u32>(header.msg_seq_number) << '\n';
    os << "Размер сообщения: "           << static_cast<u16>(header.msg_size) << '\n';
    os << "Флаги сообщения:\n";
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

