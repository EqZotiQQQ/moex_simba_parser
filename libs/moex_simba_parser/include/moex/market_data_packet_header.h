#pragma once

#include "binary_parser/buffered_reader.h"
#include "packets/incremental_packet.h"
#include "packets/snapshot_packet.h"

enum class MessageFlagsImpl {
    MESSAGE_FRAGMENTATION = 0x1,
    FIRST_MESSAGE = 0x2,
    LAST_MESSAGE = 0x4,
    INCREMENTAL_MESSAGE = 0x8,
    POS_DUP_FLAG = 0x10,
};

struct MessageFlags {
    uint16_t value;

    explicit MessageFlags(uint16_t flags):
        value(flags) {}

    bool is_flag_set(MessageFlagsImpl flag) const noexcept {
        return (value & static_cast<uint8_t>(flag)) == static_cast<uint8_t>(flag);
    }

    bool is_incremental_message() const noexcept {
        return is_flag_set(MessageFlagsImpl::INCREMENTAL_MESSAGE);
    }

    std::string to_string() const {
        std::string result;

        if (is_flag_set(MessageFlagsImpl::MESSAGE_FRAGMENTATION)) {
            result += "* Message fragmentation\n";
        } else {
            result += "* No Message fragmentation\n";
        }
        if (is_flag_set(MessageFlagsImpl::FIRST_MESSAGE)) {
            result += "* First message\n";
        }
        if (is_flag_set(MessageFlagsImpl::LAST_MESSAGE)) {
            result += "* Last message\n";
        }
        if (is_flag_set(MessageFlagsImpl::INCREMENTAL_MESSAGE)) {
            result += "* Incremental packet\n";
        } else {
            result += "* Snapshot packet\n";
        }
        if (is_flag_set(MessageFlagsImpl::POS_DUP_FLAG)) {
            result += "* Pos dup flag\n";
        } else {
            result += "* No pos dup flag\n";
        }
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const MessageFlags& message) {
        os << "<Message Flags>\n";
        os << message.to_string();
        os << "</Message Flags>\n";
        return os;
    }
};

struct MarketDataPacketHeader {
    uint32_t msg_seq_number;
    uint32_t msg_size;
    MessageFlags flags;
    uint64_t sending_time;

    explicit MarketDataPacketHeader(buffered_reader::BufferedReader& reader);

    bool is_incremental() const noexcept {
        return flags.is_incremental_message();
    }

    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& os, const MarketDataPacketHeader& header);
};
