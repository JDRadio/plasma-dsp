set(plasma_headers
    plasma/kaiser.hpp
    plasma/kaiser.ipp
    plasma/math.hpp
    plasma/nco_no_lookup.ipp
    plasma/nco_use_lookup.ipp
    plasma/nco.hpp
    plasma/rrc.hpp
    plasma/rrc.ipp
)

add_library(plasma_dsp INTERFACE)

target_include_directories(plasma_dsp INTERFACE
    ${PROJECT_SOURCE_DIR}/plasma
)

target_compile_features(plasma_dsp INTERFACE
    cxx_std_17
)

install(
    TARGETS plasma_dsp
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
