echo off
setlocal EnableDelayedExpansion

REM   ******************* README *********************************
REM    You might have to adjust some settings manually. This file 
REM    requires that you know the name of your CMake generator, if
REM    you have more than one development environment installed.
REM   ******************* README *********************************

set BUILD_DIR="build"
if not exist "%BUILD_DIR%" (mkdir "%BUILD_DIR%")
pushd "%BUILD_DIR%"
set BUILD_DIR=!CD!
popd 

echo 
echo ********* Detecting supported property libraries ***********
set i=0
for /f "delims=" %%a in ('FINDSTR FLUIDPROP Sources\include.h') do (
  set line[!i!]=%%a
  set /A i=i+1
  )
for /f "tokens=3" %%a in ("%line[0]%") do set FLUIDP=%%a
echo FluidProp support set to: %FLUIDP%

set i=0
for /f "delims=" %%a in ('FINDSTR COOLPROP Sources\include.h') do (
  set line[!i!]=%%a
  set /A i=i+1
  )
for /f "tokens=3" %%a in ("%line[0]%") do set COOLP=%%a
echo  CoolProp support set to: %COOLP%

pushd "%BUILD_DIR%"
:: Select one of the options below
:: :: Use 64bit
:: cmake .. -G "Visual Studio 9 2008"  -A "x64" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 10 2010" -A "x64" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 11 2012" -A "x64" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 12 2013" -A "x64" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 14 2015" -A "x64" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 15 2017" -A "x64" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 16 2019" -A "x64" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: :: Use 32bit
:: cmake .. -G "Visual Studio 9 2008"  -A "Win32" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 10 2010" -A "Win32" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 11 2012" -A "Win32" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 12 2013" -A "Win32" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 14 2015" -A "Win32" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 15 2017" -A "Win32" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
:: cmake .. -G "Visual Studio 16 2019" -A "Win32" -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
::
cmake .. -DFLUIDP=%FLUIDP% -DCOOLP=%COOLP%
::
cmake --build . --config Release --target install
popd 

:End
