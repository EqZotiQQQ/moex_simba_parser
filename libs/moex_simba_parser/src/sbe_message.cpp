#include <c++/11/iostream>
#include "moex/sbe/sbe_message.h"

SbeMessageHeader::SbeMessageHeader(BufferedReader& reader) {
    block_length = reader.next<uint16_t>();
    template_ID = TemplateId{reader.next<uint16_t>()};
    schema_ID = reader.next<uint16_t>();
    version = reader.next<uint16_t>();
}

void SbeMessageHeader::parse(BufferedReader& reader) {

}

std::ostream &operator<<(std::ostream& os, const SbeMessageHeader& header) {
    os << fmt::format("\n{}\n", header.to_string());
    return os;
}

std::string SbeMessageHeader::to_string() const noexcept {
    return fmt::format(
            "block_length: {}\n"
            "template_ID: {}\n"
            "schema_ID: {}\n"
            "version: {}\n",
            block_length,
            template_ID.to_string(),
            schema_ID,
            version
    );
}

SbeMessageHeader::SbeMessageHeader() {

}

SbeMessage::SbeMessage(BufferedReader& reader):
        header{SbeMessageHeader{reader}} {
    parsed += SbeMessageHeader::SIZE;

    switch (header.template_ID.value) {
        case MessageTypeValue::OrderBestPrices: { order = BestPriceisOrder{reader}; break;}
        case MessageTypeValue::OrderUpdate: { order = Update{reader} ; break;}
        case MessageTypeValue::OrderExecution: { order = OrderExecution{reader}; break;}
        case MessageTypeValue::OrderBookSnapshotPacket: { order = OrderBookSnapshot{reader}; break;}
        case MessageTypeValue::Heartbeat:
        case MessageTypeValue::SequenceReset:
        case MessageTypeValue::EmptyBook:
        case MessageTypeValue::SecurityDefinition:
        case MessageTypeValue::SecurityStatus:
        case MessageTypeValue::SecurityDefinitionUpdateReport:
        case MessageTypeValue::TradingSessionStatus:
        case MessageTypeValue::Logon:
        case MessageTypeValue::Logout:
        case MessageTypeValue::MarketDataRequest:
            break;
        default: throw std::runtime_error("Unsupported sbe message type");
    }

    parsed += std::visit([this, &reader]<typename T>(T&& arg) -> uint32_t {
        if constexpr (!std::is_same_v<std::monostate, std::decay_t<T>>) {
            return arg.get_parsed_bytes();
        } else {
            reader.skip(header.block_length);
            return header.block_length;
        }
    }, order);
}

void SbeMessage::parse(BufferedReader &reader) {

}

std::string SbeMessage::to_string() const {
    std::string res;
    res = std::visit([]<typename T>(T&& o) -> std::string {
        if constexpr (!std::is_same_v<std::monostate, std::decay_t<T>>) {
            return o.to_string();
        } else {
            return "";
        }
    }, order);
    return res;
}

std::ostream &operator<<(std::ostream& os, const SbeMessage& header) {
    os << header.to_string();
    return os;
}
