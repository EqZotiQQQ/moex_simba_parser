#pragma once

#include <cstdint>
#include <vector>
#include <binary_parser/buffered_reader.h>
#include <iostream>

struct MdFlags {
    uint64_t value;

    explicit MdFlags(uint64_t value): value(value) {}

    static constexpr uint8_t day = 0x1;
    static constexpr uint8_t ioc = 0x2;

    std::string to_string() const {
        std::string ret;
        ret += "Типы сделок: "; // -> битовая маска (TODO) "<< order.md_flags << '\n';
        if ((value & day) == day) {
            ret += "Котировочная сделка (Day)\n";
        }
        if ((value & ioc) == ioc) {
            ret += "Встречная сделка (IOC)\n";
        }
        return ret;
    }
};

struct EntryType {
    uint8_t value;

    explicit EntryType(uint64_t value): value(value) {}

    static constexpr uint8_t bid = '0';
    static constexpr uint8_t ask = '1';
    static constexpr uint8_t empty_book = 'J';

    std::string to_string() const {
        std::string ret;
        ret += "Entry type ";
        if ((value & bid) == bid) {
            ret += "Bid\n";
        } else if ((value & ask) == ask) {
            ret += "Ask\n";
        } else if ((value & empty_book) == empty_book) {
            ret += "Empty book\n";
        } else {
            throw std::runtime_error("Bad entry type");
        }
        return ret;
    }
};

struct OrderBookSnapshot {
    int64_t md_entry_id;
    uint64_t transact_time;
    int64_t md_entry_px;
    int64_t md_entry_size;
    int64_t trade_id;
    MdFlags md_flags;
    EntryType md_entry_type;

    static constexpr size_t SIZE = 49;

    explicit OrderBookSnapshot(BufferedReader& reader);
    void parse(BufferedReader& reader);

    static size_t get_parsed_bytes() {return SIZE;}
    std::string to_string() const;

    friend std::ostream& operator<<(std::ostream& os, const OrderBookSnapshot& snapshot);
};

struct OrderBookSnapshotPacket {
    int32_t  security_id;
    uint32_t last_msg_seq_num_processed;
    uint32_t rpt_seq;
    uint32_t exchange_trading_session_id;
    uint16_t block_len;
    uint8_t  no_md_entries;
    std::vector<OrderBookSnapshot> md_entries;

    static constexpr size_t SIZE = 19;

    explicit OrderBookSnapshotPacket(BufferedReader& reader):
            security_id{reader.next<int32_t>()},
            last_msg_seq_num_processed{reader.next<uint32_t>()},
            rpt_seq{reader.next<uint32_t>()},
            exchange_trading_session_id{reader.next<uint32_t>()},
            block_len{reader.next<uint16_t>()},
            no_md_entries{reader.next<uint8_t>()} {
        for (int i = 0; i < no_md_entries; i++) {
            md_entries.emplace_back(OrderBookSnapshot{reader});
        }
    }

    size_t get_parsed_bytes() const {
        return SIZE + no_md_entries * block_len;
    }

    std::string to_string() const {
        std::string result_string;
        result_string += fmt::format(
                "Instrument numeric code: {}\n"
                "The 'MsgSeqNum' of the last message sent into incremental feed at the time of the current snapshot generation: {}\n"
                "The 'RptSeq' number of the last incremental update included in the current market data snapshot for instrument: {}\n"
                "Trading session ID: {}\n"
                "Block length: {}\n"
                "Number of 'MDEntry' records in the current message: {}",
                security_id,
                last_msg_seq_num_processed,
                rpt_seq,
                exchange_trading_session_id,
                block_len,
                no_md_entries);

        for (int i = 0; i < no_md_entries; i++) {
            result_string += "MD Entry: " + std::to_string(i) + '\n';
            result_string += md_entries[i].to_string() + '\n';
        }
        return result_string;
    }

    friend std::ostream& operator<<(std::ostream& os, const OrderBookSnapshotPacket& snapshot_packet) {
        os << snapshot_packet.to_string();
        return os;
    }
};