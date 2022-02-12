#pragma once

#include "types.h"
#include "messages/order_book_snapshot.h"
#include "messages/order_execution.h"
#include "messages/order_update.h"
#include "messages/unsupported.h"

struct SBEHeader {
    u16 block_length {};  // Длина корневой части сообщения, кроме самой SBE Header и NoMDEntries
    u16 template_ID {}; // ID сообщения
    u16 schema_ID {};   // ID схемы сообщения
    u16 version {};     // Версия схемы
    constexpr static u8 size_bytes {8};

    static SBEHeader parse(std::ifstream& file, Endian endian) {
        return SBEHeader {
                .block_length = Parsers::parse_u16(file, endian),
                .template_ID = Parsers::parse_u16(file, endian),
                .schema_ID = Parsers::parse_u16(file, endian),
                .version = Parsers::parse_u16(file, endian),
        };
    }
};

std::ostream& operator<<(std::ostream& os, const SBEHeader& header) {
    os << "===================== SBEHeader ======================\n";
    os << "Block length: "      << std::dec << static_cast<u32>(header.block_length) << '\n';
    os << "Template ID: "       << std::dec << static_cast<u16>(header.template_ID) << '\n';
    os << "Schema ID: "         << std::dec << static_cast<u16>(header.schema_ID) << '\n';
    os << "Version: "           << std::dec << static_cast<u64>(header.version) << '\n';
    os << "===================== SBEHeader end ======================\n";
    return os;
}

struct SBEMessage {
    SBEHeader header {};
    std::unique_ptr<MessageBase> body {nullptr};
    u64 parsed_bytes = {};

    SBEMessage(std::ifstream& file, Endian endian) {
        header = SBEHeader::parse(file, endian);
        parsed_bytes += SBEHeader::size_bytes;
        switch (header.template_ID) {
            case MessageType::OrderUpdate: {
                body = std::make_unique<OrderUpdate>(OrderUpdate::parse(file, endian));
                parsed_bytes += OrderUpdate::size_bytes;
                break;
            }
            case MessageType::OrderExecution: {
                body = std::make_unique<OrderExecution>(OrderExecution::parse(file, endian));
                parsed_bytes += OrderExecution::size_bytes;
                break;
            }
            case MessageType::OrderBookSnapshot: {
                body = std::make_unique<OrderBookSnapshot>(OrderBookSnapshot::parse(file, endian));
                parsed_bytes += OrderBookSnapshot::size_bytes;
                break;
            }
            default: {
                UnsupportedMessageType::skip(file, header.block_length);
                parsed_bytes += header.block_length;
                break;
            }
        }

        std::cout << header << '\n';
    }
};

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
    os << "Sequential number: " << std::dec << static_cast<u32>(header.msg_seq_number) << '\n';
    os << "Message size: "      << std::dec << static_cast<u16>(header.msg_size) << '\n';
    os << "Message flags: "     << std::dec << static_cast<u16>(header.msg_flags) << '\n';
    os << "Sent at: "           << std::dec << static_cast<u64>(header.sending_time) << '\n';
    os << "============================== MarketDataPacketHeader end =======================================\n";
    return os;
}

struct PacketBase {
    virtual ~PacketBase() {}
};

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


struct SnapshotPacket : public PacketBase {
    SBEMessage sbe_message;
    SnapshotPacket(std::ifstream& file, Endian endian): sbe_message(file, endian) {}
};

std::ostream& operator<<(std::ostream& os, const IncrementalPacketHeader& header) {
    os << "===================== IncrementalPacketHeader ======================\n";
    os << "Transaction time: "            << std::dec << static_cast<u64>(header.transact_time) << '\n';
    os << "Exchange trading session ID: " << std::dec << static_cast<u32>(header.exchange_trading_session_ID) << '\n';
    os << "===================== IncrementalPacketHeader end ======================\n";
    return os;
}



