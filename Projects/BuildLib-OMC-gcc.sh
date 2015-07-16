#!/bin/bash

# -DCOOLPROP=1 enables the use of CoolProp, which defaults to being disabled.
# Use -DCOOLPROP=0 to compile ExternalMedia wrapper without CoolProp support
# The C++ files for CoolProp will still be compiled

if [ "$(uname)" == "Darwin" ]; then
    echo "Mac OS X is untested..."
    SYSDIR="mac64"
    CP_COMP="Unix Makefiles"
    exit 1
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    SYSDIR="linux32"
    CP_COMP="Unix Makefiles"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    SYSDIR="win32"
    CP_COMP="MinGW Makefiles"
fi

GCC_OPTS="-O2 -loleaut32 -DCOOLPROP=1"

EXTERNALS=`readlink -f "../externals"`
CP_SRC="${EXTERNALS}/coolprop.git"
CP_TMP="${EXTERNALS}/coolprop.build.gcc"

CP_INC=${CP_SRC}/include
INCLUDES="-I${CP_INC}"

# echo "Getting coolpropsolver.cpp from CoolProp"
# cp ${CP}/wrappers/Modelica/src/coolpropsolver.cpp Sources
# cp ${CP}/wrappers/Modelica/src/coolpropsolver.h Sources

echo "Compiling sources"
#  ****** compile all the OpenModelica files ************
gcc ${GCC_OPTS} -c ${INCLUDES} Sources/*.cpp
#  ********** CoolProp sources *********
if [ -d "$CP_SRC" ]; then 
    pushd "$CP_SRC"
    git pull
    popd
else
    git clone --recursive https://github.com/CoolProp/CoolProp.git "$CP_SRC"
fi
mkdir -p "$CP_TMP"
pushd "$CP_TMP"
cmake "$CP_SRC" -DCOOLPROP_OBJECT_LIBRARY=ON -DCOOLPROP_RELEASE=ON -G "$CP_COMP" 
cmake --build . --config Release
popd
cp "$CP_TMP/CoolProp.dir/Release/"*.o . 

ar -r libExternalMediaLib.a *.o
ranlib libExternalMediaLib.a

echo "Copying files to External Media 3.2.1"
cp libExternalMediaLib.a "../Modelica/ExternalMedia 3.2.1/Resources/Library/$SYSDIR/libExternalMediaLib.OMC-gcc.a"
cp libExternalMediaLib.a "../Modelica/ExternalMedia 3.2.1/Resources/Library/$SYSDIR/libExternalMediaLib.a"
rm libExternalMediaLib.a
rm *.o
cp Sources/externalmedialib.h "../Modelica/ExternalMedia 3.2.1/Resources/Include"
echo "All done"