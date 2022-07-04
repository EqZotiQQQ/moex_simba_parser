#pragma once

#include <cstdint>
#include <binary_parser/buffered_reader.h>
#include <vector>

struct BestPriceisOrderPayload {
    constexpr static std::size_t size = 21;

    int64_t mkt_bid_px {};
    int64_t mkt_offer_px {};
    uint8_t bp_flags {};
    int32_t security_id {};

    explicit BestPriceisOrderPayload(BufferedReader& reader);
    void parse(BufferedReader& reader);

    friend std::ostream& operator<<(std::ostream& os, const BestPriceisOrderPayload& header);
};

struct BestPriceisOrder {
    constexpr static uint8_t size = 3;

    uint16_t entry_size {};
    uint8_t no_md_entries {};
    std::vector<BestPriceisOrderPayload> md_entries;

    explicit BestPriceisOrder(BufferedReader& reader);
    void parse(BufferedReader& reader);

    size_t get_parsed_bytes() const {return 0;}
    std::string to_string() const {return std::string{"TODO"};}

    friend std::ostream& operator<<(std::ostream& os, const BestPriceisOrder& header);
};