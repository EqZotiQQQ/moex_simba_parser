#pragma once

#include "types/typenames.h"

#define INT64_NULL (-9223372036854775807LL - 1)
#define DECIMAL5_NULL (9223372036854775807)


namespace Constants {
    constexpr u32 big_endian_milliseconds = 3569595041; // D4C3B2A1
    constexpr u32 big_endian_nanoseconds = 1295823521;  // 4D3CB2A1
    constexpr u32 little_endian_milliseconds = 2712847316;    // A1B2C3D4
    constexpr u32 little_endian_nanoseconds = 2712812621;     // A1B23C4D
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

enum Endian {
    little_endian,
    big_endian,
};

std::ostream& operator<<(std::ostream& os, Endian endian) {
    os << "============================== Endian: =======================================\n";
    if (endian == Endian::little_endian) {
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
