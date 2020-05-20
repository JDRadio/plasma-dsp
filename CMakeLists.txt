cmake_minimum_required(VERSION 3.14)
project(plasma)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE SRC_FILES src/examples/*.cpp)
include_directories(PUBLIC .)

add_executable(plasma ${SRC_FILES})
# target_link_libraries(plasma LINK_PUBLIC pthread)
