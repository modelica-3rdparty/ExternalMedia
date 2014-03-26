#!/bin/bash

# -DCOOLPROP=1 enables the use of CoolProp, which defaults to being disabled.
# Use -DCOOLPROP=0 to compile ExternalMedia wrapper without coolprop support
# The C++ files for CoolProp will still be compiled

GCC_OPTS="-O2 -loleaut32 -DCOOLPROP=1"
CP=../externals/coolprop/trunk
CPinc=${CP}/CoolProp
INCLUDES="-I${CPinc}"

# echo "Getting coolpropsolver.cpp from CoolProp"
# cp ${CP}/wrappers/Modelica/src/coolpropsolver.cpp Sources
# cp ${CP}/wrappers/Modelica/src/coolpropsolver.h Sources

echo "Compiling sources"
#  ****** compile all the OpenModelica files ************
gcc ${GCC_OPTS} -c ${INCLUDES} Sources/*.cpp
#  ********** CoolProp sources *********
gcc ${GCC_OPTS} -c ${INCLUDES} ${CP}/CoolProp/*.cpp

ar -r libExternalMediaLib.a *.o
ranlib libExternalMediaLib.a

echo "Copying files to External Media 3.2.1"
cp libExternalMediaLib.a "../Modelica/ExternalMedia 3.2.1/Resources/Library/linux32/libExternalMediaLib.OMC-gcc.a"
cp libExternalMediaLib.a "../Modelica/ExternalMedia 3.2.1/Resources/Library/linux32/libExternalMediaLib.a"
rm libExternalMediaLib.a
cp Sources/externalmedialib.h "../Modelica/ExternalMedia 3.2.1/Resources/Include"
echo "All done"