cmake_minimum_required(VERSION 3.21)
project(moex_simba_parser)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Werror -Wall)

include_directories(include)

set(SRC main.cpp)
set(HEADERS
        include/pcap_parser.h
        include/exceptions.h
        include/global_pcap_parser.h
        include/utils/parsers.h
        include/udp_parser.h
        include/types/constants.h
        include/types/typenames.h
        include/tests/tests.h
        include/messages/order_update.h
        include/messages/order_execution.h
        include/messages/order_book_snapshot.h
        include/market_data_packet_parser.h
        include/packets/incremental_packet.h
        include/packets/snapshot_packet.h
        include/packets/market_data_packet.h
        include/packets/sbe/sbe_message.h
        include/messages/order_best_prices.h
        include/types/enumerations.h
        include/pcap_packet_parser.h include/ip_header_parser.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(json)

add_executable(${PROJECT_NAME} ${SRC}  ${HEADERS})

target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)
