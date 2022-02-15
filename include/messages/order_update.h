#pragma once

#include <vector>
#include "../types/typenames.h"

class OrderUpdate {
    friend std::ostream& operator<<(std::ostream& os, const OrderUpdate& pcap);
    friend std::ofstream& operator<<(std::ofstream& os, const OrderUpdate& pcap);
private:
    i64 md_entry_id; // ID заявки
    i64 md_entry_px; // Цена заявки
    i64 md_entry_size; // Объём заявки
    u64 md_flags; // Тип -> битовая маска -> в доке битовая маска
    i32 security_id;  // ID инструмента
    u32 rpt_seq;      // ID инкремент. обновления
    u8  md_update_action; // Тип инкремент. обновления ( 0 - new; 2 - delete)
    u8  md_entry_type;  // Тип заявки (0 - продажа 1 - покупка)
public:
    OrderUpdate() {}
    static constexpr u8 size_bytes = 42;

    static constexpr u8 day = 0x1; //- Котировочная (Day)
    static constexpr u8 IOC = 0x2; //- Встречная (IOC)
    static constexpr u8 OTC = 0x4; //- Внесистемная заявка
    static constexpr u16 end_of_transaction= 0x1000;// - Признак последней записи в транзакции матчинга
    static constexpr u32 fill_or_kill = 0x80000;// - Заявка Fill-or-Kill
    static constexpr u32 order_move_result= 0x100000;//- Запись является результатом перемещения заявки
    static constexpr u32 cancel_result = 0x200000;//- Запись является результатом удаления заявки
    static constexpr u32 mass_cancel_result = 0x400000;//- Запись является результатом группового удаления заявок
    static constexpr u32 negotiated_order = 0x4000000; // - Признак адресной заявки
    static constexpr u32 multi_leg_order = 0x8000000; // - Признак заявки по связке
    static constexpr u32 sign_of_order_deletion_due_to_a_cross_trade= 0x20000000; // - Признак удаления остатка заявки по причине кросс-сделки
    static constexpr u64 cancel_of_disconnect_result= 0x100000000; // - Запись является результатом отмены заявок сервисом "Cancel on Disconnect"
    static constexpr u64 synthetic_order= 0x200000000000; // - Признак синтетической заявки
    static constexpr u64 RFS_order= 0x400000000000; // - Заявка из системы RFS

    void parse(std::ifstream& file, Endian endian) {
        md_entry_id = Parsers::parse_i64(file, endian);
        md_entry_px = Parsers::parse_i64(file, endian);
        md_entry_size = Parsers::parse_i64(file, endian);
        md_flags = Parsers::parse_u64(file, endian);
        security_id = Parsers::parse_i32(file, endian);
        rpt_seq = Parsers::parse_u32(file, endian);
        md_update_action = Parsers::parse_u8(file, endian);
        md_entry_type = Parsers::parse_u8(file, endian);
    }
};

std::ostream& operator<<(std::ostream& os, const OrderUpdate& pcap) {
    os << "==  OrderUpdate packet: ==\n";
    os << std::dec;
    os << "ID заявки: " << pcap.md_entry_id << '\n';
    os << "Цена заявки: " << pcap.md_entry_px << '\n';
    os << "Объём заявки: " << pcap.md_entry_size << '\n';
    os << "Тип заявки (битовая маска):\n";// << pcap.md_flags << '\n';

    if ((pcap.md_flags & OrderUpdate::day) == OrderUpdate::day) {
        os << "* 0x1 Котировочная\n";
    }
    if ((pcap.md_flags & OrderUpdate::IOC) == OrderUpdate::IOC) {
        os << "* 0x2 Встречная\n";
    }
    if ((pcap.md_flags & OrderUpdate::OTC) == OrderUpdate::OTC) {
        os << "* 0x4 Внесистемная заявка\n";
    }
    if ((pcap.md_flags & OrderUpdate::end_of_transaction) == OrderUpdate::end_of_transaction) {
        os << "* 0x1000 - Признак последней записи в транзакции матчинга\n";
    }
    if ((pcap.md_flags & OrderUpdate::fill_or_kill) == OrderUpdate::fill_or_kill) {
        os << "* 0x80000 - Заявка Fill-or-Kill\n";
    }
    if ((pcap.md_flags & OrderUpdate::order_move_result) == OrderUpdate::order_move_result) {
        os << "* 0x100000 - Запись является результатом перемещениязаявки\n";
    }
    if ((pcap.md_flags & OrderUpdate::cancel_result) == OrderUpdate::cancel_result) {
        os << "* 0x200000 - Запись является результатом удаления заявки\n";
    }
    if ((pcap.md_flags & OrderUpdate::mass_cancel_result) == OrderUpdate::mass_cancel_result) {
        os << "* 0x400000 - Запись является результатом группового удаления заявок\n";
    }
    if ((pcap.md_flags & OrderUpdate::negotiated_order) == OrderUpdate::negotiated_order) {
        os << "* 0x4000000 - Признак адресной заявки\n";
    }
    if ((pcap.md_flags & OrderUpdate::multi_leg_order) == OrderUpdate::multi_leg_order) {
        os << "* 0x8000000 - Признак заявки по связке\n";
    }
    if ((pcap.md_flags & OrderUpdate::sign_of_order_deletion_due_to_a_cross_trade) ==
        OrderUpdate::sign_of_order_deletion_due_to_a_cross_trade) {
        os << "* 0x20000000 - Признак удаления остатка заявки по причине кросс-сделки\n";
    }
    if ((pcap.md_flags & OrderUpdate::cancel_of_disconnect_result) == OrderUpdate::cancel_of_disconnect_result) {
        os << "* 0x100000000 - Запись является результатом отмены заявок сервисом Cancel on Disconnect\n";
    }
    if ((pcap.md_flags & OrderUpdate::synthetic_order) == OrderUpdate::synthetic_order) {
        os << "* 0x200000000000 - Признак синтетической заявки\n";
    }
    if ((pcap.md_flags & OrderUpdate::RFS_order) == OrderUpdate::RFS_order) {
        os << "* 0x400000000000 - Заявка из системы RFS\n";
    }

    os << "ID инструмента: " << pcap.security_id << '\n';
    os << "ID инкрементального обновления: " << pcap.rpt_seq << '\n';
    os << "Тип инкрементального обновления: ";
    if (pcap.md_update_action == '0') {
        os << "Создание (New)\n";
    } else if (pcap.md_update_action == '2') {
        os << "Удаление (Delete)\n";
    }
    os << "ID Тип заявки: ";
    if (pcap.md_entry_type == '0') {
        os << "Продажа (Bid)\n";
    } else {
        os << "Покупка (Ask)\n";
    }
    os << "== OrderUpdate packet end: ==\n";
    return os;
}

