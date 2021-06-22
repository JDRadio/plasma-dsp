add_library(plasma_version OBJECT
    version.cpp
)

if (BUILD_SHARED_LIBS)
    set_property(TARGET plasma_version PROPERTY POSITION_INDEPENDENT_CODE 1)
endif()
