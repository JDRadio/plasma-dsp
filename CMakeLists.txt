cmake_minimum_required(VERSION 3.14)
project(plasma
    VERSION 1.0.0
    DESCRIPTION "A fast C++ library for RF DSP"
    HOMEPAGE_URL "https://github.com/JDRadio/plasma-dsp"
)

include(GNUInstallDirs)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(plasma)
