set(plasma_headers
    plasma/math.hpp
    plasma/nco.hpp
    plasma/nco_no_lookup.ipp
    plasma/nco_use_lookup.ipp
)

add_library(dsp INTERFACE
    ${plasma_headers}
)

target_include_directories(dsp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/plasma>
    $<INSTALL_INTERFACE:include/plasma>
)

target_compile_features(dsp INTERFACE
    cxx_std_17
)

install(
    TARGETS dsp
    EXPORT plasmaExport
)

install(
    EXPORT plasmaExport
    NAMESPACE plasma::
    DESTINATION lib/cmake/plasma
)

install(
    FILES ${plasma_headers}
    DESTINATION include/plasma
)

option(BUILD_BENCHMARK "Build the benchmarking utility" OFF)
option(BUILD_TESTS "Build the tests" OFF)

if (BUILD_BENCHMARK)
    add_subdirectory(benchmark)
endif ()

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()
