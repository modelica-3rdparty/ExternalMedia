cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
include(ExternalProject)

#######################################
#         PROJECT INFORMATION         #
#######################################

# Project name
project ("ExternalMedia")

# Project version
set (VERSION_MAJOR 3)
set (VERSION_MINOR 3)
set (VERSION_PATCH 0)
set (APP_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message (STATUS "ExternalMedia library version: ${APP_VERSION}")
set (LIBRARY_NAME "ExternalMediaLib")
set (LIBRARY_HEADER "externalmedialib.h")


#######################################
#       PROJECT-WIDE OPTIONS          #
#######################################
option(ENABLE_DYMOLA "Build a library that is compatible with the Dymola software" ON)
option(ENABLE_OPEN_MODELICA "Build a library that is compatible with the Open Modelica environment" ON)

option(ADD_FLUIDPROP_STATIC "NOT SUPPORTED: Link the static FluidProp library to ExternalMedia" OFF)
option(ADD_FLUIDPROP_SHARED "Link the shared FluidProp library to ExternalMedia, requires FluidProp to be available in the system PATH" OFF)
option(ADD_COOLPROP_STATIC "Link the static CoolProp library to ExternalMedia" ON)
option(ADD_COOLPROP_SHARED "NOT SUPPORTED: Link the shared CoolProp library to ExternalMedia, requires CoolProp to be available in the system PATH" OFF)

# "Build a library that uses the FluidProp solver"
if(ADD_FLUIDPROP_STATIC OR ADD_FLUIDPROP_SHARED)
  set(ENABLE_FLUIDPROP ON)
else()
  set(ENABLE_FLUIDPROP OFF)
endif()

# "Build a library that uses the CoolProp solver"
if(ADD_COOLPROP_STATIC OR ADD_COOLPROP_SHARED)
  set(ENABLE_COOLPROP ON)
else()
  set(ENABLE_COOLPROP OFF)
endif()

macro(set_define BASE_VAR)
  if(BASE_VAR)
    set(${BASE_VAR}_DEFINE 1)
  else()
    set(${BASE_VAR}_DEFINE 0)
  endif()
endmacro()

# Use the current options to create the include definitions
set_define(ENABLE_DYMOLA)
set_define(ENABLE_OPEN_MODELICA)
set_define(ENABLE_FLUIDPROP)
set_define(ENABLE_COOLPROP)
set_define(BUILD_SHARED_LIBS)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Sources/include.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Sources/include.h @ONLY)


#######################################
#       PROJECT-WIDE SETTINGS         #
#######################################

# if (MSVC)
# #    # Force visual studio to statically link the c runtime to avoid dependency on MSVCRXXX.dll
# #    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
# #    set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
# #    set (CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /MT")
# #    set (CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} /MTd")
    # ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
# endif()
# ADD_DEFINITIONS (-DMODELICA_ERRORS=0 -DNOMINMAX)
# # Workaround for missing ModelicaMessage and ModelicaError objects
# set  (INCLUDE_DIRS "")


