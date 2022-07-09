project(moex_simba_parser_app)

add_executable(${PROJECT_NAME}
        main.cpp
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        lib::binary_parser
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        libs::moex_simba_parser
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        fmt
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        date
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        argparse
        )

target_compile_features(${PROJECT_NAME}
        PRIVATE
        cxx_std_20)