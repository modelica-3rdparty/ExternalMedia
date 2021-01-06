cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
include(ExternalProject)

CMAKE_POLICY(SET CMP0053 OLD)

#######################################
#         PROJECT INFORMATION         #
#######################################

# Project name
project ("ExternalMedia")

# Project version
set (VERSION_MAJOR 3)
set (VERSION_MINOR 2)
set (VERSION_PATCH 1)
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
    ADD_DEFINITIONS (-D_CRT_SECURE_NO_WARNINGS)
endif()
ADD_DEFINITIONS (-DMODELICA_ERRORS=0 -DNOMINMAX)
# Workaround for missing ModelicaMessage and ModelicaError objects
set(INCLUDE_DIRS "")


#######################################
#       INCLUDE COOLPROP              #
#######################################

if(COOLP)
  if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
    file(DOWNLOAD https://sourceforge.net/projects/coolprop/files/CoolProp/5.1.1/source/CoolProp_sources.zip/download ${CMAKE_BINARY_DIR}/coolprop.zip)
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/coolprop-tmp")
    #file(ARCHIVE_EXTRACT INPUT ${CMAKE_BINARY_DIR}/coolprop.zip)
    execute_process(COMMAND unzip ${CMAKE_BINARY_DIR}/coolprop.zip -d "${CMAKE_BINARY_DIR}/coolprop-tmp")
    #execute_process(COMMAND sed -i '421s;^;\#;' "${CMAKE_BINARY_DIR}/coolprop-tmp/CoolProp.sources/CMakeLists.txt")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../externals")
    file(RENAME "${CMAKE_BINARY_DIR}/coolprop-tmp/CoolProp.sources" "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
    file(REMOVE_RECURSE "${CMAKE_BINARY_DIR}/coolprop-tmp")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")
  endif()
  
  SET(COOLPROP_STATIC_LIBRARY OFF CACHE BOOL "Force the object library")
  SET(COOLPROP_SHARED_LIBRARY OFF CACHE BOOL "Force the object library")
  SET(COOLPROP_OBJECT_LIBRARY ON  CACHE BOOL "Force the object library")
  ADD_SUBDIRECTORY ("${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git" "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")
  list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
  list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/include")
  list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/msgpack-c/include")
  list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/cppformat")
else()
  list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/coolpropsolver.cpp")
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
if (NOT FLUIDP)
    list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/FluidProp_IF.cpp")
    list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/FluidProp_COM.cpp")
    list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/fluidpropsolver.cpp")
endif()
LIST (APPEND LIB_SOURCES $<TARGET_OBJECTS:CoolProp>)

add_library (${LIBRARY_NAME} STATIC ${LIB_SOURCES})
set_property (TARGET ${LIBRARY_NAME} PROPERTY VERSION ${APP_VERSION})
if(COOLP)
add_dependencies (${LIBRARY_NAME} CoolProp)
endif()

if(WIN32)
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_PLATFORM "win64")
  else()
    set(MODELICA_PLATFORM "win32")
  endif()
elseif(APPLE)
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_PLATFORM "linux64")
  else()
    set(MODELICA_PLATFORM "linux32")
  endif()
elseif(UNIX)
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_PLATFORM "linux64")
  else()
    set(MODELICA_PLATFORM "linux32")
  endif()
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
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
    set(MODELICA_PLATFORM "mingw64")
  else()
    set(MODELICA_PLATFORM "mingw32")
  endif()
  set(MODELICA_COMPILER "")
  message(WARNING "Discarded compiler information since OpenModelica expect the mingw32/64 platform.")
endif()

set(MODELICA_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Modelica/ExternalMedia ${APP_VERSION}/Resources")

message(STATUS "Installation path: ${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}/${MODELICA_COMPILER}")

INSTALL(TARGETS "${LIBRARY_NAME}"                                     DESTINATION "${MODELICA_INSTALL_PATH}/Library/${MODELICA_PLATFORM}/${MODELICA_COMPILER}")
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/${LIBRARY_HEADER}" DESTINATION "${MODELICA_INSTALL_PATH}/Include")


#######################################
#          TEST DEFINITIONS           #
#######################################
# Customise this part to use your own CPP files
if (COOLP)
add_executable (main EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/Tests/coolprop_comparisons.cpp ${LIB_SOURCES})
add_dependencies (main CoolProp)
endif()
