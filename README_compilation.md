# Compilation guide

## Quick-start guide

The heavy-lifting regarding the project configuration is done using the CMake
file `CMakeLists.txt`, which makes the [CMake software](https://cmake.org/)
a prerequisite for compiling ExternalMedia.

Once you have installed CMake and can access it from a command prompt, you can
go to the root folder of the source code and run:

```shell
cmake -B build -S Projects -DCMAKE_BUILD_TYPE=Release
cmake -B build -S Projects -DCMAKE_BUILD_TYPE=Release
```

Please note that there is no typing mistake in the lines above. The current version
of ExternalMedia requires you to run the configure step twice. Now you should have
a working project configuration and the actual compilation can be triggered using:

```shell
cmake --build build --config Release --target install
```

By default, the libraries are installed in a subfolder with a name that is determined
from the current operating system and the compiler, possible combinations are:
- `Modelica/ExternalMedia ${APP_VERSION}/Resources/Library/win32/vs2015`
- `Modelica/ExternalMedia ${APP_VERSION}/Resources/Library/win64/vs2019`
- `Modelica/ExternalMedia ${APP_VERSION}/Resources/Library/linux64/gcc81`

If you would like to skip the compiler part and make the current configuration the
default for the platform, you can use this command below:

```shell
cmake --build build --config Release --target install-as-default
```

You can override these settings manually using the command line switches for
`MODELICA_PLATFORM` and `MODELICA_COMPILER`. The command 
`cmake -B build -S Projects -DMODELICA_PLATFORM:STRING=mingw64 -DMODELICA_COMPILER:STRING=`
would for example configure the installation folder to
`Modelica/ExternalMedia ${APP_VERSION}/Resources/Library/mingw64`, which is the
preferred search path for OpenModelica that supports side-by-side installations with
other compilers and configuration that support other Modelica tools.

## Selecting the fluid property libraries

You can disable and enable the FluidProp and the CoolProp integration with command
line switches.

The recommended configuration step for Windows systems is 

```shell
cmake -B build -S Projects -DCMAKE_BUILD_TYPE=Release -DFLUIDPROP:BOOL=ON -DCOOLPROP:BOOL=ON
cmake -B build -S Projects -DCMAKE_BUILD_TYPE=Release -DFLUIDPROP:BOOL=ON -DCOOLPROP:BOOL=ON
```

... and for all other systems, you probably want to use

```shell
cmake -B build -S Projects -DCMAKE_BUILD_TYPE=Release -DFLUIDPROP:BOOL=OFF -DCOOLPROP:BOOL=ON
cmake -B build -S Projects -DCMAKE_BUILD_TYPE=Release -DFLUIDPROP:BOOL=OFF -DCOOLPROP:BOOL=ON
```

## Building OpenModelica libraries

Get the OMDEV environment from the git repository:

```shell
git clone https://openmodelica.org/git/OMDev.git C:/OMDev
```

To install OMDEV in the `C:\OMDev` path, you should start `C:\OMDev\tools\msys\msys.bat`. This
gives you a command window that looks like the emulation of a unix prompt. Afterwards, you can
run the following commands

```shell
$ mount d:/Path_to_your_ExternalMediaLibrary_working_copy /ExternalMediaLibrary
$ cd /ExternalMediaLibrary/
$ cmake -B build -S Projects -G "MSYS Makefiles" -DCMAKE_BUILD_TYPE=Release
$ cmake --build build --target install
```

This will build the static gcc library and copy it and the `externalmedia.h`
header files in the Resource directories of the Modelica packages, so it can
be used right away by just loading the Modelica package in OMC.
