#pragma once

#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <fmt/format.h>
#include <ctime>
#include <date/date.h>
#include "pcap_config.h"

#define INT64_NULL (-9223372036854775807LL - 1)
#define DECIMAL6_NULL (9223372036854775807)

enum class MDUpdateActionValue {
    NEW,
    CHANGE,
    DELETE
};

struct MDUpdateAction {
    MDUpdateActionValue value;

    MDUpdateAction() {}
    explicit MDUpdateAction(uint8_t action) {
        switch(action) {
            case 0: {value = MDUpdateActionValue::NEW; break;}
            case 1: {value = MDUpdateActionValue::CHANGE; break;}
            case 2: {value = MDUpdateActionValue::DELETE; break;}
            default: {throw std::runtime_error(fmt::format("Bad MD Update Action: {}", action));}
        }
    }

    std::string to_string() const {
        switch(value) {
            case MDUpdateActionValue::NEW: {return "New";}
            case MDUpdateActionValue::CHANGE: {return "Change";}
            case MDUpdateActionValue::DELETE: {return "Delete";}
            default: throw std::runtime_error("Bad Action type"); // mark unreachable
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const MDUpdateAction& header) {
        os << header.to_string();
        return os;
    }
};

enum class MDEntryTypeEnum {
    BID,
    ASK,
    EMPTY_BOOK
};

struct MDEntryType {
    MDEntryTypeEnum value;

    MDEntryType() {}
    explicit MDEntryType(uint8_t entry) {
        switch(entry) {
            case 48: {value = MDEntryTypeEnum::BID; break;}
            case 49: {value = MDEntryTypeEnum::ASK; break;}
            case 74: {value = MDEntryTypeEnum::EMPTY_BOOK; break;}
            default: {throw std::runtime_error(fmt::format("Bad MD Entry Action: {}", entry));}
        }
    }

    std::string to_string() const {
        switch(value) {
            case MDEntryTypeEnum::BID: {return "Bid";}
            case MDEntryTypeEnum::ASK: {return "Ask";}
            case MDEntryTypeEnum::EMPTY_BOOK: {return "Empty book";}
            default: throw std::runtime_error("Bad MD Entry Type");
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const MDEntryType& header) {
        os << header.to_string();
        return os;
    }
};

//nano -> ns 10^-9
inline int64_t sec_ns_to_ns(uint64_t sec, uint64_t ns) {
    return sec * 1'000'000'000 + ns;
}

//micro -> us 10^-6
inline int64_t sec_us_to_ns(uint64_t sec, uint64_t us) {
    return sec * 1'000'000 + us;
}

inline int64_t ns_to_sec(uint64_t ns) {
    return ns / 1'000'000'000;
}

inline int64_t floor_ns(uint64_t ns) {
    return ns % 1'000'000'000;
}

inline std::string to_human_readable_time_presc(uint32_t secondary_time, Time format) {
    if (format == Time::MICROSECONDS) {
        std::string s = std::to_string(1'000'000 + secondary_time);
        s[0] = '0';
        return s;
    } else {
        std::string s = std::to_string(1'000'000'000 + secondary_time);
        s[0] = '0';
        return s;
    }
}

inline std::string to_human_readable_time(uint64_t sec) {
    date::sys_time<std::chrono::seconds> st{round<std::chrono::seconds>(std::chrono::duration<int64_t>{sec})};
    std::string time {date::format("%F %T", st) };
    return time;
}

