#pragma once


#include <fstream>
#include <iostream>
#include <unordered_map>
#include <optional>
#include "types/typenames.h"
#include "utils/buffered_reader.h"
#include "messages/order_update.h"
#include "global_pcap_parser.h"
#include "messages/order_execution.h"
#include "messages/order_book_snapshot.h"
#include "messages/order_best_prices.h"
#include "exceptions.h"

const char* get_message_type(u16 type) {
    switch (type) {
        case 1: return "Hearthbeat";
        case 2: return "SequenceReset";
        case 3: return "BestPrices";
        case 4: return "EmptyBook";
        case 5: return "OrderUpdate";
        case 6: return "OrderExecution";
        case 7: return "OrderBookSnapshot";
        case 8: return "SecurityDefinition";
        case 9: return "SecurityStatus";
        case 10: return "SecurityDefinitionUpdateReport";
        case 11: return "TradingSessionStatus";
        case 1000: return "Logon";
        case 1001: return "Logout";
        case 1002: return "MarketDataRequest";
        default: throw BadMessageTypeException();
    }
}

class SBEHeader {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const SBEHeader& header);
private:
    u16 block_length {};  // Длина корневой части сообщения, кроме самой SBE Header и NoMDEntries
    u16 template_ID {}; // ID сообщения
    u16 schema_ID {};   // ID схемы сообщения
    u16 version {};     // Версия схемы
public:
    constexpr static u8 size_bytes {8};
public:
    void parse(BufferedReader& parser) {
        block_length = parser.next<u16>();
        template_ID = parser.next<u16>();
        schema_ID = parser.next<u16>();
        version = parser.next<u16>();
    }
    u16 get_template_id() const noexcept {
        return template_ID;
    }
    u16 get_length() const noexcept {
        return block_length;
    }
};

class SBEMessage {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const SBEMessage& message);
private:
    SBEHeader header {};
    std::optional<OrderUpdate> order_update;
    std::optional<OrderExecution> order_execution;
    std::optional<OrderBookSnapshotPacket> order_book_snapshot_packet;
    u64 size = {};

public:
    SBEMessage() {

    }

    u64 parse(BufferedReader& parser) {
        header.parse(parser);
        size += SBEHeader::size_bytes;
        if (get_message_type(header.get_template_id())) {
            switch (header.get_template_id()) {
                case MessageType::OrderUpdate: {
                    order_update = OrderUpdate();
                    order_update->parse(parser);
                    size += OrderUpdate::size_bytes;
                    break;
                }
                case MessageType::OrderExecution: {
                    order_execution = OrderExecution {};
                    order_execution->parse(parser);
                    size += OrderExecution::size_bytes;
                    break;
                }
                case MessageType::OrderBookSnapshot: {
                    order_book_snapshot_packet = OrderBookSnapshotPacket {};
                    order_book_snapshot_packet->parse(parser);
                    size += OrderBookSnapshot::size * order_book_snapshot_packet->entries() + OrderBookSnapshotPacket::size;
                    break;
                }
                case MessageType::BestPrices: {
                    BestPricesOrder best_prices_order = BestPricesOrder {};
                    best_prices_order.parse(parser);
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
                    parser.skip(header.get_length());
                    size += header.get_length();
                    break;
                }
            }
        } else {
            throw BadMessageTypeException(); // so we throw an exception at the end of file :)
        }
        return size;
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const SBEHeader& header) {
    os << "== SBEHeader ==\n";
    os << std::dec;
    os << "Длина блока: "       << static_cast<u32>(header.block_length) << '\n';
    os << "Template ID: "       << header.template_ID << " (" << get_message_type(header.template_ID) << ")\n";
    os << "Schema ID: "         << static_cast<u16>(header.schema_ID) << '\n';
    os << "Version: "           << static_cast<u64>(header.version) << '\n';
    os << "== SBEHeader end ==\n";
    return os;
}

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const SBEMessage& message) {
    os << "== SBE Message ==\n";
    os << message.header << std::endl;
    if (message.order_update) {
        os << message.order_update.value() << std::endl;
    }
    if (message.order_execution) {
        os << message.order_execution.value() << std::endl;
    }
    if (message.order_book_snapshot_packet) {
        os << message.order_book_snapshot_packet.value() << std::endl;
    }
    os << "== SBE Message end ==\n";
    return os;
}

