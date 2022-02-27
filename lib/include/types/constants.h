#pragma once

#include "types/typenames.h"

#define INT64_NULL (-9223372036854775807LL - 1)
#define DECIMAL5_NULL (9223372036854775807)


namespace Constants {
    constexpr u32 big_endian_milliseconds = 0xA1B2C3D4;
    constexpr u32 big_endian_nanoseconds = 0xA1B23C4D;
    constexpr u32 little_endian_milliseconds = 0xD4C3B2A1;
    constexpr u32 little_endian_nanoseconds = 0x4D3CB2A1;
}

namespace MessageType {
    constexpr u8 Hearthbeat = 1;
    constexpr u8 SequenceReset = 2;
    constexpr u8 BestPrices = 3;
    constexpr u8 EmptyBook = 4;
    constexpr u8 OrderUpdate = 5;
    constexpr u8 OrderExecution = 6;
    constexpr u8 OrderBookSnapshot = 7;
    constexpr u8 SecurityDefinition = 8;
    constexpr u8 SecurityStatus = 9;
    constexpr u8 SecurityDefinitionUpdateReport = 10;
    constexpr u8 TradingSessionStatus = 11;
    constexpr u16 Logon = 1000;
    constexpr u16 Logout = 1001;
    constexpr u16 MarketDataRequest = 1002;
}

std::ostream& operator<<(std::ostream& os, std::endian endian) {
    os << "============================== Endian: =======================================\n";
    if (endian == std::endian::little) {
        os << "Little endian\n";
    } else {
        os << "Big endian\n";
    }
    os << "============================== Endian end =======================================\n";
    return os;
}

enum TimeStamp {
    milliseconds,
    nanoseconds,
};
