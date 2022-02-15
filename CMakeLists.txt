cmake_minimum_required(VERSION 3.21)
project(another_pcap_parser)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Werror -Wall)

include_directories(include)

set(SRC main.cpp)
set(HEADERS
        include/pcap_parser.h
        include/exceptions.h
        include/ip.h
        include/utils/parsers.h
        include/udp.h
        include/types/constants.h
        include/types/typenames.h
        include/tests/tests.h
        include/messages/order_update.h
        include/messages/order_execution.h
        include/messages/order_book_snapshot.h
        include/market_data_packet.h
        include/packets/incremental_packet.h
        include/packets/snapshot_packet.h
        include/packets/market_data_packet.h
        include/packets/sbe/sbe_message.h
        include/messages/order_best_prices.h
        include/types/enumerations.h
        include/pcap_header.h)

add_executable(another_pcap_parser ${SRC}  ${HEADERS})
