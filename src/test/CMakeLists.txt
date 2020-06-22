################################################################################
##! \file
##! \details This file is part of the AmateurRadio.Engineer project
##! \author Jean-Sebastien Dominique <jd@amateurradio.engineer>
##! \date 2020
##! \copyright AmateurRadio.Engineer
################################################################################
find_package(benchmark REQUIRED)
file(GLOB_RECURSE SRC_FILES *.cpp)
add_executable(test EXCLUDE_FROM_ALL ${SRC_FILES})
target_link_libraries(test LINK_PUBLIC benchmark::benchmark benchmark::benchmark_main pthread)
# add_custom_command(TARGET test POST_BUILD COMMAND ./test WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
