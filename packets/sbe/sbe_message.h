#pragma once


#include <fstream>
#include <iostream>
#include <unordered_map>
#include "../../types.h"
#include "../../utils.h"
#include "../../parsers.h"
#include "../../messages/order_update.h"
#include "../../ip.h"
#include "../../messages/order_execution.h"
#include "../../messages/order_book_snapshot.h"
#include "../../messages/order_best_prices.h"

struct MessageId {
    u16 template_id {};
    std::string message_name {};
};

const static std::unordered_map<i32, MessageId> message_id {
    std::pair(1, MessageId{MessageType::Hearthbeat, "Hearthbeath"}),
    std::pair(2, MessageId{MessageType::SequenceReset, "SequenceReset"}),
    std::pair(3, MessageId{MessageType::BestPrices, "BestPrices"}),
    std::pair(4, MessageId{MessageType::EmptyBook, "EmptyBook"}),
    std::pair(5, MessageId{MessageType::OrderUpdate, "OrderUpdate"}),
    std::pair(6, MessageId{MessageType::OrderExecution, "OrderExecution"}),
    std::pair(7, MessageId{MessageType::OrderBookSnapshot, "OrderBookSnapshot"}),
    std::pair(8, MessageId{MessageType::SecurityDefinition, "SecurityDefinition"}),
    std::pair(9, MessageId{MessageType::SecurityStatus, "SecurityStatus"}),
    std::pair(10, MessageId{MessageType::SecurityDefinitionUpdateReport, "SecurityDefinitionUpdateReport"}),
    std::pair(11, MessageId{MessageType::TradingSessionStatus, "TradingSessionStatus"}),
    std::pair(1000, MessageId{MessageType::Logon, "Logon"}),
    std::pair(1001, MessageId{MessageType::Logout, "Logout"}),
    std::pair(1002, MessageId{MessageType::MarketDataRequest, "MarketDataRequest"}),
};

struct SBEHeader {
    u16 block_length {};  // Длина корневой части сообщения, кроме самой SBE Header и NoMDEntries
    u16 template_ID {}; // ID сообщения
    u16 schema_ID {};   // ID схемы сообщения
    u16 version {};     // Версия схемы

    constexpr static u8 size_bytes {8};

    void parse(std::ifstream& file, Endian endian) {
        block_length = Parsers::parse_u16(file, endian);
        template_ID = Parsers::parse_u16(file, endian);
        schema_ID = Parsers::parse_u16(file, endian);
        version = Parsers::parse_u16(file, endian);
    }
};

std::ostream& operator<<(std::ostream& os, const SBEHeader& header) {
    os << "======= SBEHeader =======\n";
    os << "Длина блока: "       << std::dec << static_cast<u32>(header.block_length) << '\n';
    os << "Template ID: "       << std::dec << static_cast<u16>(header.template_ID) << '\n';
    os << "Schema ID: "         << std::dec << static_cast<u16>(header.schema_ID) << '\n';
    os << "Version: "           << std::dec << static_cast<u64>(header.version) << '\n';
    os << "======= SBEHeader end =======\n";
    return os;
}

struct SBEMessage {
    SBEHeader header {};
    std::optional<OrderUpdate> order_update;
    std::optional<OrderExecution> order_execution;
    std::optional<OrderBookSnapshotPacket> order_book_snapshot_packet;
    u64 size = {};

    SBEMessage() {

    }

    u64 parse(std::ifstream& file, Endian endian) {
        header.parse(file, endian);
        size += SBEHeader::size_bytes;
        std::cout << header << '\n';
        if (message_id.contains(header.template_ID)) {
            switch (message_id.at(header.template_ID).template_id) {
                case MessageType::OrderUpdate: {
                    std::cout << "Parsed MessageType::OrderUpdate" << '\n';
                    order_update = OrderUpdate();
                    order_update->parse(file, endian);
                    size += OrderUpdate::size_bytes;
                    break;
                }
                case MessageType::OrderExecution: {
                    std::cout << "Parsed MessageType::OrderExecution" << '\n';
                    order_execution = OrderExecution {};
                    order_execution->parse(file, endian);
                    size += OrderExecution::size_bytes;
                    break;
                }
                case MessageType::OrderBookSnapshot: {
                    std::cout << "Parsed MessageType::OrderBookSnapshot" << '\n';
                    order_book_snapshot_packet = OrderBookSnapshotPacket {};
                    order_book_snapshot_packet->parse(file, endian);
                    size += OrderBookSnapshot::size * order_book_snapshot_packet->entries() + OrderBookSnapshotPacket::size;
                    break;
                }
                case MessageType::BestPrices: {
                    std::cout << "Parsed MessageType::BestPrices" << '\n';
                    BestPricesOrder best_prices_order = BestPricesOrder {};
                    best_prices_order.parse(file, endian);
                    size += BestPricesOrderPayload::size * best_prices_order.entries() + BestPricesOrder::size;
                    break;
                }
                case MessageType::Hearthbeat: {}
                case MessageType::SequenceReset: {}
                case MessageType::EmptyBook: {}
                case MessageType::SecurityDefinition: {}
                case MessageType::SecurityStatus: {}
                case MessageType::SecurityDefinitionUpdateReport: {}
                case MessageType::TradingSessionStatus: {}
                case MessageType::Logon: {}
                case MessageType::Logout: {}
                case MessageType::MarketDataRequest: {
                    Parsers::skip(file, header.block_length);
                    size += header.block_length;
                    break;
                }
            }
        } else {
            throw BadMessageTypeException();
        }
        return size;
    }
};

std::ostream& operator<<(std::ostream& os, const SBEMessage& message) {
    os << "==== SBE Message ====\n";
    os << message.header << '\n';
    if (message.order_update) {
        os << message.order_update.value() << '\n';
    }
    if (message.order_execution) {
        os << message.order_execution.value() << '\n';
    }
    if (message.order_book_snapshot_packet) {
        os << message.order_book_snapshot_packet.value() << '\n';
    }
    os << "==== SBE Message end ====\n";
    return os;
}
