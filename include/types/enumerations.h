#pragma once

#include <string>
#include <unordered_map>
#include "typenames.h"

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
        {3569595041, "Big endian"},
        {2712847316, "Little endian"},
        {1295823521, "Big endian"},
        {2712812621, "Little endian"},
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
