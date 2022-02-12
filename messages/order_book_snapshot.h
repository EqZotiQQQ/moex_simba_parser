#pragma once

#include <vector>
#include "../types.h"
#include "message_base.h"


struct OrderBookSnapshot : public MessageBase {
    i64 md_entry_id;
    u64 transact_time;
    i64 md_entry_px;
    i64 md_entry_size;
    i64 trade_id;
    u64 md_flags_set;
    u8 md_entry_type;

    static constexpr u8 size_bytes = 49;

    static OrderBookSnapshot parse(std::ifstream& file, Endian endian) {
        return OrderBookSnapshot {
                .md_entry_id = Parsers::parse_i64(file, endian),
                .transact_time = Parsers::parse_u64(file, endian),
                .md_entry_px = Parsers::parse_i64(file, endian),
                .md_entry_size = Parsers::parse_i64(file, endian),
                .trade_id = Parsers::parse_i64(file, endian),
                .md_flags_set = Parsers::parse_u64(file, endian),
                .md_entry_type = Parsers::parse_u8(file, endian),
        };
    }
};

struct OrderBookSnapshotPacket : public MessageBase {
    int32_t security_id;                            //  Numeric identifier tool 32 bit
    uint32_t last_msg_seq_num_processed;            //  Number of MsgSeqNum 32 bit
    uint32_t rpt_seq;                               //  Number of RptSeq 32 bit
    uint32_t exchange_trading_session_id;           //  Trading session id 32 bit
    uint16_t block_len;                             //  Block length of group 16 bit
    uint8_t group_size;                             //  Group size 8 bit
    std::vector<OrderBookSnapshot> bids_slice;   //  All snapshot records records.size() * 49 bits


    static OrderBookSnapshotPacket parse(std::ifstream& file, Endian endian) {
        OrderBookSnapshotPacket order_book_snapshot {
                .security_id = Parsers::parse_i32(file, endian),
                .last_msg_seq_num_processed = Parsers::parse_u32(file, endian),
                .rpt_seq = Parsers::parse_u32(file, endian),
                .exchange_trading_session_id = Parsers::parse_u32(file, endian),
                .block_len = Parsers::parse_u16(file, endian),
                .group_size = Parsers::parse_u8(file, endian),
        };
        for (int i = 0; i < order_book_snapshot.group_size; i++) {
            order_book_snapshot.bids_slice.push_back(OrderBookSnapshot::parse(file, endian));
        }
        return order_book_snapshot;
    }
};

