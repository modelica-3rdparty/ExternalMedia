# ExternalMedia

The ExternalMedia library provides a framework for interfacing external codes
computing fluid properties to Modelica.Media-compatible component models.

The latest releases of the library can be downloaded [here](https://github.com/modelica-3rdparty/ExternalMedia/releases/):

 - The precompiled Modelica library can be found in the zip-file
 - The manual can be downloaded as PDF
 - The full source code is also available as compressed file

## Library overview

The ExternalMedia library provides a framework for interfacing external codes
computing fluid properties to Modelica.Media-compatible component models. The
latest 4.0.x releases are compatible with Modelica Standard Library (MSL) 4.0.0
and later, while 3.3.x versions are provided for legacy models that still use MSL
3.2.3.

The current version of the library supports pure and pseudo-pure fluids models,
possibly two-phase, compliant with the
Modelica.Media.Interfaces.PartialTwoPhaseMedium interface. Please have a look at
the [dedicated introduction section](README_introduction.md) for an in-depth
description of the architecture.

The latest releases of the library include built-in access to the open-source
[CoolProp](http://www.coolprop.org) software and a pre-compiled interface to
the [FluidProp](https://asimptote.com/fluidprop/) commercial
software. CoolProp medium models work out of the box without the need of any
further installation. FluidProp medium models require to install the FluidProp
software with proper licensing to access the media of your interest and to
compute the property derivatives, which are required by ExternalMedia.
The library works with FluidProp version 3.0 and later. It might work with
previous versions of that software, but compatibility is no longer guaranteed.
Please refer to the [chapter on FluidProp](README_fluidprop.md) and the
dedicated [chapter on CoolProp](README_coolprop.md) for further details.

The latest releases were tested with Dymola and OpenModelica on Windows
and Linux. Support for more tools and operating systems
might be added in the future, please let us know if you want to contribute.

You can modify the library to add an interface to your own solver. If your
solver is open-source, please contact the developers, so we can add it to the
official ExternalMedia library.

## Installation instructions for the ExternalMedia library

For OpenModelica, you can install and manage ExternalMedia using the
built-in [Package Manager](https://openmodelica.org/doc/OpenModelicaUsersGuide/latest/omedit.html#omedit-install-library-label).
This feature is coming soon, for the time being you can download the zip file
with the library and unzip it in your file system.

For use with Dymola, you can download the zip file with the library and
unzip it in your file system. The released library already contains all
the pre-compiled binaries for all operating systems, so it should work
out of the box.

Install version 3.3.x of External Media if your models still uses
Modelica Standard Library 3.2.3, otherwise install version 4.0.x.

If you want to experiment with the code and recompile the libraries, check
the [compilation instructions](README_compilation.md).

### Modelica integration

The Modelica Language Specification mentions annotations for External Libraries
and Include Files in [section 12.9.4](https://specification.modelica.org/maint/3.5/functions.html#annotations-for-external-libraries-and-include-files).
Following the concepts put forward there, the ExternalMedia package provides several pre-compiled
shared libraries supporting a selection of operating systems, C-compilers and Modelica tools.

Please open the `package.mo` file inside the `ExternalMedia` folder to
load the library. If your Modelica tool is able to find a matching precompiled
binary for your configuration, you should now be able to run the examples.

### Compiling ExternalMedia from sources

ExternalMedia extensively relies of external functions using code from
pre-compiled dynamic libraries. The released versions include binaries
for Windows and Linux, supporting CoolProp and FluidProp. If you want
to experiment with other external codes or operating systems, you
can build the ExternalMedia binary libraries yourself. All
you need to compile ExternalMedia, besides your C/C++ compiler, is the 
[CMake software](https://cmake.org/) by Kitware. If you would like to include
the CoolProp library, you also need a working Python installation.

Please consult the [compilation guide](README_compilation.md) for further
instructions and details on how to compile ExternalMedia for different Modelica
tools and operating systems.

## License

This Modelica package is free software and the use is completely at your own
risk; it can be redistributed and/or modified under the terms of the
[BSD 3-clause license](https://opensource.org/licenses/BSD-3-Clause).

## Development and contribution

ExternalMedia has been around since 2006 and many different people have
contributed to it. The [history page](README_history.md) provides a lot
of useful insights and explains how the software became what it is today.

Current main developers: 
 - [Francesco Casella](mailto:francesco.casella@polimi.it) started the
   development in 2006 and coordinates the current development effort.
 - [Jorrit Wronski](mailto:jowr@ipu.dk) and Ian Bell took care of the
   integration of CoolProp in the library and of CMake-based compilation.
 - [Federico Terraneo](mailto:federico.terraneo@polimi.it) helped getting
   the library to work with different Modelica tools and operating systems.

Please report problems using the library
[GitHub issue tracker](https://github.com/modelica-3rdparty/ExternalMedia/issues).
