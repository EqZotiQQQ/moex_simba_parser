#pragma once

#include <cstdint>
#include <binary_parser/buffered_reader.h>
#include <vector>

struct BestPriceisOrderPayload {
    constexpr static std::size_t SIZE = 21;

    int64_t mkt_bid_px {};
    int64_t mkt_offer_px {};
    uint8_t bp_flags {};
    int32_t security_id {};

    explicit BestPriceisOrderPayload(BufferedReader& reader);

    std::string to_string() const;
    friend std::ostream& operator<<(std::ostream& os, const BestPriceisOrderPayload& header);
};

struct BestPricesOrder {
    constexpr static size_t SIZE = 3;

    uint16_t entry_size {};
    uint8_t no_md_entries {};
    std::vector<BestPriceisOrderPayload> md_entries;

    explicit BestPricesOrder(BufferedReader& reader):
            entry_size{reader.next<uint16_t>()},
            no_md_entries{reader.next<uint8_t>()} {
        for (int i = 0; i < no_md_entries; i++) {
            md_entries.emplace_back(BestPriceisOrderPayload{reader});
        }
    }

    size_t get_parsed_bytes() const {
        return SIZE + BestPriceisOrderPayload::SIZE * no_md_entries;
    }

    std::string to_string() const {
        return std::string{"TODO"};
    }

    friend std::ostream& operator<<(std::ostream& os, const BestPricesOrder& header) {
        os << header.to_string();
        return os;
    }
};