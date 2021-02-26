#!/bin/bash

#REM   ******************* README *********************************
#REM    You might have to adjust some settings manually. This file 
#REM    requires that you know the name of your CMake generator, if
#REM    you have more than one development environment installed.
#REM   ******************* README *********************************

if [ "$(uname)" == "Darwin" ]; then
    echo "Mac OS X is untested..."
    CMake_generator="Unix Makefiles"
    exit 1
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    CMake_generator="Unix Makefiles"
#elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
#    CMake_generator="MinGW Makefiles"
else
    CMake_generator="MSYS Makefiles"
fi

BUILD_DIR="build"
if [ ! -d "$BUILD_DIR" ]; then mkdir -p "$BUILD_DIR"; fi
BUILD_DIR=`readlink -f "$BUILD_DIR"`

echo " "
echo "********* Detecting supported property libraries ***********"
FLUIDP=1
COOLP=1
echo "FluidProp support set to: $FLUIDP"
echo  "CoolProp support set to: $COOLP"

pushd "$BUILD_DIR"
cmake .. -G "${CMake_generator}" -DFLUIDP=${FLUIDP} -DCOOLP=${COOLP}
#
cmake --build . --config Release --target install
popd 

exit 0
