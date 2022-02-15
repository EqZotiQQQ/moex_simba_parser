#pragma once

#include <vector>
#include <fstream>
#include "types/typenames.h"
#include "utils/parsers.h"

class BestPricesOrderPayload {
private:
    i64 mkt_bid_px {};      // Лучшая цена на покупку
    i64 mkt_offer_px {};    // Лучшая цена на продажу
    u8 bp_flags {};         // Признак пустого стакана
    i32 security_id {};     // ID инструкмента
public:
    constexpr static u8 size {21};
public:
    void parse(std::ifstream& file, Endian endian) {
        mkt_bid_px = Parsers::parse_i64(file, endian);
        mkt_offer_px = Parsers::parse_i64(file, endian);
        bp_flags = Parsers::parse_u8(file);
        security_id = Parsers::parse_i32(file, endian);
    }
};

class BestPricesOrder {
    u16 entry_size {};
    u8 no_md_entries {};
    std::vector<BestPricesOrderPayload> md_entries; // Записи
public:
    constexpr static u8 size {3}; // Actual size is 1 + sizeof(BestPricesOrderHeader) * no_md_entries
public:
    void parse(std::ifstream& file, Endian endian) {
        entry_size = Parsers::parse_u16(file, endian);
        no_md_entries = Parsers::parse_u8(file);
        for (int i = 0; i < no_md_entries; i++) {
            BestPricesOrderPayload payload;
            payload.parse(file, endian);
            md_entries.emplace_back(payload);
        }
    }

    u64 entries() const noexcept {
        return no_md_entries;
    }
};


