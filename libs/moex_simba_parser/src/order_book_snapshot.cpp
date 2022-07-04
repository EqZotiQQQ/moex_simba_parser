#include "moex/orders/order_book_snapshot.h"
#include "utility.h"

OrderBookSnapshot::OrderBookSnapshot(BufferedReader &reader):
    md_entry_id{reader.next<int64_t>()},
    transact_time{reader.next<uint64_t>()},
    md_entry_px{reader.next<int64_t>()},
    md_entry_size{reader.next<int64_t>()},
    trade_id{reader.next<int64_t>()},
    md_flags{reader.next<uint64_t>()},
    md_entry_type{reader.next<uint8_t>()} {}


void OrderBookSnapshot::parse(BufferedReader &reader) {

}

std::ostream &operator<<(std::ostream &os, const OrderBookSnapshot &header) {
    os << header.to_string();
    return os;
}

std::string OrderBookSnapshot::to_string() const {
    return fmt::format(
            "MD Entry ID: {}\n"
            "The start time of the event processing. UNIX time in nanoseconds, according to UTC: {}\n"
            "MD Entry px: {}\n"
            "MD Entry size: {}\n"
            "Trade id: {}\n"
            "Order or trade type: {}\n"
            "Record type: {}\n",
            md_entry_id == INT64_NULL ? "ID is Null" : std::to_string(md_entry_id),
            transact_time,
            md_entry_px == DECIMAL6_NULL ? "Price is Null" : std::to_string(md_entry_px),
            md_entry_size == INT64_NULL ? "Quantity is Null" : std::to_string(md_entry_size),
            trade_id == INT64_NULL ? "Trade ID is Null" : std::to_string(trade_id),
            md_flags.to_string(),
            md_entry_type.to_string()
    );
}
