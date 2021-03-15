cmake_minimum_required(VERSION 3.14)
project(plasma
    VERSION 1.0.0
    DESCRIPTION "A fast C++ library for RF DSP"
    HOMEPAGE_URL "https://github.com/JDRadio/plasma-dsp"
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(plasma)