std::ofstream& operator<<(std::ofstream& os, const OrderUpdate& pcap) {
    os << "==  OrderUpdate packet: ==\n";
    os << std::dec;
    os << "ID заявки: " << pcap.md_entry_id << '\n';
    os << "Цена заявки: " << pcap.md_entry_px << '\n';
    os << "Объём заявки: " << pcap.md_entry_size << '\n';
    os << "Тип заявки (битовая маска):\n";// << pcap.md_flags << '\n';

    if ((pcap.md_flags & OrderUpdate::day) == OrderUpdate::day) {
        os << "* 0x1 Котировочная\n";
    }
    if ((pcap.md_flags & OrderUpdate::IOC) == OrderUpdate::IOC) {
        os << "* 0x2 Встречная\n";
    }
    if ((pcap.md_flags & OrderUpdate::OTC) == OrderUpdate::OTC) {
        os << "* 0x4 Внесистемная заявка\n";
    }
    if ((pcap.md_flags & OrderUpdate::end_of_transaction) == OrderUpdate::end_of_transaction) {
        os << "* 0x1000 - Признак последней записи в транзакции матчинга\n";
    }
    if ((pcap.md_flags & OrderUpdate::fill_or_kill) == OrderUpdate::fill_or_kill) {
        os << "* 0x80000 - Заявка Fill-or-Kill\n";
    }
    if ((pcap.md_flags & OrderUpdate::order_move_result) == OrderUpdate::order_move_result) {
        os << "* 0x100000 - Запись является результатом перемещениязаявки\n";
    }
    if ((pcap.md_flags & OrderUpdate::cancel_result) == OrderUpdate::cancel_result) {
        os << "* 0x200000 - Запись является результатом удаления заявки\n";
    }
    if ((pcap.md_flags & OrderUpdate::mass_cancel_result) == OrderUpdate::mass_cancel_result) {
        os << "* 0x400000 - Запись является результатом группового удаления заявок\n";
    }
    if ((pcap.md_flags & OrderUpdate::negotiated_order) == OrderUpdate::negotiated_order) {
        os << "* 0x4000000 - Признак адресной заявки\n";
    }
    if ((pcap.md_flags & OrderUpdate::multi_leg_order) == OrderUpdate::multi_leg_order) {
        os << "* 0x8000000 - Признак заявки по связке\n";
    }
    if ((pcap.md_flags & OrderUpdate::sign_of_order_deletion_due_to_a_cross_trade) ==
        OrderUpdate::sign_of_order_deletion_due_to_a_cross_trade) {
        os << "* 0x20000000 - Признак удаления остатка заявки по причине кросс-сделки\n";
    }
    if ((pcap.md_flags & OrderUpdate::cancel_of_disconnect_result) == OrderUpdate::cancel_of_disconnect_result) {
        os << "* 0x100000000 - Запись является результатом отмены заявок сервисом Cancel on Disconnect\n";
    }
    if ((pcap.md_flags & OrderUpdate::synthetic_order) == OrderUpdate::synthetic_order) {
        os << "* 0x200000000000 - Признак синтетической заявки\n";
    }
    if ((pcap.md_flags & OrderUpdate::RFS_order) == OrderUpdate::RFS_order) {
        os << "* 0x400000000000 - Заявка из системы RFS\n";
    }

    os << "ID инструмента: " << pcap.security_id << '\n';
    os << "ID инкрементального обновления: " << pcap.rpt_seq << '\n';
    os << "Тип инкрементального обновления: ";
    if (pcap.md_update_action == '0') {
        os << "Создание (New)\n";
    } else {
        os << "Удаление (Delete)\n";
    }
    os << "ID Тип заявки: ";
    if (pcap.md_entry_type == '0') {
        os << "Продажа (Bid)\n";
    } else {
        os << "Покупка (Ask)\n";
    }
    os << "== OrderUpdate packet end: ==\n";
    return os;
}
