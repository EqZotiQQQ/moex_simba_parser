#pragma once

#include <fstream>
#include "message_base.h"
#include "../types.h"
#include "../utils.h"
#include "../parsers.h"

struct OrderExecution : public MessageBase {
    i64 md_entry_id;    // Идентификатор заявки
    i64 md_entry_px;    // Цена заявки
    i64 md_entry_size;  // Оставшееся количество в заявке
    i64 last_px;        // Цена сделки
    i64 last_qty;       // Объем сделки
    i64 trade_id;       // Идентификатор сделки
    i64 md_flags_set;   // Типы сделок -> битовая маска
    i32 security_id;    // Числовой идентификатор инструмента
    u32 rpt_seq;        // Порядковый номер инкрементального обновления
    u8 md_update_action;// Тип инкрементального обновления 1 - change; 2 - delete
    u8 md_entry_type;   // Тип заявки - 2 - продажа или 1 - покупка

    static constexpr u8 size_bytes = 66;

    static OrderExecution parse(std::ifstream& file, Endian endian) {
        return OrderExecution {
                .md_entry_id = Parsers::parse_i64(file, endian),
                .md_entry_px = Parsers::parse_i64(file, endian),
                .md_entry_size = Parsers::parse_i64(file, endian),
                .last_px = Parsers::parse_i64(file, endian),
                .last_qty = Parsers::parse_i64(file, endian),
                .trade_id = Parsers::parse_i64(file, endian),
                .md_flags_set = Parsers::parse_i64(file, endian),
                .security_id = Parsers::parse_i32(file, endian),
                .rpt_seq = Parsers::parse_u32(file, endian),
                .md_update_action = Parsers::parse_u8(file, endian),
                .md_entry_type = Parsers::parse_u8(file, endian),
        };
    }
};

std::ostream& operator<<(std::ostream& os, const OrderExecution& order) {
    os << "====================  OrderExecution packet: ===================\n";
    os << std::dec;
    os << "ID заявки "<< order.md_entry_id << '\n';
    os << "Цена заявки "<< order.md_entry_px << '\n';
    os << "Оставшееся количество в заявке "<< order.md_entry_size << '\n';
    os << "Цена сделки "<< order.last_px << '\n';
    os << "Объем сделки "<< order.last_qty << '\n';
    os << "Идентификатор сделки "<< order.trade_id << '\n';
    os << "Типы сделок -> битовая маска (TODO) "<< order.md_flags_set << '\n';
    os << "Числовой идентификатор инструмента "<< order.security_id << '\n';
    os << "Порядковый номер инкрементального обновления "<< order.rpt_seq << '\n';
    os << "Тип инкрементального обновления "<< order.md_update_action << '\n';
    os << "Тип заявки "<< order.md_entry_type << '\n';
    os << "+++++++++++++++++++++ OrderExecution packet end: +++++++++++++++\n";

    return os;
}