#######################################
#         TARGET DEFINITIONS          #
#######################################
file (GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.cpp")

if(NOT ENABLE_FLUIDPROP)
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/FluidProp_IF.cpp")
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/FluidProp_COM.cpp")
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/fluidpropsolver.cpp")
endif()

add_library (${LIBRARY_NAME} ${LIB_SOURCES})
set_property (TARGET ${LIBRARY_NAME} PROPERTY VERSION ${APP_VERSION})
target_include_directories(${LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Sources")

INSTALL(TARGETS "${LIBRARY_NAME}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/../Modelica/ExternalMedia ${APP_VERSION}/Resources/Library/${CMAKE_SYSTEM_NAME}" )
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/${LIBRARY_HEADER}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/../Modelica/ExternalMedia ${APP_VERSION}/Resources/Include" )


#######################################
#     EXTERNAL PROJECT SETTINGS       #
#######################################

### Use the temporary directory to build submodules / external projects
#if(WIN32)
#  set(TMPDIR "$ENV{TEMP}")
#else()
#  set(TMPDIR "$ENV{TMPDIR}")
#endif()
#if(NOT TMPDIR)
#  message(FATAL_ERROR "The temporary directory could not be identified.")
#endif()
#if(NOT EXISTS "${TMPDIR}")
#  message(FATAL_ERROR "The temporary directory \"${TMPDIR}\" does not exists.")
#endif()
#file(TO_CMAKE_PATH "${TMPDIR}" TMPDIR)

# Shorthand function to locate libraries
function(find_files BASE_PATH FILE_PATTERNS OUTPUT_VAR)
  list(APPEND _LIB_PATTERNS "*/${CMAKE_STATIC_LIBRARY_PREFIX}*${CMAKE_STATIC_LIBRARY_SUFFIX}")
  #list(APPEND _LIB_PATTERNS "*/${CMAKE_SHARED_LIBRARY_PREFIX}*${CMAKE_SHARED_LIBRARY_SUFFIX}")
  foreach(_PATTERN IN LISTS FILE_PATTERNS)
    file(GLOB_RECURSE _FOUND
      FOLLOW_SYMLINKS
      LIST_DIRECTORIES false
      "${BASE_PATH}/${_PATTERN}"
    )
    if(_FOUND)
      list(APPEND OUTPUT_VAR "${_FOUND}")
    endif()
  endforeach()
  set(OUTPUT_VAR ${OUTPUT_VAR} PARENT_SCOPE)
endfunction()

function(find_static_libraries BASE_PATH OUTPUT_VAR)
  list(APPEND _LIB_PATTERNS "*/${CMAKE_STATIC_LIBRARY_PREFIX}*${CMAKE_STATIC_LIBRARY_SUFFIX}")
  find_files(${BASE_PATH} ${_LIB_PATTERNS} ${OUTPUT_VAR})
  set(OUTPUT_VAR ${OUTPUT_VAR} PARENT_SCOPE)
endfunction()

function(find_shared_libraries BASE_PATH OUTPUT_VAR)
  list(APPEND _LIB_PATTERNS "*/${CMAKE_SHARED_LIBRARY_PREFIX}*${CMAKE_SHARED_LIBRARY_SUFFIX}")
  find_files(${BASE_PATH} ${_LIB_PATTERNS} ${OUTPUT_VAR})
  set(OUTPUT_VAR ${OUTPUT_VAR} PARENT_SCOPE)
endfunction()


#######################################
#     INCLUDE COOLPROP LIBRARY        #
#######################################

# This is really a mess, but we cannot do it differently since 
# Coolprop does not follow CMake standards and handles installation
# directories in a rather exotic way.

if(ENABLE_COOLPROP)

  # Download and build CoolProp as needed
  ExternalProject_Add(coolprop-download
    GIT_REPOSITORY    https://github.com/coolprop/coolprop.git
    GIT_TAG           v6.4.1
    SOURCE_DIR        "${CMAKE_BINARY_DIR}/coolprop-download-src"
    BINARY_DIR        "${CMAKE_BINARY_DIR}/coolprop-download-build"
    INSTALL_DIR       "${CMAKE_BINARY_DIR}/coolprop-download-prefix"
  #  INSTALL_COMMAND   ${CMAKE_COMMAND} -E copy_directory "${TMPDIR}/coolprop-src/install_root/" "${CMAKE_BINARY_DIR}/coolprop-download-prefix"
    CMAKE_ARGS ${COMMON_EXTERNAL_PROJECT_ARGS}
     -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
     -DCOOLPROP_STATIC_LIBRARY:BOOL=${ADD_COOLPROP_STATIC}
     -DCOOLPROP_SHARED_LIBRARY:BOOL=${ADD_COOLPROP_SHARED}
     -DCOOLPROP_DEBUG:BOOL=$<CONFIG:Debug>
     -DCOOLPROP_RELEASE:BOOL=$<CONFIG:Release>
     -DCOOLPROP_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  )

  ExternalProject_Get_Property(coolprop-download SOURCE_DIR)
  set(COOLPROP_SOURCE_DIR ${SOURCE_DIR})
  ExternalProject_Get_Property(coolprop-download BINARY_DIR)
  set(COOLPROP_BINARY_DIR ${BINARY_DIR})
  ExternalProject_Get_Property(coolprop-download INSTALL_DIR)
  set(COOLPROP_INSTALL_DIR ${INSTALL_DIR})

  add_custom_target(coolprop-clean
    #COMMAND ${CMAKE_COMMAND} -E remove_directory "${COOLPROP_SOURCE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${COOLPROP_BINARY_DIR}"
    COMMAND ${CMAKE_COMMAND} -E remove_directory "${COOLPROP_INSTALL_DIR}"
    COMMENT "Cleaning the generated files for the CoolProp builds"
  )

  if(CMAKE_CL_64)
    set(COOLPROP_BITNESS "64")
  else()
    set(COOLPROP_BITNESS "32")
  endif()
  set(COOLPROP_PATH_SUFFIX "${CMAKE_SYSTEM_NAME}/${COOLPROP_BITNESS}bit_${CMAKE_CXX_COMPILER_ID}_${CMAKE_CXX_COMPILER_VERSION}")

  if(ADD_COOLPROP_STATIC)
    set(COOLPROP_LIBRARY_PREFIX ${CMAKE_STATIC_LIBRARY_PREFIX})
    set(COOLPROP_LIBRARY_SUFFIX ${CMAKE_STATIC_LIBRARY_SUFFIX})
    set(COOLPROP_INSTALL_DIR ${COOLPROP_INSTALL_DIR}/static_library)
  else()
    set(COOLPROP_LIBRARY_PREFIX ${CMAKE_SHARED_LIBRARY_PREFIX})
    set(COOLPROP_LIBRARY_SUFFIX ${CMAKE_SHARED_LIBRARY_SUFFIX})
    set(COOLPROP_INSTALL_DIR ${COOLPROP_INSTALL_DIR}/shared_library)
  endif()

  set(COOLPROP_LIBRARY_NAME CoolProp)
  set(COOLPROP_LIBRARY_REL ${COOLPROP_LIBRARY_PREFIX}${COOLPROP_LIBRARY_NAME}${CMAKE_RELEASE_POSTFIX}${COOLPROP_LIBRARY_SUFFIX})
  set(COOLPROP_LIBRARY_DBG ${COOLPROP_LIBRARY_PREFIX}${COOLPROP_LIBRARY_NAME}${CMAKE_DEBUG_POSTFIX}${COOLPROP_LIBRARY_SUFFIX})

  if(ADD_COOLPROP_STATIC)
    add_library(CoolPropLib STATIC IMPORTED GLOBAL)
    ## Cannot use non-existing interface includes at configure time ...
    #target_include_directories(CoolPropLib INTERFACE ${COOLPROP_SOURCE_DIR})
    #target_include_directories(CoolPropLib INTERFACE ${COOLPROP_SOURCE_DIR}/include)
    #target_include_directories(CoolPropLib INTERFACE ${COOLPROP_SOURCE_DIR}/externals/msgpack-c/include)
    #target_include_directories(CoolPropLib INTERFACE ${COOLPROP_SOURCE_DIR}/externals/fmtlib)
    ## ... use direct project includes instead
    list(APPEND COOLPROP_INCLUDES "${COOLPROP_SOURCE_DIR}")
    list(APPEND COOLPROP_INCLUDES "${COOLPROP_SOURCE_DIR}/include")
    list(APPEND COOLPROP_INCLUDES "${COOLPROP_SOURCE_DIR}/externals/msgpack-c/include")
    list(APPEND COOLPROP_INCLUDES "${COOLPROP_SOURCE_DIR}/externals/fmtlib")
  elseif(ADD_COOLPROP_SHARED)
    add_library(CoolPropLib SHARED IMPORTED GLOBAL)
    ## Cannot use non-existing interface includes at configure time ...
    #target_include_directories(CoolPropLib INTERFACE ${COOLPROP_INSTALL_DIR})
    ## ... use direct project includes instead
    list(APPEND COOLPROP_INCLUDES "${COOLPROP_INSTALL_DIR}")
  endif()
  set_property(TARGET CoolPropLib PROPERTY IMPORTED_LOCATION_RELEASE ${COOLPROP_INSTALL_DIR}/${COOLPROP_PATH_SUFFIX}/${COOLPROP_LIBRARY_REL})
  set_property(TARGET CoolPropLib PROPERTY IMPORTED_LOCATION_DEBUG   ${COOLPROP_INSTALL_DIR}/${COOLPROP_PATH_SUFFIX}/${COOLPROP_LIBRARY_DBG})
  add_dependencies(CoolPropLib coolprop-download)

  target_link_libraries(${LIBRARY_NAME} PUBLIC CoolPropLib)
  target_include_directories(${LIBRARY_NAME} PUBLIC ${COOLPROP_INCLUDES})

  # Customise this part to use your own CPP files
  add_executable (coolprop_comparisons ${CMAKE_CURRENT_SOURCE_DIR}/Tests/coolprop_comparisons.cpp)
  target_link_libraries(coolprop_comparisons PRIVATE ${LIBRARY_NAME})
  #target_include_directories(coolprop_comparisons PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Sources)
  #target_link_libraries(coolprop_comparisons PRIVATE CoolPropLib)
  #target_include_directories(coolprop_comparisons PRIVATE ${COOLPROP_INCLUDES})

endif()
