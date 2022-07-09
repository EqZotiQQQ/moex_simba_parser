project(moex_simba_parser)

add_library(${PROJECT_NAME}
        src/global_pcap_header.cpp
        src/udp_header.cpp
        src/ip_header.cpp
        src/record_header.cpp
        src/market_data_packet_header.cpp
        src/market_data_packet.cpp
        src/incremental_packet.cpp
        src/snapshot_packet.cpp
        src/best_prices_order.cpp
        src/order_execution.cpp
        src/order_book_snapshot.cpp
        src/order_update.cpp
        src/sbe_message.cpp
        src/protocol_version.cpp include/pcap_config.h)

add_library(libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_compile_features(${PROJECT_NAME}
        PRIVATE
        cxx_std_20)

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wno-missing-braces )


target_link_libraries(${PROJECT_NAME}
        PRIVATE
        lib::binary_parser
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        fmt
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        date
        )

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
