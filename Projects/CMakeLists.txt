cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
# Was 3.8 for the generator expression CMake
# $<IF:$<BOOL:ON>,1,0>
# checked with https://github.com/nlohmann/cmake_min_version
# Now it is 3.9 for CheckIPOSupported.

CMAKE_POLICY(SET CMP0053 NEW)
CMAKE_POLICY(SET CMP0063 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0063 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0069 NEW)

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

include(CheckIPOSupported)
check_ipo_supported(RESULT is_ipo_supported OUTPUT lto_error LANGUAGES C CXX)
message(STATUS "is_ipo_supported = ${is_ipo_supported}")
if(is_ipo_supported)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()




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


# For installation of the shared library look in ExternalMediaLib/CMakeLists.txt
set(MODELICA_INSTALL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../Modelica/ExternalMedia/Resources")
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/ExternalMediaLib/include/" DESTINATION "${MODELICA_INSTALL_PATH}/Include")



add_subdirectory("ModelicaUtilitiesHelper")




#######################################
#       INCLUDE COOLPROP              #
#######################################

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

  # If the static CoolProp library is linked into a dynamic library don't export any symbols from coolprop!
  set(CMAKE_CXX_VISIBILITY_PRESET hidden)
  set(CMAKE_C_VISIBILITY_PRESET hidden)
  
  # SET(COOLPROP_SHARED_LIBRARY ON CACHE BOOL "Force the shared library")
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../externals/CoolProp" "${CMAKE_CURRENT_BINARY_DIR}/CoolProp")

  # reinstate the default:
  set(CMAKE_CXX_VISIBILITY_PRESET default)
  set(CMAKE_C_VISIBILITY_PRESET default)

endif()


#######################################
#         TARGET DEFINITIONS          #
#######################################

add_subdirectory(ExternalMediaLib)

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



#######################################
#          TEST DEFINITIONS           #
#######################################
# Customise this part to use your own CPP files
if (COOLPROP)
  add_executable (main EXCLUDE_FROM_ALL ${CMAKE_CURRENT_SOURCE_DIR}/Tests/coolprop_comparisons.cpp)
  target_link_libraries(main PRIVATE ${LIBRARY_NAME} CoolProp modelica_utilities_DONT_USE)
endif()
