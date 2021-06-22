add_library(plasma_windows OBJECT
    kaiser.cpp
)

if (BUILD_SHARED_LIBS)
    set_property(TARGET plasma_windows PROPERTY POSITION_INDEPENDENT_CODE 1)
endif()
