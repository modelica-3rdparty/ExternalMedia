echo off

REM   ****************** README *********************************
REM BuildLib-VSXXXX.bat should be called to set the proper environmental variables
REM Do not run this file directly!!!
REM   ****************** README *********************************

REM /DCOOLPROP=1 enables the use of CoolProp, which defaults to being disabled.
REM Use /DCOOLPROP=0 to compile ExternalMedia wrapper without CoolProp support
REM The C++ files for CoolProp will still be compiled
REM ~ /Ox : full optimization
REM ~ /c : compile
REM ~ /MP3 : 
REM ~ /fp:fast :
REM ~ /MD : Multi-threaded DLL runtime library
REM ~ /Ehsc : Explicitly enable exception handling

set C_OPTS=/Ox /c /MP3 /fp:fast /DCOOLPROP=1 /MD /EHsc
set CP=..\externals\coolprop\branches\ExternalMedia
set CPinc=%CP%\CoolProp
set INCLUDES=-I%CPinc%

REM echo "Copying coolpropsolver.cpp/h to Sources"
REM copy "%CP%\wrappers\Modelica\src\coolpropsolver.cpp" Sources
REM copy "%CP%\wrappers\Modelica\src\coolpropsolver.h" Sources

echo "Compiling sources"
REM ****** compile all the sources for OpenModelica ************
cl %C_OPTS% /c %INCLUDES% Sources\*.cpp

REM ********** CoolProp sources *********
cl %C_OPTS% /c %INCLUDES% %CP%\CoolProp\*.cpp

lib *.obj /OUT:ExternalMediaLib.lib
erase *.obj
