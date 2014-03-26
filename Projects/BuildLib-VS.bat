echo off

REM   ****************** README *********************************
REM BuildLib-VSXXXX.bat should be called to set the proper environmental variables
REM Do not run this file directly!!!
REM   ****************** README *********************************
REM ~ /Ox : full optimization
REM ~ /c : compile
REM ~ /MP3 : 
REM ~ /fp:fast :
REM ~ /MD : Multi-threaded DLL runtime library
REM ~ /Ehsc : Explicitly enable exception handling

set C_OPTS=/Ox /c /MP3 /fp:fast /MD /EHsc
set CP=..\externals\coolprop\trunk
set CPinc=%CP%\CoolProp
set INCLUDES=-I%CPinc%

REM echo "Copying coolpropsolver.cpp/h to Sources"
REM copy "%CP%\wrappers\Modelica\src\coolpropsolver.cpp" Sources
REM copy "%CP%\wrappers\Modelica\src\coolpropsolver.h" Sources

for /f "delims=" %%a in ('FINDSTR FLUIDPROP Sources\include.h') do set line=%%a
for /f "tokens=3" %%a in ("%line%") do set FLUIDP=%%a

for /f "delims=" %%a in ('FINDSTR COOLPROP Sources\include.h') do set line=%%a
for /f "tokens=3" %%a in ("%line%") do set COOLP=%%a


REM echo "Compiling sources"
REM ****** compile all the sources for OpenModelica ************
cl %C_OPTS% /c %INCLUDES% Sources\*.cpp

REM ********** CoolProp sources *********
if "%COOLP%"=="1" cl %C_OPTS% /c %INCLUDES% %CP%\CoolProp\*.cpp

lib *.obj /OUT:ExternalMediaLib.lib
erase *.obj