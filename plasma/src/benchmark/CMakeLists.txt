include_directories(..)

find_package(benchmark REQUIRED)

add_executable(benchmark
    nco.cpp
)

target_link_libraries(benchmark
    benchmark::benchmark
    benchmark::benchmark_main
    pthread
)
