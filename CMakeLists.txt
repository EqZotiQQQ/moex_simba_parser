cmake_minimum_required(VERSION 3.21)
project(another_pcap_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(another_pcap_parser main.cpp pcap_parser.h exceptions.h pcap_header.h parsers.h utils.h packet_header.h constants.h types.h layer4.h tests.h messages/order_update.h messages/order_execution.h messages/order_book_snapshot.h messages/message_base.h simba_bin_decoder.h packets/incremental_packet.h packets/snapshot_packet.h packets/packet_base.h packets/market_data_packet.h packets/sbe/sbe_message.h)
