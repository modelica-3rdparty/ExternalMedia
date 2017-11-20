#!/bin/bash

#REM   ******************* README *********************************
#REM    You might have to adjust some settings manually. This file 
#REM    requires that you know the name of your CMake generator, if
#REM    you have more than one development environment installed.
#REM   ******************* README *********************************

#if [ "$(uname)" == "Darwin" ]; then
#    echo "Mac OS X is untested..."
#    CMake_generator="Unix Makefiles"
#    exit 1
#elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
#    CMake_generator="Unix Makefiles"
#elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
#    CMake_generator="MinGW Makefiles"
#fi

EXTERNALS="../externals"
if [ ! -d "$EXTERNALS" ]; then mkdir -p "$EXTERNALS"; fi
EXTERNALS=`readlink -f "$EXTERNALS"`
CP_SRC="${EXTERNALS}/CoolProp.git"

BUILD_DIR="build"
if [ ! -d "$BUILD_DIR" ]; then mkdir -p "$BUILD_DIR"; fi
BUILD_DIR=`readlink -f "$BUILD_DIR"`

echo " "
echo "********* Detecting supported property libraries ***********"
FLUIDP=1
COOLP=1
echo "FluidProp support set to: $FLUIDP"
echo  "CoolProp support set to: $COOLP"

if [ "$COOLP" == "1" ]; then
  echo "******************** CoolProp ******************************"
  echo "Sources are located in $CP_SRC"
  if [ -d "$CP_SRC" ]; then
    pushd "$CP_SRC"
    git pull origin master
    git submodule init
    git submodule update
    # git submodule foreach git pull origin master
    popd 
  else
    git clone --recursive https://github.com/CoolProp/CoolProp.git "$CP_SRC"
  fi
fi

pushd "$BUILD_DIR"
# cmake .. -G "%CMake_generator%" 
cmake .. 
cmake --build . --config Release --target install
popd 

exit 0
