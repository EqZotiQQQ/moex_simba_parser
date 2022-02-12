#pragma once

#include "types.h"

namespace Constants {
    constexpr u32 little_endian_milliseconds = 3569595041;
    constexpr u32 big_endian_milliseconds = 2712847316;
    constexpr u32 little_endian_nanoseconds = 1295823521;
    constexpr u32 big_endian_nanoseconds = 2712812621;
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