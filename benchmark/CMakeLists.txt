cmake_minimum_required(VERSION 3.21)

project(parser_benchmark)

set(CMAKE_CXX_STANDARD 20)

include_directories(../lib/include)

find_package(benchmark REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)
target_link_libraries(${PROJECT_NAME} parser)
