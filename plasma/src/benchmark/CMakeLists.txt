include_directories(..)

add_executable(bm
    nco.cpp
)

target_link_libraries(bm
    benchmark
    benchmark_main
    pthread
)
