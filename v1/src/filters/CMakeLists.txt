add_library(plasma_filters OBJECT
    windowed.cpp
)

if (BUILD_SHARED_LIBS)
    set_property(TARGET plasma_filters PROPERTY POSITION_INDEPENDENT_CODE 1)
endif()
