include_directories(..)

add_executable(test
    nco.cpp
)

target_link_libraries(test
    gtest
    gtest_main
    pthread
)
