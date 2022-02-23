#pragma once

#include <vector>
#include <fstream>
#include "types/typenames.h"
#include "utils/buffered_reader.h"

class BestPricesOrderPayload {
private:
    i64 mkt_bid_px {};      // Лучшая цена на покупку
    i64 mkt_offer_px {};    // Лучшая цена на продажу
    u8 bp_flags {};         // Признак пустого стакана
    i32 security_id {};     // ID инструкмента
public:
    constexpr static u8 size {21};
public:
    void parse(BufferedReader& parser) {
        mkt_bid_px = parser.next<i64>();
        mkt_offer_px = parser.next<i64>();
        bp_flags = parser.next<u8>();
        security_id = parser.next<i32>();
    }
};

class BestPricesOrder {
    u16 entry_size {};
    u8 no_md_entries {};
    std::vector<BestPricesOrderPayload> md_entries; // Записи
public:
    constexpr static u8 size {3}; // Actual size is 1 + sizeof(BestPricesOrderHeader) * no_md_entries
public:
    void parse(BufferedReader& parser) {
        entry_size = parser.next<u16>();
        no_md_entries = parser.next<u8>();
        for (int i = 0; i < no_md_entries; i++) {
            BestPricesOrderPayload payload;
            payload.parse(parser);
            md_entries.emplace_back(payload);
        }
    }

    u64 entries() const noexcept {
        return no_md_entries;
    }
};


