set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)


#######################################
#          TEST DEFINITIONS           #
#######################################
# Customise this part to use your own CPP files

# You can link the target "modelica_static_helper" in order to provide symbols
# that are undefined in the externalmedialib. When the library is actually used
# by modelica, these symbols will be provided by the modelica standard library.
# But for testing purposes it is easier to provide them ourselves.

if (COOLPROP)
  add_executable (main EXCLUDE_FROM_ALL coolprop_comparisons.cpp)
  target_link_libraries(main PRIVATE modelica_static_helper ${LIBRARY_NAME} CoolProp )
endif()
