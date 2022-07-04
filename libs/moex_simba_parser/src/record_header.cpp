#include "record_header.h"
#include <chrono>
#include <iomanip>

RecordHeader::RecordHeader(BufferedReader& reader) {
    parse(reader);
}

void RecordHeader::parse(BufferedReader& reader) {
    ts_ms = reader.next<uint32_t>();
    ts_ns = reader.next<uint32_t>();
    pack_length = reader.next<uint32_t>();
    real_length = reader.next<uint32_t>();
}

std::string RecordHeader::to_string() const {
    return fmt::format(
            "Timestamp ms: {}\n"
            "Timestamp ns: {}\n"
            "Packet length: {}\n"
            "Real length: {}\n",
            ts_ms,
            ts_ns,
            pack_length,
            real_length
    );
}

std::ostream& operator<<(std::ostream& os, const RecordHeader& header) {
    os << header.to_string();
    return os;
}