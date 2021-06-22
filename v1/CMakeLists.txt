add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -mavx2
    -mfma
)

include_directories(include)

add_subdirectory(src/filters)
add_subdirectory(src/noise)
add_subdirectory(src/version)
add_subdirectory(src/windows)

add_library(plasma-dsp
    $<TARGET_OBJECTS:plasma_filters>
    $<TARGET_OBJECTS:plasma_noise>
    $<TARGET_OBJECTS:plasma_version>
    $<TARGET_OBJECTS:plasma_windows>
)

add_subdirectory(examples)
add_subdirectory(tests)
