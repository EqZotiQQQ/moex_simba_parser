#pragma once

#include <cstdint>
#include <stdexcept>
#include "binary_parser/buffered_reader.h"

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

struct MessageType {
    MessageTypeValue value {};

    MessageType() {}

    explicit MessageType(uint32_t type) {
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
            default: {throw std::runtime_error("Unsupported sbe message type");}
        }
    }

    std::string to_string() const {
        std::string result;
        switch(value) {
            case MessageTypeValue::Heartbeat: { result = "MessageTypeValue::Heartbeat" ; break;}
            case MessageTypeValue::SequenceReset: { result = "MessageTypeValue::SequenceReset"; break;}
            case MessageTypeValue::OrderBestPrices: { result = "MessageTypeValue::OrderBestPrices"; break;}
            case MessageTypeValue::EmptyBook: { result = "MessageTypeValue::EmptyBook"; break;}
            case MessageTypeValue::OrderUpdate: { result = "MessageTypeValue::OrderUpdate"; break;}
            case MessageTypeValue::OrderExecution: { result = "MessageTypeValue::OrderExecution"; break;}
            case MessageTypeValue::OrderBookSnapshotPacket: { result = "MessageTypeValue::OrderBookSnapshotPacket"; break;}
            case MessageTypeValue::SecurityDefinition: { result = "MessageTypeValue::SecurityDefinition"; break;}
            case MessageTypeValue::SecurityStatus: { result = "MessageTypeValue::SecurityStatus"; break;}
            case MessageTypeValue::SecurityDefinitionUpdateReport: { result = "MessageTypeValue::SecurityDefinitionUpdateReport"; break;}
            case MessageTypeValue::TradingSessionStatus: { result = "MessageTypeValue::TradingSessionStatus"; break;}
            case MessageTypeValue::Logon: { result = "MessageTypeValue::Logon"; break;}
            case MessageTypeValue::Logout: { result = "MessageTypeValue::Logout"; break;}
            case MessageTypeValue::MarketDataRequest: { result = "MessageTypeValue::MarketDataRequest"; break;}
            default: {throw std::runtime_error("Unsupported sbe message type");}
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const MessageType& message) {
        os << message.to_string();
        return os;
    }
};

struct SbeHeader {
    uint16_t block_length {};
    MessageType template_id {};
    uint16_t schema_id {};
    uint16_t version {};

    SbeHeader(BufferedReader& reader) {
        block_length = reader.next<uint16_t>();
        template_id = MessageType{reader.next<uint16_t>()};
        schema_id = reader.next<uint16_t>();
        version = reader.next<uint16_t>();
    }

    std::string to_string() const {
        return fmt::format(
                "block_length: {}\n"
                "template_id: {}\n"
                "schema_id: {}\n"
                "version: {}",
                block_length,
                template_id.to_string(),
                schema_id,
                version);
    }

    friend std::ostream& operator<<(std::ostream& os, const SbeHeader& message) {
        os << message.to_string();
        return os;
    }
};
