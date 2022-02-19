cmake_minimum_required(VERSION 3.21)
project(parser)

set(CMAKE_CXX_STANDARD 20)

include_directories(lib/include)

set(SOURCES src/global_pcap_parser.cpp)

set(HEADERS
        include/exceptions.h
        include/ip_header_parser.h
        include/global_pcap_parser.h
        include/market_data_packet_parser.h
        include/pcap_parser.h
        include/pcap_packet_parser.h
        include/udp_parser.h
        include/messages/order_update.h
        include/messages/order_execution.h
        include/messages/order_book_snapshot.h
        include/messages/order_best_prices.h
        include/packets/incremental_packet.h
        include/packets/snapshot_packet.h
        include/packets/market_data_packet.h
        include/packets/sbe/sbe_message.h
        include/types/enumerations.h
        include/types/constants.h
        include/types/typenames.h
        include/utils/parsers.h
)

include_directories(include)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
