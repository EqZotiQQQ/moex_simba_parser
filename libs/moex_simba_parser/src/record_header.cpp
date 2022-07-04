#include "record_header.h"
#include <chrono>
#include <iomanip>
#include "utility.h"

RecordHeader::RecordHeader(BufferedReader& reader, bool is_ns):
        ts_ms{reader.next<uint32_t>()},
        ts_ns{reader.next<uint32_t>()},
        pack_length{reader.next<uint32_t>()},
        real_length{reader.next<uint32_t>()},
        is_ns(is_ns)
{
}

void RecordHeader::parse(BufferedReader& reader) {
}

std::string RecordHeader::to_string() const {
    return fmt::format(
            "Sending time: {}\n"
            "Timestamp ms: {}\n"
            "Timestamp ns: {}\n"
            "Packet length: {}\n"
            "Real length: {}",
            to_human_readable_time(ts_ms, ts_ns, is_ns),
            ts_ms,
            ts_ns,
            pack_length,
            real_length
    );
}

std::ostream& operator<<(std::ostream& os, const RecordHeader& header) {
    os << "\n<Record Header>\n";
    os << header.to_string();
    os << "\n</Record Header>\n";
    return os;
}