echo off
setlocal EnableDelayedExpansion

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

set EXTERNALS="..\externals"
if not exist "%EXTERNALS%" (mkdir "%EXTERNALS%")
pushd "%EXTERNALS%"
set CP_SRC=!CD!\coolprop.git
set CP_TMP=!CD!\coolprop.build.msvc
popd 
set CP_INC=%CP_SRC%\include
set INCLUDES=-I "%CP_INC%"
set CP_COMP=%~1


echo ' ' 
echo "Detecting supported solvers"
set i=0
for /f "delims=" %%a in ('FINDSTR FLUIDPROP Sources\include.h') do (
  set line[!i!]=%%a
  set /A i=i+1
  )
for /f "tokens=3" %%a in ("%line[0]%") do set FLUIDP=%%a
echo "FluidProp support set to: %FLUIDP%"

set i=0
for /f "delims=" %%a in ('FINDSTR COOLPROP Sources\include.h') do (
  set line[!i!]=%%a
  set /A i=i+1
  )
for /f "tokens=3" %%a in ("%line[0]%") do set COOLP=%%a
echo " CoolProp support set to: %COOLP%"


REM echo "Compiling sources"
REM ****** compile all the sources for OpenModelica ************
cl %C_OPTS% /c %INCLUDES% Sources\*.cpp

REM ********** CoolProp sources *********
if "%COOLP%"=="1" (
  REM echo "Sources in %CP_SRC%"
  REM echo "Builds in %CP_TMP%"
  if exist "%CP_SRC%" (
    pushd "%CP_SRC%"
    git pull
    popd 
  ) else (
    git clone --recursive https://github.com/CoolProp/CoolProp.git "%CP_SRC%"
  )
  if not exist "%CP_TMP%" (
    mkdir "%CP_TMP%"
    REM echo "Created: %CP_TMP%"
  )
  pushd "%CP_TMP%"
  REM echo "Sources for %CP_COMP% in %CP_SRC%"
  cmake "%CP_SRC%" -DCOOLPROP_OBJECT_LIBRARY=ON -DCOOLPROP_RELEASE=ON -G "%CP_COMP%" 
  cmake --build . --config Release
  popd
  copy "%CP_TMP%\CoolProp.dir\Release\"*.obj .
)


lib *.obj /OUT:ExternalMediaLib.lib
erase *.obj

:End