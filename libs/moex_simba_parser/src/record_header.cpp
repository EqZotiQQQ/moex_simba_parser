#include "record_header.h"
#include <chrono>
#include <iomanip>
#include "utility.h"

RecordHeader::RecordHeader(BufferedReader& reader, const PcapConfig& pcap_config):
        seconds{reader.next<uint32_t>()},
        secondary_time{reader.next<uint32_t>()},
        pack_length{reader.next<uint32_t>()},
        real_length{reader.next<uint32_t>()},
        time_format(pcap_config.time_format)
{
}

void RecordHeader::parse(BufferedReader& reader) {
}

std::string RecordHeader::to_string() const {
    return fmt::format(
            "Sending time: {}\n"
            "Timestamp Seconds: {}\n"
            "Timestamp {}(Microseconds or nanoseconds): {}\n"
            "Captured Packet Length: {}\n"
            "Original Packet Length: {}",
            to_human_readable_time(seconds, secondary_time),
            seconds,
            time_format == Time::MICROSECONDS ? "Microseconds" : "Nanoseconds",
            secondary_time,
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