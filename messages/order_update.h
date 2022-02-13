#pragma once

#include <vector>
#include "../types.h"
#include "message_base.h"

struct OrderUpdate : public MessageBase {
    i64 md_entry_id; // ID заявки
    i64 md_entry_px; // Цена заявки
    i64 md_entry_size; // Объём заявки
    u64 md_flags_set; // Тип -> битовая маска -> в доке битовая маска
    i32 security_id;  // ID инструмента
    u32 rpt_seq;      // ID инкремент. обновления
    u8  md_update_action; // Тип инкремент. обновления ( 0 - new; 2 - delete)
    u8  md_entry_type;  // Тип заявки (0 - продажа 1 - покупка)

    static constexpr u8 size_bytes = 42;

    static OrderUpdate parse(std::ifstream& file, Endian endian) {
        return OrderUpdate {
               .md_entry_id = Parsers::parse_i64(file, endian),
               .md_entry_px = Parsers::parse_i64(file, endian),
               .md_entry_size = Parsers::parse_i64(file, endian),
               .md_flags_set = Parsers::parse_u64(file, endian),
               .security_id = Parsers::parse_i32(file, endian),
               .rpt_seq = Parsers::parse_u32(file, endian),
               .md_update_action = Parsers::parse_u8(file, endian),
               .md_entry_type = Parsers::parse_u8(file, endian),
        };
    }
};

std::ostream& operator<<(std::ostream& os, const OrderUpdate& pcap) {
        os << "====================  OrderUpdate packet: ===================\n";
        os << "ID заявки: " << pcap.md_entry_id << '\n';
        os << "Цена заявки: " << pcap.md_entry_px << '\n';
        os << "Объём заявки: " << pcap.md_entry_size << '\n';
        os << "Тип заявки (битовая маска): " << pcap.md_flags_set << '\n';
        os << "ID инструмента: " << pcap.security_id << '\n';
        os << "ID инкрементального обновления: " << pcap.rpt_seq << '\n';
        os << "Тип инкрементального обновления: " << pcap.md_update_action << '\n';
        os << "ID Тип заявки: " << pcap.md_entry_type << '\n';
        os << "+++++++++++++++++++++ OrderUpdate packet end: +++++++++++++++\n";

    return os;
}
