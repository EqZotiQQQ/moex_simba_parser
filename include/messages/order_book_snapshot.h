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
    void parse(std::ifstream& file, Endian endian) {
        md_entry_id = Parsers::parse_i64(file, endian);
        transact_time = Parsers::parse_u64(file, endian);
        md_entry_px = Parsers::parse_i64(file, endian);
        md_entry_size = Parsers::parse_i64(file, endian);
        trade_id = Parsers::parse_i64(file, endian);
        md_flags = Parsers::parse_u64(file, endian);
        md_entry_type = Parsers::parse_u8(file);
    }
};

class OrderBookSnapshotPacket {
    template <typename OutPipe>
    friend OutPipe& operator<<(OutPipe& os, const OrderBookSnapshotPacket& order);
private:
    int32_t security_id;                            //  ID тулзы
    uint32_t last_msg_seq_num_processed;            //  Номер последнего обработанного сообщения
    uint32_t rpt_seq;                               //  Порядковый номер инкрементального обновления
    uint32_t exchange_trading_session_id;           //  ID торговой сделки
    uint16_t block_len;                             //  Длина блока
    uint8_t no_md_entries;                          //  Размер пачки заявок
    std::vector<OrderBookSnapshot> md_entries;      //  Размер пачки заявок
public:
    static constexpr u8 size = 19; // total size = 19 + sizeofOrderBookSnapshot * len(bids_slice)
public:
    void parse(std::ifstream& file, Endian endian) {
        security_id = Parsers::parse_i32(file, endian);
        last_msg_seq_num_processed = Parsers::parse_u32(file, endian);
        rpt_seq = Parsers::parse_u32(file, endian);
        exchange_trading_session_id = Parsers::parse_u32(file, endian);
        block_len = Parsers::parse_u16(file, endian);
        no_md_entries = Parsers::parse_u8(file);
        for (int i = 0; i < no_md_entries; i++) {
            OrderBookSnapshot snapshot {};
            snapshot.parse(file, endian);
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
