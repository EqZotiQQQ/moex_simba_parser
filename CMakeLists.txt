cmake_minimum_required(VERSION 3.21)
project(another_pcap_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(another_pcap_parser main.cpp pcap_parser.h exceptions.h pcap_header.h parsers.h utils.h packet_header.h constants.h types.h layer2.h layer4.h market_data.h tests.h messages/order_update.h messages/order_execution.h messages/order_book_snapshot.h messages/unsupported.h messages/message_base.h simba_bin_decoder.h)
