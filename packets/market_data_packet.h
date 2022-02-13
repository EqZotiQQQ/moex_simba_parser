#pragma once

#include "../types.h"
#include "../messages/order_book_snapshot.h"
#include "../messages/order_execution.h"
#include "../messages/order_update.h"
#include "../parsers.h"

struct MarketDataPacketHeader { // Little endian
    u32 msg_seq_number {};          // Счётчик. ++ когда отправляет сообщение. Ресет раз в сутки
    u16 msg_size {};                // Длина сообщения в байтах
    u16 msg_flags {};               // Флаги. Лучше читать в доке про флаги
    u64 sending_time {};            // Время отправки сообщения шлюзом
    constexpr static u8 size_bytes {16};

    constexpr static u8 message_fragmentation {0x1};
    constexpr static u8 first_message {0x2};
    constexpr static u8 last_message {0x4};
    constexpr static u8 incremental_message {0x8};
    constexpr static u8 pos_dup_flag {0x10};

    static MarketDataPacketHeader parse_market_data_packet_header(std::ifstream& file, Endian endian) {
        return MarketDataPacketHeader {
                .msg_seq_number = Parsers::parse_u32(file, endian),
                .msg_size = Parsers::parse_u16(file, endian),
                .msg_flags = Parsers::parse_u16(file, endian),
                .sending_time = Parsers::parse_u64(file, endian),
        };
    }
};

std::ostream& operator<<(std::ostream& os, const MarketDataPacketHeader& header) {
    os << "============================== MarketDataPacketHeader: =======================================\n";
    os << "Порядковый номер сообщения: " << std::dec << static_cast<u32>(header.msg_seq_number) << '\n';
    os << "Размер сообщения: "           << std::dec << static_cast<u16>(header.msg_size) << '\n';
    os << "Флаги сообщения: "            << std::dec << static_cast<u16>(header.msg_flags) << '\n';
    os << "Отправлено в: "               << std::dec << static_cast<u64>(header.sending_time) << '\n';
    os << "============================== MarketDataPacketHeader end =======================================\n";
    return os;
}

