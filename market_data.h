#pragma once

#include "types.h"

struct MarketDataPacketHeader {
    u32 msg_seq_number {};
    u16 msg_size {};
    u16 msg_flags {};
    u64 sending_time {};
};

struct IncreamentalPacketHeader {

};

