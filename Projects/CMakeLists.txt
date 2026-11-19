cmake_minimum_required(VERSION 3.13 FATAL_ERROR) # Due to the use of target_link_options
include(ExternalProject)

CMAKE_POLICY(SET CMP0053 NEW)

#######################################
#         PROJECT INFORMATION         #
#######################################

# Oldest macOS version the binaries run on. Without this setting the
# compiler uses the macOS version of the build machine, so binaries built
# on a new Mac would refuse to load on older ones. 11.0 is the first
# version for Apple Silicon (arm64) and fully supports C++17. It has to be
# set before project(). Override it with -DCMAKE_OSX_DEPLOYMENT_TARGET=...
# or the MACOSX_DEPLOYMENT_TARGET environment variable.
if(NOT DEFINED CMAKE_OSX_DEPLOYMENT_TARGET AND NOT DEFINED ENV{MACOSX_DEPLOYMENT_TARGET})
  set(CMAKE_OSX_DEPLOYMENT_TARGET "11.0" CACHE STRING "Minimum macOS version")
endif()

# Project name
project ("ExternalMedia")

# CoolProp 8 and later require C++17, also for the headers included
# by ExternalMedia. Set it explicitly since compilers such as MSVC
# default to an older standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project version
set (VERSION_MAJOR 4)
set (VERSION_MINOR 1)
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
  set(COOLPROP_SOURCES "${CMAKE_BINARY_DIR}/CoolProp.sources")
  #file(REMOVE_RECURSE "${COOLPROP_SOURCES}")
  if(EXISTS "${COOLPROP_SOURCES}")
    message(STATUS "Using existing CoolProp sources from ${COOLPROP_SOURCES}")
  else()
    message(STATUS "Updating CoolProp sources in ${COOLPROP_SOURCES}")
    set(COOLPROP_SOURCES_GIT "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
    if(EXISTS "${COOLPROP_SOURCES_GIT}")
      message(STATUS "Using existing CoolProp sources from ${COOLPROP_SOURCES_GIT}")
      #file(COPY "${COOLPROP_SOURCES_GIT}" DESTINATION "${COOLPROP_SOURCES}")
      execute_process(
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${COOLPROP_SOURCES_GIT}" "${COOLPROP_SOURCES}"
      )
    else()
      # Clone the release tag with git instead of downloading a zip archive.
      # CoolProp 8 or later is required, see the check below.
      set(COOLPROP_GIT_TAG "v8.0.0")
      message(STATUS "Cloning CoolProp ${COOLPROP_GIT_TAG} into ${COOLPROP_SOURCES}")
      find_package(Git REQUIRED)
      execute_process(
        COMMAND "${GIT_EXECUTABLE}" clone --recursive --depth 1 --shallow-submodules --branch "${COOLPROP_GIT_TAG}" https://github.com/CoolProp/CoolProp.git "${COOLPROP_SOURCES}"
        RESULT_VARIABLE COOLPROP_CLONE_RESULT
      )
      if(NOT COOLPROP_CLONE_RESULT EQUAL 0)
        file(REMOVE_RECURSE "${COOLPROP_SOURCES}")
        message(FATAL_ERROR "Could not clone CoolProp ${COOLPROP_GIT_TAG}. Clone it into externals/CoolProp.git yourself, see README_compilation.md.")
      endif()
    endif()
  endif()
  # The CoolProp solver needs CoolProp 8 or later (C++17 headers and the
  # SVDSBTL backend). Reject older sources, e.g. a CoolProp.sources folder
  # left over from an earlier build, instead of failing later in the compiler.
  set(COOLPROP_CONFIG_KEYS "${COOLPROP_SOURCES}/include/CoolProp/detail/configuration_keys.h")
  if(EXISTS "${COOLPROP_CONFIG_KEYS}")
    file(STRINGS "${COOLPROP_CONFIG_KEYS}" COOLPROP_SVDSBTL_KEY REGEX "SVDSBTL_SAMPLING_THREADS")
  else()
    set(COOLPROP_SVDSBTL_KEY "")
  endif()
  if(NOT COOLPROP_SVDSBTL_KEY)
    message(FATAL_ERROR "The CoolProp sources in ${COOLPROP_SOURCES} are older than CoolProp 8.0.0. "
      "Delete that folder and update externals/CoolProp.git (or remove it to clone CoolProp 8.0.0), then configure again.")
  endif()
  # Prepare the CoolProp build, "${COOLPROP_SOURCES}" is ready now
  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")
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
  ADD_SUBDIRECTORY ("${COOLPROP_SOURCES}" "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")
  list (APPEND INCLUDE_DIRS "${COOLPROP_SOURCES}")
  list (APPEND INCLUDE_DIRS "${COOLPROP_SOURCES}/include")
  list (APPEND INCLUDE_DIRS "${COOLPROP_SOURCES}/externals/msgpack-c/include")
  # Handle different CoolProp versions
  if(EXISTS "${COOLPROP_SOURCES}/externals/cppformat")
    list (APPEND INCLUDE_DIRS "${COOLPROP_SOURCES}/externals/cppformat")
  endif()
  if(EXISTS "${COOLPROP_SOURCES}/externals/fmtlib/include")
    list (APPEND INCLUDE_DIRS "${COOLPROP_SOURCES}/externals/fmtlib/include")
  endif()
  # CoolProp 8 and later download their dependencies (fmt, Eigen, ...)
  # at configure time, reuse the include directories CoolProp uses.
  get_directory_property(COOLPROP_INCLUDE_DIRS DIRECTORY "${COOLPROP_SOURCES}" INCLUDE_DIRECTORIES)
  list (APPEND INCLUDE_DIRS ${COOLPROP_INCLUDE_DIRS})
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
  # Platform name is not standardized
  # https://openmodelica.org/forum/default-topic/583-openmodelica-1-8-1-fmu-export-fails-under-mac-os-x
  list(LENGTH CMAKE_OSX_ARCHITECTURES NUMBER_OF_OSX_ARCHITECTURES)
  if (NUMBER_OF_OSX_ARCHITECTURES EQUAL 0)
    if (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "x86_64")
      set(MODELICA_PLATFORM "darwin64")
    elseif (CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL "arm64")
      set(MODELICA_PLATFORM "aarch64-darwin")
    else()
      message(FATAL_ERROR "Unknown macOS architecture CMAKE_HOST_SYSTEM_PROCESSOR=${CMAKE_HOST_SYSTEM_PROCESSOR}.")
    endif()
  elseif(NUMBER_OF_OSX_ARCHITECTURES EQUAL 1)
    if (CMAKE_OSX_ARCHITECTURES STREQUAL "x86_64")
      set(MODELICA_PLATFORM "darwin64")
    elseif (CMAKE_OSX_ARCHITECTURES STREQUAL "arm64")
      set(MODELICA_PLATFORM "aarch64-darwin")
    else()
      message(FATAL_ERROR "Unknown macOS architecture CMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}.")
    endif()
  else()
    message(FATAL_ERROR "Universal builds not supported CMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}.")
  endif()
  # Defer resolving ModelicaUtilities references at runtime
  set(CMAKE_SHARED_LINKER_FLAGS "-undefined dynamic_lookup")
else()
  message(FATAL_ERROR "Could not determine the MODELICA_PLATFORM, aborting.")
endif()

if(MSVC) #CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  # Ensure UTF-8 encoding for MSVC compiler
  foreach(flag_var CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
                    CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    if(NOT ${flag_var} MATCHES "/utf-8")
      set(${flag_var} "${${flag_var}} /utf-8")
    endif()
  endforeach()
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
  elseif(MSVC_TOOLSET_VERSION EQUAL 145)
    set(MODELICA_COMPILER vs2026)
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
  # MinGW builds are meant for OpenModelica, which does not use the compiler
  # subfolder. OpenModelica 1.23 and later (MSYS2 UCRT64) search the folders
  # Library/ucrt64, Library/mingw64, Library/win64 and Library, in that order.
  # A UCRT64 build goes to Library/ucrt64, which no other Modelica tool reads.
  # The compiler defines _UCRT when it targets the UCRT runtime.
  include(CheckCXXSourceCompiles)
  check_cxx_source_compiles("#include <cstdio>
#ifndef _UCRT
#error not UCRT
#endif
int main() { return 0; }" MINGW_USES_UCRT)
  if(MINGW_USES_UCRT AND CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_PLATFORM "ucrt64")
  endif()
  set(MODELICA_COMPILER "")
  message(STATUS "MinGW build, installing for OpenModelica in Library/${MODELICA_PLATFORM}")
  # Link the GCC runtime statically for all MinGW builds. Otherwise the DLL uses
  # the libstdc++-6.dll that the Modelica tool has loaded, which can come from
  # another MSYS2 environment (e.g. UCRT64) and is not compatible. For g++,
  # -static implies -static-libgcc and -static-libstdc++, and it also links
  # winpthread statically, which the other two flags do not. -s strips the
  # symbol table and debug information from the DLL.
  target_link_options(${LIBRARY_NAME} PRIVATE -static -s)
endif()

# The platform and compiler names above are chosen automatically. They can
# be overridden on the command line, e.g.
#   -DMODELICA_PLATFORM:STRING=win64 -DMODELICA_COMPILER:STRING=
# A value given with -D is stored in the CMake cache, while the automatic
# choice above is a normal variable that hides the cached value. Hence we
# copy the cached value back here if the user provided one.
foreach(_var MODELICA_PLATFORM MODELICA_COMPILER)
  get_property(_user_set CACHE ${_var} PROPERTY TYPE SET)
  if(_user_set)
    get_property(_user_value CACHE ${_var} PROPERTY VALUE)
    set(${_var} "${_user_value}")
    message(STATUS "${_var} set by the user: \"${_user_value}\"")
  endif()
endforeach()

set(MODELICA_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Modelica/ExternalMedia/Resources")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/${LIBRARY_HEADER}" DESTINATION "${MODELICA_INSTALL_PATH}/Include")
message(STATUS "Installation path: ${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}/${MODELICA_COMPILER}")
INSTALL(TARGETS "${LIBRARY_NAME}" DESTINATION "${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}/${MODELICA_COMPILER}")
if(NOT WIN32)
    # Both OpenModelica and Dymola on Linux DO NOT expect a compiler-specific subdirectory
    # NB: This means that subsequent builds for different compilers overwrite the previously installed default binaries.
    message(STATUS "Additional installation path: ${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}")
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
