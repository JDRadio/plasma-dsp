cmake_minimum_required(VERSION 3.14)
project(plasma)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(PUBLIC .)

add_subdirectory(src/test EXCLUDE_FROM_ALL)

option(BUILD_EXAMPLES "Build examples" OFF)

if (BUILD_EXAMPLES)
    file(GLOB_RECURSE SRC_FILES src/examples/*.cpp)
    add_executable(plasma ${SRC_FILES})
    # target_link_libraries(plasma LINK_PUBLIC pthread)
endif (BUILD_EXAMPLES)
