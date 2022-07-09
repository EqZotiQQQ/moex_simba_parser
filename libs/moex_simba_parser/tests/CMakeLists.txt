project(moex_simba_parser_tests)

add_executable(${PROJECT_NAME}
        main.cpp
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        libs::moex_simba_parser
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        lib::binary_parser
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        gtest
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
        pthread
        )

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        argparse
        )

target_compile_features(${PROJECT_NAME}
        PRIVATE
        cxx_std_20)

target_compile_options(${PROJECT_NAME}
        PRIVATE
        -fsanitize=undefined
        -fsanitize=address
        )

target_link_options(${PROJECT_NAME}
        PRIVATE
        -fsanitize=address
        -fsanitize=undefined
        #        -fsanitize=integer -fsanitize=thread
        )

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wno-missing-braces )

add_test(lib1_tests ${PROJECT_NAME})
