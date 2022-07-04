#pragma once

#include <cstdint>
#include <binary_parser/buffered_reader.h>

struct Update {
    int64_t  md_entry_id; // ID заявки
    int64_t  md_entry_px; // Цена заявки
    int64_t  md_entry_size; // Объём заявки
    uint64_t md_flags; // Тип -> битовая маска -> в доке битовая маска
    int32_t  security_id;  // ID инструмента
    uint32_t rpt_seq;      // ID инкремент. обновления
    uint8_t  md_update_action; // Тип инкремент. обновления ( 0 - new; 2 - delete)
    uint8_t  md_entry_type;  // Тип заявки (0 - продажа 1 - покупка)

    explicit Update(BufferedReader& reader);
    void parse(BufferedReader& reader);

    static size_t get_parsed_bytes() { return 42; }
    std::string to_string() const {return "TODO";}

    friend std::ostream& operator<<(std::ostream& os, const Update& header);
};