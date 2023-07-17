cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
# For the generator expression CMake
# $<IF:$<BOOL:ON>,1,0>
#checked with https://github.com/nlohmann/cmake_min_version

CMAKE_POLICY(SET CMP0053 NEW)

#######################################
#         PROJECT INFORMATION         #
#######################################

# PROJECT_VERSION
# PROJECT_VERSION_MAJOR
# PROJECT_VERSION_MINOR
# PROJECT_VERSION_PATCH


set(VERSION_MAJOR 3)
set(VERSION_MINOR 3)
set(VERSION_PATCH 2)
set(APP_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message (STATUS "ExternalMedia library version: ${APP_VERSION}")
set (LIBRARY_NAME "ExternalMediaLib")
set (LIBRARY_HEADER "externalmedialib.h")

project ("ExternalMedia" LANGUAGES C CXX VERSION ${APP_VERSION})


if(APPLE)
  # Latest version of coolprop only compiles with a modern C++ standard,
  # and MacOS does not do that by default
  set(CMAKE_CXX_STANDARD 14)
endif()


#######################################
#       PROJECT-WIDE SETTINGS         #
#######################################
if (MSVC)
#    # Force visual studio to statically link the c runtime to avoid dependency on MSVCRXXX.dll
#    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
#    set (CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
#    set (CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE} /MT")
#    set (CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG} /MTd")
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-DNOMINMAX) # Avoid name clash in CoolProp FMT library
endif()
#ADD_DEFINITIONS (-DMODELICA_ERRORS=0 -DNOMINMAX)
# Workaround for missing ModelicaMessage and ModelicaError objects
set(INCLUDE_DIRS "")

option(COOLPROP "Include the CoolProp library for fluid properties" ON)
option(FLUIDPROP "Include the FluidProp library for fluid properties" OFF)

#######################################
#       INCLUDE COOLPROP              #
#######################################


add_subdirectory("ModelicaUtilitiesHelper")

if(COOLPROP)
  
  # Force bitness for MinGW
  if(MINGW)
    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET(FORCE_BITNESS_64 ON CACHE BOOL "force 64 bit")
    else()
      SET(FORCE_BITNESS_32 ON CACHE BOOL "force 32 bit")
    endif()
  endif()

  # Configure CMake switches for CoolProp
  # Enable this for shared libraries (either CoolProp itself or ExternalMedia)
  SET(COOLPROP_FPIC ON CACHE BOOL "force -fPIC switch")
  SET(COOLPROP_STATIC_LIBRARY ON CACHE BOOL "Force the static library")
  # SET(COOLPROP_SHARED_LIBRARY ON CACHE BOOL "Force the shared library")

  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp" "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")

endif()


