#include "moex/orders/order_update.h"

OrderUpdate::OrderUpdate(BufferedReader& reader):
    md_entry_id{reader.next<int64_t>()},
    md_entry_px{reader.next<int64_t>()},
    md_entry_size{reader.next<int64_t>()},
    md_flags{reader.next<uint64_t>()},
    security_id{reader.next<int32_t>()},
    rpt_seq{reader.next<uint32_t>()},
    md_update_action{reader.next<uint8_t>()},
    md_entry_type{reader.next<uint8_t>()}
{
}

std::ostream &operator<<(std::ostream& os, const OrderUpdate& order_update) {
    os << order_update.to_string();
    return os;
}
