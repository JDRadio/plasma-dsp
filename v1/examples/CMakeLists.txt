include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(example-filters
    filters/main.cpp
)

target_link_libraries(example-filters
    plasma-dsp
    fmt
)
