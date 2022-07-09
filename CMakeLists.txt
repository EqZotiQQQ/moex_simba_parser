cmake_minimum_required(VERSION 3.16)

project(moex_simba_parser VERSION 0.0.1 LANGUAGES CXX)

include(CTest)

add_subdirectory(libs)
add_subdirectory(app)

add_subdirectory(3rd_party/binary_parser)
add_subdirectory(3rd_party/googletest)
add_subdirectory(3rd_party/benchmark)
add_subdirectory(3rd_party/fmt)
add_subdirectory(3rd_party/date)
add_subdirectory(3rd_party/argparse)
