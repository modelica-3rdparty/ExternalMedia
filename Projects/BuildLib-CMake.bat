echo off
setlocal EnableDelayedExpansion

REM   ******************* README *********************************
REM    You might have to adjust some settings manually. This file 
REM    requires that you know the name of your CMake generator, if
REM    you have more than one development environment installed.
REM   ******************* README *********************************

:: For Windows builds in 32bit
::set CMake_generator="Visual Studio 9 2008"
::set CMake_generator="Visual Studio 10 2010"
::set CMake_generator="Visual Studio 11 2012"
::set CMake_generator="Visual Studio 12 2013"
::set CMake_generator="Visual Studio 14 2015"

:: For Windows builds in 64bit
::set CMake_generator="Visual Studio 9 2008 Win64"
::set CMake_generator="Visual Studio 10 2010 Win64"
::set CMake_generator="Visual Studio 11 2012 Win64"
::set CMake_generator="Visual Studio 12 2013 Win64"
::set CMake_generator="Visual Studio 14 2015 Win64"

set EXTERNALS="..\externals"
if not exist "%EXTERNALS%" (mkdir "%EXTERNALS%")
pushd "%EXTERNALS%"
set CP_SRC=!CD!\CoolProp.git
popd 

set BUILD_DIR="build"
if not exist "%BUILD_DIR%" (mkdir "%BUILD_DIR%")
pushd "%BUILD_DIR%"
set BUILD_DIR=!CD!
popd 

echo " " 
echo "********* Detecting supported property libraries ***********"
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

if "%COOLP%"=="1" (
  echo "******************** CoolProp ******************************"
  echo "Sources are located in %CP_SRC%"
  if exist "%CP_SRC%" (
    pushd "%CP_SRC%"
    git pull origin master
    git submodule init
    git submodule update
    :: git submodule foreach git pull origin master
    popd 
  ) else (
    git clone --recursive https://github.com/CoolProp/CoolProp.git "%CP_SRC%"
  )
)

pushd "%BUILD_DIR%"
:: cmake .. -G "%CMake_generator%" 
cmake .. -G "%CMake_generator%" 
cmake --build . --config Release
popd 

:End
