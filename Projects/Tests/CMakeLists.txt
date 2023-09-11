

#######################################
#          TEST DEFINITIONS           #
#######################################
# Customise this part to use your own CPP files
if (COOLPROP)
  add_executable (main EXCLUDE_FROM_ALL coolprop_comparisons.cpp)
  target_link_libraries(main PRIVATE modelica_utilities_DONT_USE modelica_static_helper ${LIBRARY_NAME} CoolProp)
endif()
