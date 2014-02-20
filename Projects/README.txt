This directory contains the C/C++ source files in the Sources directory
and the project files for different versions of Microsoft Visual Studio.

BUILDING THE LIBRARY FOR DYMOLA USING MICROSOFT VISUAL STUDIO ON WINDOWS

Open the .sln file for the version of Visual Studio you use with Dymola, change
the source code if necessary, and build the static library.

Once built, run the appropriate make-vs-20xx.bat file. This will copy the 
externalmedialib.h file in the Resources/Include directory, and the
compiled library in the Resources/Library directory, both as ExternalMediaLib.lib,
so the Modelica packages can be used right away with the newly built library,
and as ExternalMediaLib.vs20xx.lib, to be used by third parties. Note that
the ExternalMediaLib.lib file is not stored in the SVN repository, only on your
local working copy.

BUIDING THE LIBRARY FOR OPENMODELICA USING GCC ON WINDOWS

- Get the OMDEV environment from the SVN repository:
  https://openmodelica.org/svn/OpenModelicaExternal/trunk/tools/windows/OMDev
  user: anonymous
  pass: none
- Install OMDEV in the C:\OMDev path
- set #include COOLPROP 0 in include.h (CoolProp is not fully supported yet)
- Start C:\OMDev\tools\msys\msys.bat (from the MinGW)
- $ mount d:/Path_to_your_ExternalMediaLibrary_working_copy /ExternalMediaLibrary
- $ cd /ExternalMediaLibrary/Projects
- $ make-gcc
 
This will build the static gcc library and copy it and the externalmedia.h
header files in the Resource directories of the Modelica packages, so it can
be used right away by just loading the Modelica package in OMC

Due to a bug in OMC (https://trac.openmodelica.org/OpenModelica/ticket/2565#comment:7)
it is currently necessary to also copy the include and library files in the OMC installation
directories. To accomplish this, run make-gcc-workaround instead of make-gcc

