# ExternalMedia

The ExternalMedia library provides a framework for interfacing external codes
computing fluid properties to Modelica.Media-compatible component models.

## Library overview

The ExternalMedia library provides a framework for interfacing external codes
computing fluid properties to Modelica.Media-compatible component models. It is
compatible with Modelica Standard Library v 3.2, 3.2.1, 3.2.2 and 3.2.3.

The current version of the library supports pure and pseudo-pure fluids models,
possibly two-phase, compliant with the
Modelica.Media.Interfaces.PartialTwoPhaseMedium interface. Please have a look at
the [dedicated introduction section](README_introduction.md) for an in-depth
description of the architecture.

The current release of the library (3.2.3) includes a pre-compiled interface to
the [FluidProp](http://www.asimptote.nl/software/fluidprop) software and
built-in access to [CoolProp](http://www.coolprop.org).
If you use the FluidProp software, you need to have the proper licenses to
access the media of your interest and to compute the property derivatives.
The library works with FluidProp version 3.0 and later. It might work with
previous versions of that software, but compatibility is no longer guaranteed.
Please refer to the [chapter on FluidProp](README_fluidprop.md) and the
dedicated [chapter on CoolProp](README_coolprop.md) for details.

The released files are typically tested with Dymola and OpenModelica on Windows
as well as with Dymola on Linux. Support for more tools and operating systems
might be added in the future.

You can modify the library to add an interface to your own solver. If your
solver is open-source, please contact the developers, we might add it to the
official ExternalMedia library.

## Installation instructions for the ExternalMedia library

The provided version of ExternalMedia is compatible with Modelica
Standard Library 3.2.3.

You can also use it with the Modelica Standard Library down to 3.2, provided
you change the uses annotation in the package.mo file of the Modelica package
root directory to uses(Modelica(version = "3.2")). Previous versions of the
Modelica Standard Library are no longer supported.

If you want to experiment with the code and recompile the libraries, check
the [compilation instructions](README_compilation.md).

### Modelica integration

The Modelica Language Specification mentions annotations for External Libraries
and Include Files in section 12.9.4. Following the concepts put forward there,
the ExternalMedia package provides several static libraries supporting a
selection of compilers and development environments.

Please open the `package.mo` file inside the `ExternalMedia 3.2.3` folder to
load the library. If your Modelica tool is able to find a matching precompiled
binary for your configuration, you should now be able to run the examples.

### Missing library problems and compilation instructions

If your Modelica tool cannot find the provided binaries or if you use an
unsupported compiler, you can build the ExternalMedia files yourself. All
you need to compile ExternalMedia, besides your C/C++ compiler, is the 
[CMake software] (https://cmake.org/) by Kitware. If you would like to include
the CoolProp library, you also need a working Python installation.

Please consult [compilation guide](README_compilation.md) for further
instructions and details on how to compile ExternalMedia for different Modelica
tools and operating systems.

## License

This Modelica package is free software and the use is completely at your own
risk; it can be redistributed and/or modified under the terms of the
[Modelica License 2](https://modelica.org/licenses/ModelicaLicense2).

## Development and contribution

ExternalMedia has been around since 2006 and many different people have
controbuted to it. The [history page](README_history.md) provides a lot
of useful insights and explains how the software became what it is today.

Current main developers: 
 - [Francesco Casella](mailto:francesco.casella@polimi.it) and Christoph
   Richter for the initial development
 - [Jorrit Wronski](mailto:jowr@ipu.dk) and Ian Bell for the integration of
   CoolProp in the library

Please report problems using
[GitHub issues](https://github.com/modelica-3rdparty/ExternalMedia/issues).
