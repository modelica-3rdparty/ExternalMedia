
# This helper library provides symbols (ModelicaError and ModelicaWarning) so
# that you don't need to link the full modelica standard library in order to
# use/test externalmedialib.

set(CMAKE_CXX_VISIBILITY_PRESET default)
set(CMAKE_C_VISIBILITY_PRESET default)

add_library(modelica_static_helper STATIC ModelicaUtilities.cpp)
target_link_libraries(modelica_static_helper PUBLIC Export modelica_utilities_DONT_USE)
target_include_directories(modelica_static_helper PUBLIC include)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
