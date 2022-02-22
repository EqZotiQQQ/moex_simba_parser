#pragma once

#include <fstream>
#include "types/typenames.h"
#include "utils/parsers.h"

class OrderExecution {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const OrderExecution& order);
private:
    i64 md_entry_id;    // Идентификатор заявки
    i64 md_entry_px;    // Цена заявки
    i64 md_entry_size;  // Оставшееся количество в заявке
    i64 last_px;        // Цена сделки
    i64 last_qty;       // Объем сделки
    i64 trade_id;       // Идентификатор сделки
    i64 md_flags;   // Типы сделок -> битовая маска
    i32 security_id;    // Числовой идентификатор инструмента
    u32 rpt_seq;        // Порядковый номер инкрементального обновления
    u8 md_update_action;// Тип инкрементального обновления 1 - change; 2 - delete
    u8 md_entry_type;   // Тип заявки - 2 - продажа или 1 - покупка

public:
    static constexpr u8 size_bytes = 66;

    static constexpr u8 day = 0x1; //- Котировочная (Day)
    static constexpr u8 ioc = 0x2; //- Встречная (IOC)
    static constexpr u8 otc = 0x4; //- Внесистемная сделка, включая адресные, сделки по ногам, сделки RFS
    static constexpr u16 end_of_transaction= 0x1000;// - Признак последней записи в транзакции матчинга
    static constexpr u32 fill_or_kill = 0x80000;// - Заявка Fill-or-Kill
    static constexpr u32 negotiated_order = 0x4000000; // - Адресная заявка
    static constexpr u32 multi_leg_order = 0x8000000; // - Признак заявки по связке
    static constexpr u32 sign_of_order_deletion_due_to_a_cross_trade= 0x20000000; // - Признак удаления остатка заявки по причине кросс-сделки
    static constexpr u64 cancel_of_disconnect_result= 0x100000000; // - Запись является результатом отмены заявок сервисом "Cancel on Disconnect"
    static constexpr u64 synthetic_order= 0x200000000000; // - Признак синтетической заявки
    static constexpr u64 rfs_order= 0x400000000000; // - Заявка из системы RFS

    void parse(BufferedReader& parser) {
        md_entry_id = parser.next<i64>();
        md_entry_px = parser.next<i64>();
        md_entry_size = parser.next<i64>();
        last_px = parser.next<i64>();
        last_qty = parser.next<i64>();
        trade_id = parser.next<i64>();
        md_flags = parser.next<i64>();
        security_id = parser.next<i32>();
        rpt_seq = parser.next<u32>();
        md_update_action = parser.next<u8>();
        md_entry_type = parser.next<u8>();
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const OrderExecution& order) {
    os << "==  OrderExecution packet: ==\n";
    os << std::dec;
    os << "ID заявки "<< order.md_entry_id << '\n';
    if (order.md_entry_px == DECIMAL5_NULL) {
        os << "Цена заявки: null\n";
    } else {
        os << "Цена заявки: " << order.md_entry_px << "\n";
    }
    if (order.md_entry_size == INT64_NULL) {
        os << "Цена заявки: null\n";
    } else {
        os << "Цена заявки: " << order.md_entry_size << "\n";
    }
    os << "Цена сделки "<< order.last_px << '\n';
    os << "Объем сделки "<< order.last_qty << '\n';
    os << "Идентификатор сделки "<< order.trade_id << '\n';
    os << "Типы сделок (битовая маска):\n";// (TODO) "<< order.md_flags << '\n';
    if ((order.md_flags & OrderExecution::day) == OrderExecution::day) {
        os << "* 0x1 Котировочная\n";
    }
    if ((order.md_flags & OrderExecution::ioc) == OrderExecution::ioc) {
        os << "* 0x2 Встречная\n";
    }
    if ((order.md_flags & OrderExecution::otc) == OrderExecution::otc) {
        os << "* 0x4 Внесистемная заявка\n";
    }
    if ((order.md_flags & OrderExecution::end_of_transaction) == OrderExecution::end_of_transaction) {
        os << "* 0x1000 - Признак последней записи в транзакции матчинга\n";
    }
    if ((order.md_flags & OrderExecution::fill_or_kill) == OrderExecution::fill_or_kill) {
        os << "* 0x80000 - Заявка Fill-or-Kill\n";
    }
    if ((order.md_flags & OrderExecution::negotiated_order) == OrderExecution::negotiated_order) {
        os << "* 0x4000000 - Признак адресной заявки\n";
    }
    if ((order.md_flags & OrderExecution::multi_leg_order) == OrderExecution::multi_leg_order) {
        os << "* 0x8000000 - Признак заявки по связке\n";
    }
    if ((order.md_flags & OrderExecution::sign_of_order_deletion_due_to_a_cross_trade) == OrderExecution::sign_of_order_deletion_due_to_a_cross_trade) {
        os << "* 0x20000000 - Признак удаления остатка заявки по причине кросс-сделки\n";
    }
    if ((order.md_flags & OrderExecution::cancel_of_disconnect_result) == OrderExecution::cancel_of_disconnect_result) {
        os << "* 0x100000000 - Запись является результатом отмены заявок сервисом Cancel on Disconnect\n";
    }
    if ((order.md_flags & OrderExecution::synthetic_order) == OrderExecution::synthetic_order) {
        os << "* 0x200000000000 - Признак синтетической заявки\n";
    }
    if ((order.md_flags & OrderExecution::rfs_order) == OrderExecution::rfs_order) {
        os << "* 0x400000000000 - Заявка из системы RFS\n";
    }
    os << "Числовой идентификатор инструмента "<< order.security_id << '\n';
    os << "Порядковый номер инкрементального обновления "<< order.rpt_seq << '\n';
    os << "Тип инкрементального обновления: ";//<< order.md_update_action << '\n';
    if (order.md_update_action == '1') {
        os << "Изменение (Change)\n";
    } else {
        os << "Удаление (Delete)\n";
    }
    os << "Тип заявки: ";//<< order.md_entry_type << '\n';
    if (order.md_entry_type == '0') {
        os << "Продажа (Bid)\n";
    } else {
        os << "Покупка (Ask)\n";
    }
    os << "== OrderExecution packet end: ==\n";

    return os;
}
