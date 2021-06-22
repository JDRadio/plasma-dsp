add_library(plasma_noise OBJECT
    wgn.cpp
)

if (BUILD_SHARED_LIBS)
    set_property(TARGET plasma_noise PROPERTY POSITION_INDEPENDENT_CODE 1)
endif()
