#include "moex/orders/execution.h"

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

std::ostream &operator<<(std::ostream& os, const OrderExecution& header) {
    os << fmt::format(
            "md_entry_id: {}\n"
            "md_entry_px: {}\n"
            "md_entry_size: {}\n"
            "last_px: {}\n"
            "last_qty: {}\n"
            "trade_id: {}\n"
            "md_flags: {}\n"
            "security_id: {}\n"
            "rpt_seq: {}\n"
            "md_update_action: {}\n"
            "md_entry_type: {}\n",
            header.md_entry_id,
            header.md_entry_px,
            header.md_entry_size,
            header.last_px,
            header.last_qty,
            header.trade_id,
            header.md_flags,
            header.security_id,
            header.rpt_seq,
            header.md_update_action,
            header.md_entry_type
    );
    return os;
}