#######################################
#         TARGET DEFINITIONS          #
#######################################
list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Sources")
include_directories (${INCLUDE_DIRS})
file (GLOB_RECURSE LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/*.cpp")

if(NOT FLUIDPROP)
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/FluidProp_IF.cpp")
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/FluidProp_COM.cpp")
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/fluidpropsolver.cpp")
endif()


# Add the target for ExternalMedia
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
if(COOLPROP)
  target_link_libraries(${LIBRARY_NAME} PUBLIC CoolProp)
  # remove unneeded coolprop symbols from the library api to guard against
  # name-clashes. MSVC should by default only export symbols that are explicitly
  # declared for export via declspec. This only works for linkers that support
  # "--exclude-libs", which ld and lld do. But all other linkers are not treated.
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(coolprop_libname "${CMAKE_STATIC_LIBRARY_PREFIX}CoolProp${CMAKE_STATIC_LIBRARY_SUFFIX}")
    target_link_options(${LIBRARY_NAME} PRIVATE "-Wl,--exclude-libs=ALL")
  endif()
else()
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/coolpropsolver.cpp")
endif()


set_property(TARGET ${LIBRARY_NAME} PROPERTY VERSION ${APP_VERSION})
set_property(TARGET ${LIBRARY_NAME} PROPERTY DEBUG_POSTFIX "")
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_FLUIDPROP=$<IF:$<BOOL:${FLUIDPROP}>,1,0>)
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_COOLPROP=$<IF:$<BOOL:${COOLPROP}>,1,0>)
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_EXPORTS) # Use this for a shared library
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_LIBRARY_EXPORTS=1) # Use this for a shared library


if(WIN32)
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_PLATFORM "win64")
  else()
    set(MODELICA_PLATFORM "win32")
  endif()
elseif(UNIX AND NOT APPLE) # Important! UNIX is set both on Linux and MacOS!
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_PLATFORM "linux64")
  else()
    set(MODELICA_PLATFORM "linux32")
  endif()
elseif(APPLE)
  # Platform name is not standardized, for now we stick with darwin but untested
  # https://openmodelica.org/forum/default-topic/583-openmodelica-1-8-1-fmu-export-fails-under-mac-os-x
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_PLATFORM "darwin64")
  else()
    set(MODELICA_PLATFORM "darwin32")
  endif()
  # Defer resolving ModelicaUtilities references at runtime
  set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
else()
  message(FATAL_ERROR "Could not determine the MODELICA_PLATFORM, aborting.")
endif()

if(MSVC) #CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  if    (MSVC_TOOLSET_VERSION EQUAL 80 )
    set(MODELICA_COMPILER vs2005)
  elseif(MSVC_TOOLSET_VERSION EQUAL 90 )
    set(MODELICA_COMPILER vs2008)
  elseif(MSVC_TOOLSET_VERSION EQUAL 100)
    set(MODELICA_COMPILER vs2010)
  elseif(MSVC_TOOLSET_VERSION EQUAL 110)
    set(MODELICA_COMPILER vs2012)
  elseif(MSVC_TOOLSET_VERSION EQUAL 120)
    set(MODELICA_COMPILER vs2013)
  elseif(MSVC_TOOLSET_VERSION EQUAL 140)
    set(MODELICA_COMPILER vs2015)
  elseif(MSVC_TOOLSET_VERSION EQUAL 141)
    set(MODELICA_COMPILER vs2017)
  elseif(MSVC_TOOLSET_VERSION EQUAL 142)
    set(MODELICA_COMPILER vs2019)
  elseif(MSVC_TOOLSET_VERSION EQUAL 143)
    set(MODELICA_COMPILER vs2022)
  else()
    message(WARNING "\"${MSVC_TOOLSET_VERSION}\" is not explicitly supported. Please check you build carefully.")
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  string(REPLACE "." ";" _VERSION_LIST ${CMAKE_CXX_COMPILER_VERSION})
  list(GET _VERSION_LIST 0 _VERSION_MAJOR)
  list(GET _VERSION_LIST 1 _VERSION_MINOR)
  set(MODELICA_COMPILER gcc${_VERSION_MAJOR}${_VERSION_MINOR})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  string(REPLACE "." ";" _VERSION_LIST ${CMAKE_CXX_COMPILER_VERSION})
  list(GET _VERSION_LIST 0 _VERSION_MAJOR)
  list(GET _VERSION_LIST 1 _VERSION_MINOR)
  set(MODELICA_COMPILER clang${_VERSION_MAJOR}${_VERSION_MINOR})
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  string(REPLACE "." ";" _VERSION_LIST ${CMAKE_CXX_COMPILER_VERSION})
  list(GET _VERSION_LIST 0 _VERSION_MAJOR)
  list(GET _VERSION_LIST 1 _VERSION_MINOR)
  set(MODELICA_COMPILER appleclang${_VERSION_MAJOR}${_VERSION_MINOR})
else()
  message(WARNING "\"${CMAKE_CXX_COMPILER_ID}\" is not explicitly supported. Please check you build carefully.")
endif()

if(MINGW)
  # It looks like we need a global setting
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
  #set_property(TARGET ${LIBRARY_NAME} PROPERTY SHARED_LIBRARY_PREFIX "")
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_COMPILER "mingw64")
  else()
    set(MODELICA_COMPILER "mingw32")
  endif()
  set(MODELICA_COMPILER "")
  message(WARNING "Discarded compiler information since OpenModelica expect the mingw32/64 platform.")
endif()

set(MODELICA_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Modelica/ExternalMedia/Resources")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/${LIBRARY_HEADER}" DESTINATION "${MODELICA_INSTALL_PATH}/Include")
if(WIN32)
    message(STATUS "Installation path: ${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}/${MODELICA_COMPILER}")
    INSTALL(TARGETS "${LIBRARY_NAME}" DESTINATION "${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}/${MODELICA_COMPILER}")
else()
    # Both OpenModelica and Dymola on Linux DO NOT expect a compiler-specific subdirectory
    # NB: This means that subsequent builds for different compilers overwrite the previously installed binaries.
    message(STATUS "Installation path: ${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}")
    INSTALL(TARGETS "${LIBRARY_NAME}" DESTINATION "${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}")
endif()


#######################################
#          TEST DEFINITIONS           #
#######################################
# Customise this part to use your own CPP files
if (COOLPROP)
  add_executable (main EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/Tests/coolprop_comparisons.cpp)
  target_link_libraries(main PRIVATE CoolProp ${LIBRARY_NAME} modelica_utilities_DONT_USE)
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_link_options(main PRIVATE "-Wl,--exclude-libs=All")
  endif()
endif()
