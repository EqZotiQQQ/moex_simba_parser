cmake_minimum_required(VERSION 3.21)
project(another_pcap_parser)

set(CMAKE_CXX_STANDARD 20)

add_executable(another_pcap_parser main.cpp pcap_parser.h exceptions.h pcap_header.h parsers.h utils.h frame.h constants.h types.h Layer2.h layer3.h layer4.h udp_application_data.h market_data.h)
