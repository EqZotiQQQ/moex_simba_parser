#include "moex/orders/best_prices_order.h"


BestPriceisOrderPayload::BestPriceisOrderPayload(buffered_reader::BufferedReader& reader):
    mkt_bid_px{reader.next<int64_t>()},
    mkt_offer_px{reader.next<int64_t>()},
    bp_flags{reader.next<uint8_t>()},
    security_id{reader.next<int32_t>()}
{}

std::ostream& operator<<(std::ostream& os, const BestPriceisOrderPayload& best_price_payload) {
    os << best_price_payload.to_string();
    return os;
}

std::string BestPriceisOrderPayload::to_string() const {
    return fmt::format(
            "mkt_bid_px: {}\n"
            "mkt_offer_px: {}\n"
            "bp_flags: {}\n"
            "security_id: {}\n",
            mkt_bid_px,
            mkt_offer_px,
            bp_flags,
            security_id
            );
}

