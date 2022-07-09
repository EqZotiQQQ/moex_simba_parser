#include "moex/sbe/sbe_message.h"
#include <fmt/format.h>

SbeMessageHeader::SbeMessageHeader(BufferedReader& reader) {
    block_length = reader.next<uint16_t>();
    template_ID = TemplateId{reader.next<uint16_t>()};
    schema_ID = reader.next<uint16_t>();
    version = reader.next<uint16_t>();
}

std::ostream &operator<<(std::ostream& os, const SbeMessageHeader& header) {
    os << "<SBE message header>\n";
    os << fmt::format("\n{}\n", header.to_string());
    os << "</SBE message header>\n";
    return os;
}

std::string SbeMessageHeader::to_string() const noexcept {
    return fmt::format(
            "Block length: {}\n"
            "Template ID: {}\n"
            "Schema ID: {}\n"
            "Version: {}\n",
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
        case MessageTypeValue::OrderBestPrices: { order = BestPricesOrder{reader}; break;}
        case MessageTypeValue::OrderUpdate: { order = OrderUpdate{reader} ; break;}
        case MessageTypeValue::OrderExecution: { order = OrderExecution{reader}; break;}
        case MessageTypeValue::OrderBookSnapshotPacket: { order = OrderBookSnapshotPacket{reader}; break;}
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

std::string SbeMessage::to_string() const {
    std::string res;
    res += header.to_string() + '\n';
    res += std::visit([]<typename T>(T&& o) -> std::string {
        if constexpr (!std::is_same_v<std::monostate, std::decay_t<T>>) {
            return o.to_string();
        } else {
            return "";
        }
    }, order);
    return res;
}

std::ostream &operator<<(std::ostream& os, const SbeMessage& message) {
    os << message.to_string();
    return os;
}
