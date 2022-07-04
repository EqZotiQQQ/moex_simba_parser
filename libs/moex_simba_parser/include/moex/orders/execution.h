#pragma once

#include <cstdint>
#include <binary_parser/buffered_reader.h>

struct OrderExecution {
    int64_t md_entry_id;    // Идентификатор заявки
    int64_t md_entry_px;    // Цена заявки
    int64_t md_entry_size;  // Оставшееся количество в заявке
    int64_t last_px;        // Цена сделки
    int64_t last_qty;       // Объем сделки
    int64_t trade_id;       // Идентификатор сделки
    int64_t md_flags;   // Типы сделок -> битовая маска
    int32_t security_id;    // Числовой идентификатор инструмента
    uint32_t rpt_seq;        // Порядковый номер инкрементального обновления
    uint8_t md_update_action;// Тип инкрементального обновления 1 - change; 2 - delete
    uint8_t md_entry_type;   // Тип заявки - 2 - продажа или 1 - покупка

    explicit OrderExecution(BufferedReader& reader);
    void parse(BufferedReader& reader);

    size_t get_parsed_bytes() const {return 0;}
    std::string to_string() const {return std::string{"TODO"};}

    friend std::ostream& operator<<(std::ostream& os, const OrderExecution& header);
};