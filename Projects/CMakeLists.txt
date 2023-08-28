cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
include(ExternalProject)

CMAKE_POLICY(SET CMP0053 NEW)

#######################################
#         PROJECT INFORMATION         #
#######################################

# Project name
project ("ExternalMedia")

if(APPLE)
  # Latest version of coolprop only compiles with a modern C++ standard,
  # and MacOS does not do that by default
  set(CMAKE_CXX_STANDARD 14)
endif()

# Project version
set (VERSION_MAJOR 3)
set (VERSION_MINOR 3)
set (VERSION_PATCH 2)
set (APP_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
message (STATUS "ExternalMedia library version: ${APP_VERSION}")
set (LIBRARY_NAME "ExternalMediaLib")
set (LIBRARY_HEADER "externalmedialib.h")


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

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../externals")
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../externals")
endif()

option(COOLPROP "Include the CoolProp library for fluid properties" ON)
option(FLUIDPROP "Include the FluidProp library for fluid properties" OFF)

#######################################
#       INCLUDE COOLPROP              #
#######################################

if(COOLPROP)
  # Get the CoolProp code
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/coolprop.zip")
      file(DOWNLOAD https://sourceforge.net/projects/coolprop/files/CoolProp/6.5.0/source/CoolProp_sources.zip/download "${CMAKE_BINARY_DIR}/coolprop.zip")
      #file(DOWNLOAD https://sourceforge.net/projects/coolprop/files/CoolProp/nightly/source/CoolProp_sources.zip/download "${CMAKE_BINARY_DIR}/coolprop.zip")
    endif()
    #file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/coolprop-tmp")
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/source")
    # Use CMake to unzip
    #file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/coolprop-tmp")
    #execute_process(
    #  COMMAND ${CMAKE_COMMAND} -E tar xjf "${CMAKE_BINARY_DIR}/coolprop.zip"
    #  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/coolprop-tmp"
    #)
    file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/coolprop.zip)
    #execute_process(COMMAND unzip ${CMAKE_BINARY_DIR}/coolprop.zip -d "${CMAKE_BINARY_DIR}/coolprop-tmp")
    #execute_process(COMMAND sed -i '421s;^;\#;' "${CMAKE_BINARY_DIR}/coolprop-tmp/CoolProp.sources/CMakeLists.txt")
    #file(RENAME "${CMAKE_BINARY_DIR}/coolprop-tmp/CoolProp.sources" "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
    file(RENAME "${CMAKE_BINARY_DIR}/source" "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
    #file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/coolprop-tmp")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")
  endif()
  # Configure CMake switches for CoolProp
  SET(COOLPROP_STATIC_LIBRARY OFF CACHE BOOL "Force the object library")
  SET(COOLPROP_SHARED_LIBRARY OFF CACHE BOOL "Force the object library")
  SET(COOLPROP_OBJECT_LIBRARY ON  CACHE BOOL "Force the object library")
  # Force bitness for MinGW
  if(MINGW)
    if(CMAKE_SIZEOF_VOID_P MATCHES "8")
      SET(FORCE_BITNESS_64 ON CACHE BOOL "force 64 bit")
    else()
      SET(FORCE_BITNESS_32 ON CACHE BOOL "force 32 bit")
    endif()
  endif()
  # Enable this for shared libraries (either CoolProp itself or ExternalMedia)
  SET(COOLPROP_FPIC ON CACHE BOOL "force -fPIC switch")
  # Add the CoolProp sources
  ADD_SUBDIRECTORY ("${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git" "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")
  list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
  list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/include")
  list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/msgpack-c/include")
  # Handle different CoolProp versions
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/cppformat")
    list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/cppformat")
  endif()
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/fmtlib/include")
    list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/fmtlib/include")
  endif()
endif()

## We use CMake to handle the dependency since the primary VCS for 
## ExternalMedia still is SVN.
#ExternalProject_Add(
#  CoolProp
#
#  GIT_REPOSITORY "https://github.com/CoolProp/CoolProp.git"
#  GIT_TAG "master"
#  
#  #UPDATE_COMMAND ""
#  #PATCH_COMMAND ""
#  
#  SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/externals/CoolProp.git"
#  CMAKE_ARGS -DCOOLPROP_OBJECT_LIBRARY=ON -DCOOLPROP_SHARED_LIBRARY=OFF -DCOOLPROP_STATIC_LIBRARY=OFF
#  
#  TEST_COMMAND ""
#)
#
#set(CoolProp_INCLUDE_DIRS "")
#list (APPEND CoolProp_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/externals/CoolProp.git/include")
#list (APPEND CoolProp_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/externals/CoolProp.git/externals/msgpack-c/include")
##set(CoolProp_OBJECTS "${CMAKE_SHARED_LIBRARY_PREFIX}humblelogging${CMAKE_SHARED_LIBRARY_SUFFIX}")
#set(CoolProp_OBJECTS $<TARGET_OBJECTS:CoolProp>)
#foreach (_dir ${CoolProp_INCLUDE_DIRS})
#  list (APPEND INCLUDE_DIRS ${_dir})
#endforeach()


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

if(COOLPROP)
  list(APPEND LIB_SOURCES $<TARGET_OBJECTS:CoolProp>)
else()
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/coolpropsolver.cpp")
endif()

# Add the target for ExternalMedia
add_library(${LIBRARY_NAME} SHARED ${LIB_SOURCES})
set_property(TARGET ${LIBRARY_NAME} PROPERTY VERSION ${APP_VERSION})
set_property(TARGET ${LIBRARY_NAME} PROPERTY DEBUG_POSTFIX "")
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_FLUIDPROP=$<IF:$<BOOL:${FLUIDPROP}>,1,0>)
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_COOLPROP=$<IF:$<BOOL:${COOLPROP}>,1,0>)
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_EXPORTS) # Use this for a shared library
target_compile_definitions(${LIBRARY_NAME} PRIVATE EXTERNALMEDIA_LIBRARY_EXPORTS=1) # Use this for a shared library

if(COOLPROP)
add_dependencies(${LIBRARY_NAME} CoolProp)
endif()

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
  add_executable (main EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/Tests/coolprop_comparisons.cpp ${LIB_SOURCES})
  add_dependencies (main CoolProp)
endif()
