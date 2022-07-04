#include "moex/orders/order_execution.h"

OrderExecution::OrderExecution(BufferedReader& reader):
        md_entry_id{reader.next<int64_t>()},
        md_entry_px{reader.next<int64_t>()},
        md_entry_size{reader.next<int64_t>()},
        last_px{reader.next<int64_t>()},
        last_qty{reader.next<int64_t>()},
        trade_id{reader.next<int64_t>()},
        md_flags{reader.next<int64_t>()},
        security_id{reader.next<int32_t>()},
        rpt_seq{reader.next<uint32_t>()},
        md_update_action{reader.next<uint8_t>()},
        md_entry_type{reader.next<uint8_t>()} {

}

void OrderExecution::parse(BufferedReader& reader) {

}

std::string OrderExecution::to_string() const {
    return fmt::format(
            "Order ID: {}\n"
            "Order Price: {}\n"
            "Remaining quantity in the order: {}\n"
            "Trade price: {}\n"
            "Trade volume: {}\n"
            "Trade ID: {}\n"
            "Trade type (Flags): {}\n"
            "Instrument numeric code: {}\n"
            "Incremental refresh sequence number: {}\n"
            "Incremental refresh type: {}\n"
            "Record type: {}\n",
            md_entry_id,
            md_entry_px,
            md_entry_size,
            last_px,
            last_qty,
            trade_id,
            md_flags,
            security_id,
            rpt_seq,
            md_update_action.to_string(),
            md_entry_type.to_string()
    );
}

std::ostream& operator<<(std::ostream& os, const OrderExecution& order) {
    os << order.to_string();
    return os;
}
