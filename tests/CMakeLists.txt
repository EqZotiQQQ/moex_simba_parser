cmake_minimum_required(VERSION 3.21)

project(simba_decoder_test)

set(CMAKE_CXX_STANDARD 20)

include_directories(../lib/include)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} gtest gtest_main)
target_link_libraries(${PROJECT_NAME} parser)
