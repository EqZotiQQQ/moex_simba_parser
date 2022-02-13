#pragma once

#include <vector>
#include <fstream>
#include "../types.h"
#include "../utils.h"
#include "../parsers.h"

struct BestPricesOrderHeader {
    i64 mkt_bid_px {};      // Лучшая цена на покупку
    i64 mkt_offer_px {};    // Лучшая цена на продажу
    u8 bp_flags {};         // Признак пустого стакана
    i32 security_id {};     // ID инструкмента
    constexpr static u8 size_bytes {21};
    static BestPricesOrderHeader parse(std::ifstream& file, Endian endian) {
        return BestPricesOrderHeader {
            .mkt_bid_px = Parsers::parse_i64(file, endian),
            .mkt_offer_px = Parsers::parse_i64(file, endian),
            .bp_flags = Parsers::parse_u8(file, endian),
            .security_id = Parsers::parse_i32(file, endian),
        };
    }
};

struct BestPricesOrder {
    u16 entry_size {};
    u8 no_md_entries {};
    std::vector<BestPricesOrderHeader> md_entries; // Записи

    constexpr static u8 size_bytes {3}; // Actual size is 1 + sizeof(BestPricesOrderHeader) * no_md_entries

    static BestPricesOrder parse(std::ifstream& file, Endian endian) {
        BestPricesOrder best_price_orders {
                .entry_size = Parsers::parse_u16(file, endian),
                .no_md_entries = Parsers::parse_u8(file, endian),
        };
        for (int i = 0; i < best_price_orders.no_md_entries; i++) {
            best_price_orders.md_entries.push_back(BestPricesOrderHeader::parse(file, endian));
        }
        return best_price_orders;
    }
};


