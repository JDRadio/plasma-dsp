include_directories(.)

add_compile_options(
    -Wall
    -Wextra
    -pedantic
    -mavx2
    -mfma
)

add_library(compiled
    compile.cpp
)
