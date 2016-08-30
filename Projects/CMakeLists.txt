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
set  (INCLUDE_DIRS "")


#######################################
#       INCLUDE COOLPROP              #
#######################################
SET(COOLPROP_STATIC_LIBRARY OFF CACHE BOOL "Force the object library")
SET(COOLPROP_SHARED_LIBRARY OFF CACHE BOOL "Force the object library")
SET(COOLPROP_OBJECT_LIBRARY ON  CACHE BOOL "Force the object library")
ADD_SUBDIRECTORY ("${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git" "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")
list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git")
list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/include")
list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/msgpack-c/include")
list (APPEND INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp.git/externals/cppformat")

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
add_dependencies (${LIBRARY_NAME} CoolProp)
INSTALL(TARGETS "${LIBRARY_NAME}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/../Modelica/ExternalMedia ${APP_VERSION}/Resources/Library/${CMAKE_SYSTEM_NAME}" )
INSTALL(FILES "${CMAKE_CURRENT_SOURCE_DIR}/Sources/${LIBRARY_HEADER}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/../Modelica/ExternalMedia ${APP_VERSION}/Resources/Include" )


#######################################
#          TEST DEFINITIONS           #
#######################################
# Customise this part to use your own CPP files
add_executable (main ${CMAKE_CURRENT_SOURCE_DIR}/Tests/coolprop_comparisons.cpp ${LIB_SOURCES})
add_dependencies (main CoolProp)

