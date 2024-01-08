# This is a shared library providing the symbols ModelicaError and ModelicaWarning.
# It is needed for the modelica_static_helper library to make symbol overloading work.
# This is probably not the most elegant way.

set(CMAKE_CXX_VISIBILITY_PRESET default)
set(CMAKE_C_VISIBILITY_PRESET default)

add_library(modelica_utilities_DONT_USE SHARED EXCLUDE_FROM_ALL ModelicaUtilities.cpp)
target_link_libraries(modelica_utilities_DONT_USE PUBLIC Export)
target_include_directories(modelica_utilities_DONT_USE PUBLIC include)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
