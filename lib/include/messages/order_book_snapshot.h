#pragma once

#include <vector>
#include "types/typenames.h"
#include "types/enumerations.h"
#include "utils/parsers.h"


class OrderBookSnapshot {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const OrderBookSnapshot& order);
private:
    i64 md_entry_id;
    u64 transact_time;
    i64 md_entry_px;
    i64 md_entry_size;
    i64 trade_id;
    u64 md_flags;
    u8 md_entry_type;
public:

    static constexpr u8 size {49};

    static constexpr u8 day = 0x1;
    static constexpr u8 ioc = 0x2;

    static constexpr u8 bid = '0';
    static constexpr u8 ask = '1';
    static constexpr u8 empty_book = 'J';
public:
    void parse(BufferedReader& parser) {
        md_entry_id = parser.next<i64>();
        transact_time = parser.next<u64>();
        md_entry_px = parser.next<i64>();
        md_entry_size = parser.next<i64>();
        trade_id = parser.next<i64>();
        md_flags = parser.next<u64>();
        md_entry_type = parser.next<u8>();
    }
};

class OrderBookSnapshotPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const OrderBookSnapshotPacket& order);
private:
    i32 security_id;                            //  ID тулзы
    u32 last_msg_seq_num_processed;            //  Номер последнего обработанного сообщения
    u32 rpt_seq;                               //  Порядковый номер инкрементального обновления
    u32 exchange_trading_session_id;           //  ID торговой сделки
    u16 block_len;                             //  Длина блока
    u8 no_md_entries;                          //  Размер пачки заявок
    std::vector<OrderBookSnapshot> md_entries;      //  Размер пачки заявок
public:
    static constexpr u8 size = 19; // total size = 19 + sizeofOrderBookSnapshot * len(bids_slice)
public:
    void parse(BufferedReader& parser) {
        security_id = parser.next<i32>();
        last_msg_seq_num_processed = parser.next<u32>();
        rpt_seq = parser.next<u32>();
        exchange_trading_session_id = parser.next<u32>();
        block_len = parser.next<u16>();
        no_md_entries = parser.next<u8>();
        for (int i = 0; i < no_md_entries; i++) {
            OrderBookSnapshot snapshot {};
            snapshot.parse(parser);
            md_entries.emplace_back(snapshot);
        }
    }

    u64 entries() const noexcept {
        return md_entries.size();
    }
};

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const OrderBookSnapshot& order) {
    os << "== OrderBookSnapshot packet: ==\n";
    if (order.md_entry_id == INT64_NULL) {
        os << "ID заявки: Null"<< '\n';
    } else {
        os << "ID заявки: "<< order.md_entry_id << '\n';
    }
    os << "Время заявки: "<< order.transact_time << '\n';
    if (order.md_entry_px == DECIMAL5_NULL) {
        os << "Цена заявки: Null\n";
    } else {
        os << "Цена заявки: "<< order.md_entry_px << '\n';
    }

    if (order.md_entry_size == INT64_NULL) {
        os << "Оставшееся количество в заявке: Null\n";
    } else {
        os << "Оставшееся количество в заявке: "<< order.md_entry_size << '\n';
    }

    if (order.trade_id == INT64_NULL) {
        os << "Идентификатор сделки: Null\n";
    } else {
        os << "Идентификатор сделки: "<< order.trade_id << '\n';
    }

    os << "Типы сделок: "; // -> битовая маска (TODO) "<< order.md_flags << '\n';
    if ((order.md_flags & OrderBookSnapshot::day) == OrderBookSnapshot::day) {
        os << "Котировочная сделка (Day)\n";
    }
    if ((order.md_flags & OrderBookSnapshot::ioc) == OrderBookSnapshot::ioc) {
        os << "Встречная сделка (IOC)\n";
    }
    os << "Тип заявки ";//<< order.md_entry_type << '\n';
    if ((order.md_entry_type & OrderBookSnapshot::bid) == OrderBookSnapshot::bid) {
        os << "Продажа (Bid)\n";

    } else if ((order.md_entry_type & OrderBookSnapshot::ask) == OrderBookSnapshot::ask) {
        os << "Покупка (Ask)\n";
    } else if ((order.md_entry_type & OrderBookSnapshot::empty_book) == OrderBookSnapshot::empty_book) {
        os << "Пустой стакан\n";
    }
    os << "== OrderBookSnapshot packet end: ==\n";
    return os;
}

template <typename OutPipe>
OutPipe& operator<<(OutPipe& os, const OrderBookSnapshotPacket& order) {
    os << "== OrderBookSnapshot packet: ==\n";
    os << std::dec;
    os << "Числовой идентификатор инструмента "          << static_cast<i32>(order.security_id) << '\n';
    os << "Последнее обработанное сообщение "            << static_cast<i32>(order.last_msg_seq_num_processed) << '\n';
    os << "Порядковый номер инкрементального обновления "<< static_cast<i32>(order.rpt_seq) << '\n';
    os << "ID торговой сделки "                          << static_cast<i32>(order.exchange_trading_session_id) << '\n';
    os << "Длина блока "                                 << static_cast<i32>(order.block_len) << '\n';
    os << "Количество заявок "                           << static_cast<i32>(order.no_md_entries) << '\n';
    for (int i = 0; i < order.no_md_entries; i++) {
        os << "Заявка №" << i + 1 << '\n';
        os << order.md_entries[i] << '\n';
    }
    os << "== OrderBookSnapshot packet end: ==\n";
    return os;
}
