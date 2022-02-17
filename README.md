# ExternalMedia

The ExternalMedia library provides a framework for interfacing external codes
computing fluid properties to Modelica.Media-compatible component models.

The current downloads can be found here:

 - [Precompiled Modelica library](https://github.com/modelica-3rdparty/ExternalMedia/releases/download/v3.3.1/ExternalMedia_v3.3.1.zip)
 - [Manual as PDF](https://github.com/modelica-3rdparty/ExternalMedia/releases/download/v3.3.1/ExternalMedia_v3.3.1.pdf)
 - [Full source code](https://github.com/modelica-3rdparty/ExternalMedia/archive/refs/tags/v3.3.1.zip)

## Library overview

The ExternalMedia library provides a framework for interfacing external codes
computing fluid properties to Modelica.Media-compatible component models. It is
compatible with Modelica Standard Library (MSL) 3.2.3, which is the latest,
backwards-compatible version of the 3.2.x series. A version compatible with
MSL 4.0.0 is planned for the near future.

The current version of the library supports pure and pseudo-pure fluids models,
possibly two-phase, compliant with the
Modelica.Media.Interfaces.PartialTwoPhaseMedium interface. Please have a look at
the [dedicated introduction section](README_introduction.md) for an in-depth
description of the architecture.

The current release of the library (3.3.0) includes a pre-compiled interface to
the [FluidProp](http://www.asimptote.nl/software/fluidprop) software and
built-in access to [CoolProp](http://www.coolprop.org).
If you use the FluidProp software, you need to have the proper licenses to
access the media of your interest and to compute the property derivatives.
The library works with FluidProp version 3.0 and later. It might work with
previous versions of that software, but compatibility is no longer guaranteed.
Please refer to the [chapter on FluidProp](README_fluidprop.md) and the
dedicated [chapter on CoolProp](README_coolprop.md) for details.

The released files are tested with Dymola and OpenModelica on Windows
as well as with Dymola on Linux. Support for more tools and operating systems
might be added in the future, please let us know if you want to contribute.

You can modify the library to add an interface to your own solver. If your
solver is open-source, please contact the developers, so we can add it to the
official ExternalMedia library.

## Installation instructions for the ExternalMedia library

The provided version of ExternalMedia is compatible with Modelica
Standard Library 3.2.3, we recommend you to use that instead of previous
3.2.x versions, because it contains many bug fixes and is fully backwards
compatible with them.

If you want to experiment with the code and recompile the libraries, check
the [compilation instructions](README_compilation.md).

### Modelica integration

The Modelica Language Specification mentions annotations for External Libraries
and Include Files in [section 12.9.4](https://specification.modelica.org/maint/3.5/functions.html#annotations-for-external-libraries-and-include-files).
Following the concepts put forward there, the ExternalMedia package provides several pre-compiled
static libraries supporting a selection of operating systems, C-compilers and Modelica tools.

Please open the `package.mo` file inside the `ExternalMedia 3.3.0` folder to
load the library. If your Modelica tool is able to find a matching precompiled
binary for your configuration, you should now be able to run the examples.

### Missing library problems and compilation instructions

If your Modelica tool cannot find the provided binaries or if you use an
unsupported compiler, you can build the ExternalMedia files yourself. All
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
controbuted to it. The [history page](README_history.md) provides a lot
of useful insights and explains how the software became what it is today.

Current main developers: 
 - [Francesco Casella](mailto:francesco.casella@polimi.it)
 - [Jorrit Wronski](mailto:jowr@ipu.dk) and Ian Bell for the integration of
   CoolProp in the library and CMake-based compilation

Please report problems using
[GitHub issues](https://github.com/modelica-3rdparty/ExternalMedia/issues).
