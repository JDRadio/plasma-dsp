include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()

add_executable(test
    version/version.cpp
)

target_link_libraries(test
    plasma-dsp
    gtest_main
    gtest
    pthread
)

add_test(
    test test
)
