#pragma once


#include <fstream>
#include <iostream>
#include "../../types.h"
#include "../../utils.h"
#include "../../parsers.h"
#include "../../messages/message_base.h"
#include "../../messages/order_update.h"
#include "../../pcap_header.h"
#include "../../messages/order_execution.h"
#include "../../messages/order_book_snapshot.h"

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
    u64 parsed_bytes = {};

    SBEMessage(std::ifstream& file, Endian endian) {
        header = SBEHeader::parse(file, endian);
        parsed_bytes += SBEHeader::size_bytes;
        switch (header.template_ID) {
            case MessageType::OrderUpdate: {
                std::cout << "Parsed MessageType::OrderUpdate" << '\n';
                order_update = OrderUpdate::parse(file, endian);
                parsed_bytes += OrderUpdate::size_bytes;
                break;
            }
            case MessageType::OrderExecution: {
                std::cout << "Parsed MessageType::OrderExecution" << '\n';
                order_execution = OrderExecution::parse(file, endian);
                parsed_bytes += OrderExecution::size_bytes;
                break;
            }
            case MessageType::OrderBookSnapshot: {
                std::cout << "Parsed MessageType::OrderBookSnapshot" << '\n';
                order_book_snapshot_packet = OrderBookSnapshotPacket::parse(file, endian);
                parsed_bytes += OrderBookSnapshot::size_bytes;
                break;
            }
            default: {
                std::cout << "Parsed default" << '\n';
                Parsers::skip(file, header.block_length);
                parsed_bytes += header.block_length;
                break;
            }
        }

        std::cout << header << '\n';
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

