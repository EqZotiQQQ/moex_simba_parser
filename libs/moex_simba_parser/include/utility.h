#pragma once

#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <fmt/format.h>
#include <ctime>

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
inline auto sec_ns_to_ns(uint64_t sec, uint64_t ns) {
    return sec * 1'000'000'000 + ns;
}

//micro -> us 10^-6
inline auto sec_us_to_ns(uint64_t sec, uint64_t us) {
    return sec * 1'000'000 + us;
}

inline std::string to_human_readable_time(uint64_t sec, uint64_t /*us*/, bool /*is_ns*/) {
    std::time_t t = sec;
    // TODO make it more clear and increase precision
    return std::asctime(std::localtime(&t));
}
