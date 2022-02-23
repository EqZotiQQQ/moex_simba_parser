#pragma once

#include <string>
#include <unordered_map>
#include "typenames.h"
#include "constants.h"

static std::unordered_map<u8, std::string> md_update_action {
        {0, "New"},
        {1, "Change"},
        {2, "Delete"},
};

static std::unordered_map<u8, std::string> md_entry_type {
        {'0', "Bid"},
        {'1', "Offer"},
        {'J', "EmptyBook"},
};

static std::unordered_map<u32, std::string> endian_type {
        {Constants::big_endian_milliseconds, "Big endian"},
        {Constants::little_endian_milliseconds, "Little endian"},
        {Constants::big_endian_nanoseconds, "Big endian"},
        {Constants::little_endian_nanoseconds, "Little endian"},
};

static std::unordered_map<u32, std::string> message_type {
        {1, "Hearthbeat"},
        {2, "SequenceReset"},
        {3, "BestPrices"},
        {4, "EmptyBook"},
        {5, "OrderUpdate"},
        {6, "OrderExecution"},
        {7, "OrderBookSnapshot"},
        {8, "SecurityDefinition"},
        {9, "SecurityStatus"},
        {10, "SecurityDefinitionUpdateReport"},
        {11, "TradingSessionStatus"},
        {1000, "Logon"},
        {1001, "Logout"},
        {1002, "MarketDataRequest"},
};

enum OutputFromat {
    console = 0,
    file = 1,
    disabled = 2,
};
