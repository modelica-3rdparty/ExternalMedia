# For all these static libraries: If they are linked into a dynamic library
# don't export any symbols from these, unless they are explicitly exported via
# __attribute__((visibility("default")))
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_subdirectory(ThermodynamicStructs)
add_subdirectory(CommonInclude)
add_subdirectory(SolverInterface)
add_subdirectory(SolverTest)
add_subdirectory(errorhandling)

# Add the target for ExternalMedia
add_library(${LIBRARY_NAME} SHARED
  externalmedialib.cpp
  mingw_gcc_comutil.cpp
  solvermap.cpp
)

set_property(TARGET ${LIBRARY_NAME} PROPERTY VERSION ${APP_VERSION})
set_property(TARGET ${LIBRARY_NAME} PROPERTY DEBUG_POSTFIX "")
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_FLUIDPROP=$<IF:$<BOOL:${FLUIDPROP}>,1,0>)
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_COOLPROP=$<IF:$<BOOL:${COOLPROP}>,1,0>)


target_include_directories(${LIBRARY_NAME} PUBLIC include)
target_include_directories(${LIBRARY_NAME} PRIVATE private_include)
target_link_libraries(${LIBRARY_NAME} PUBLIC ThermodynamicStructs Export)
target_link_libraries(${LIBRARY_NAME} PRIVATE SolverTest)

if(COOLPROP)
  add_subdirectory(SolverCoolProp)
  target_link_libraries(${LIBRARY_NAME} PRIVATE SolverCoolProp)
endif()
if(FLUIDPROP)
  add_subdirectory(SolverFluidProp)
  target_link_libraries(${LIBRARY_NAME} PRIVATE SolverFluidProp)
endif()

target_link_libraries(${LIBRARY_NAME} PRIVATE errorhandling)
target_link_libraries(${LIBRARY_NAME} PRIVATE SolverInterface)

if(WIN32)
  message(STATUS "Installation path: ${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}/${MODELICA_COMPILER}")
  INSTALL(TARGETS "${LIBRARY_NAME}" DESTINATION "${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}/${MODELICA_COMPILER}")
else()
  # Both OpenModelica and Dymola on Linux DO NOT expect a compiler-specific subdirectory
  # NB: This means that subsequent builds for different compilers overwrite the previously installed binaries.
  message(STATUS "Installation path: ${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}")
  INSTALL(TARGETS "${LIBRARY_NAME}" DESTINATION "${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}")
endif()




