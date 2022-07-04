#include "moex/orders/update.h"

Update::Update(BufferedReader& reader):
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

void Update::parse(BufferedReader& reader) {

}

std::ostream &operator<<(std::ostream& os, const Update& header) {
    os << fmt::format(
            "Entry ID: {}\n"
            "Entry price: {}\n"
            "Entry size: {}\n"
            "Flags: {}\n"
            "Security ID: {}\n"
            "RQT SEQ: {}\n"
            "MD Update action: {}\n"
            "MD Entry type: {}\n",
            header.md_entry_id,
            header.md_entry_px,
            header.md_entry_size,
            header.md_flags,
            header.security_id,
            header.rpt_seq,
            header.md_update_action,
            header.md_entry_type
    );
    return os;
}
