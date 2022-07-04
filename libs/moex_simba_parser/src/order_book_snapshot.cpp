#include "moex/orders/order_book_snapshot.h"

OrderBookSnapshot::OrderBookSnapshot(BufferedReader &reader) {
    md_entry_id = reader.next<int64_t>();
    transact_time = reader.next<uint64_t>();
    md_entry_px = reader.next<int64_t>();
    md_entry_size = reader.next<int64_t>();
    trade_id = reader.next<int64_t>();
    md_flags = reader.next<uint64_t>();
    md_entry_type = reader.next<uint8_t>();
}

void OrderBookSnapshot::parse(BufferedReader &reader) {

}

std::ostream &operator<<(std::ostream &os, const OrderBookSnapshot &header) {
    os << header.to_string();
    return os;
}

//std::string OrderBookSnapshot::to_string() const {
//    return fmt::format(
//            "MD Entry ID: {}\n"
//            "Transact time: {}\n"
//            "MD Entry px: {}\n"
//            "MD Entry size: {}\n"
//            "Trade id: {}\n"
//            "MD Flags: {}\n"
//            "MD Entry type: {}\n",
//            md_entry_id,
//            transact_time,
//            md_entry_px,
//            md_entry_size,
//            trade_id,
//            md_flags,
//            md_entry_type
//    );
//}
