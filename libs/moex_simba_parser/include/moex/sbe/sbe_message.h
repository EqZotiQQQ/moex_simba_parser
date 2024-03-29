#pragma once

#include <variant>
#include <iostream>
#include <binary_parser/buffered_reader.h>

#include "moex/orders/best_prices_order.h"
#include "moex/orders/order_execution.h"
#include "moex/orders/order_book_snapshot.h"
#include "moex/orders/order_update.h"

enum class MessageTypeValue {
    Heartbeat = 1,
    SequenceReset = 2,
    OrderBestPrices = 3,
    EmptyBook = 4,
    OrderUpdate = 5,
    OrderExecution = 6,
    OrderBookSnapshotPacket = 7,
    SecurityDefinition = 8,
    SecurityStatus = 9,
    SecurityDefinitionUpdateReport = 10,
    TradingSessionStatus = 11,
    Logon = 1000,
    Logout = 1001,
    MarketDataRequest = 1002,
};

struct TemplateId {
    MessageTypeValue value;

    TemplateId() {}

    explicit TemplateId(uint32_t type) {
        switch(type) {
            case 1: { value = MessageTypeValue::Heartbeat; break;}
            case 2: { value = MessageTypeValue::SequenceReset; break;}
            case 3: { value = MessageTypeValue::OrderBestPrices; break;}
            case 4: { value = MessageTypeValue::EmptyBook; break;}
            case 5: { value = MessageTypeValue::OrderUpdate; break;}
            case 6: { value = MessageTypeValue::OrderExecution; break;}
            case 7: { value = MessageTypeValue::OrderBookSnapshotPacket; break;}
            case 8: { value = MessageTypeValue::SecurityDefinition; break;}
            case 9: { value = MessageTypeValue::SecurityStatus; break;}
            case 10: { value = MessageTypeValue::SecurityDefinitionUpdateReport; break;}
            case 11: { value = MessageTypeValue::TradingSessionStatus; break;}
            case 1000: { value = MessageTypeValue::Logon; break;}
            case 1001: { value = MessageTypeValue::Logout; break;}
            case 1002: { value = MessageTypeValue::MarketDataRequest; break;}
            default: {throw std::runtime_error("Bad message type");}
        }
    }

    std::string to_string() const {
        switch(value) {
            case MessageTypeValue::Heartbeat: return "Heartbeat";
            case MessageTypeValue::SequenceReset: return "SequenceReset";
            case MessageTypeValue::OrderBestPrices: return "OrderBestPrices";
            case MessageTypeValue::EmptyBook: return "EmptyBook";
            case MessageTypeValue::OrderUpdate: return "OrderUpdate";
            case MessageTypeValue::OrderExecution: return "OrderExecution";
            case MessageTypeValue::OrderBookSnapshotPacket: return "OrderBookSnapshotPacket";
            case MessageTypeValue::SecurityDefinition: return "SecurityDefinition";
            case MessageTypeValue::SecurityStatus: return "SecurityStatus";
            case MessageTypeValue::SecurityDefinitionUpdateReport: return "SecurityDefinitionUpdateReport";
            case MessageTypeValue::TradingSessionStatus: return "TradingSessionStatus";
            case MessageTypeValue::Logon: return "Logon";
            case MessageTypeValue::Logout: return "Logout";
            case MessageTypeValue::MarketDataRequest: return "MarketDataRequest";
            default: throw std::runtime_error("Unsupported sbe message type");
        }
    }


    friend std::ostream& operator<<(std::ostream& os, const TemplateId& message) {
        os << message.to_string();
        return os;
    }
};

struct SbeMessageHeader {
    SbeMessageHeader();

    constexpr static std::size_t SIZE = 8;

    uint16_t block_length {};
    TemplateId template_ID {};
    uint16_t schema_ID {};
    uint16_t version {};

    explicit SbeMessageHeader(buffered_reader::BufferedReader& reader);

    std::string to_string() const noexcept;

    friend std::ostream& operator<<(std::ostream& os, const SbeMessageHeader& header);
};

struct SbeMessage {
    constexpr static std::size_t SIZE = 8;
    using OrderType = std::variant<std::monostate, OrderUpdate, OrderExecution, OrderBookSnapshotPacket, BestPricesOrder>;

    SbeMessageHeader header {};
    OrderType order;

    size_t parsed {};

    explicit SbeMessage(buffered_reader::BufferedReader& reader);

    std::string to_string() const;
    size_t get_parsed_bytes() const { return parsed; }

    friend std::ostream& operator<<(std::ostream& os, const SbeMessage& header);
};