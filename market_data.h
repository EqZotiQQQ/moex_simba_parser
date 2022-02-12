#pragma once

#include "types.h"

struct MarketDataPacketHeader { // Little endian
    u32 msg_seq_number {};          // Счётчик. ++ когда отправляет сообщение. Ресет раз в сутки
    u16 msg_size {};                // Длина сообщения в байтах
    u16 msg_flags {};               // Флаги. Лучше читать в доке про флаги
    u64 sending_time {};            // Время отправки сообщения шлюзом
};

struct IncreamentalPacketHeader { // Little endian
    u64 transact_time {};               // Начало процесинга транзакции в матчинге с момента начала эпохи
    u32 exchange_trading_session_ID {}; // ID торговой сессии
};

struct SBEHeader {
    u16 block_length {};  // Длина корневой части сообщения, кроме самой SBE Header и NoMDEntries
    u16 template_ID {}; // ID сообщения
    u16 schema_ID {};   // ID схемы сообщения
    u16 version {};     // Версия схемы
};