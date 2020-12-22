# ExternalMedia

The `ExternalMedia` library provides a framework for interfacing external codes
computing fluid properties to Modelica.Media-compatible component models.

## Library description

The ExternalMedia library provides a framework for interfacing external codes
computing fluid properties to Modelica.Media-compatible component models. It is
compatible with Modelica Standard Library v 3.2, 3.2.1, and 3.2.2.

The current version of the library supports pure and pseudo-pure fluids models,
possibly two-phase, compliant with the
Modelica.Media.Interfaces.PartialTwoPhaseMedium interface.

The current release of the library (3.2.1) includes a pre-compiled interface to
the [FluidProp](http://www.asimptote.nl/software/fluidprop) software and
built-in access to [CoolProp](http://www.coolprop.org).
If you use the FluidProp software, you need to have the proper licenses to
access the media of your interest and to compute the property derivatives.

The released files are typically tested with Dymola and OpenModelica on Windows
as well as with Dymola on 32bit Linux. Support for more tools and operating
systems might be added in the future.

You can modify the library to add an interface to your own solver; if your
solver is open-source, please contact the developers, we might add it to the
official ExternalMedia library.

The library is currently under development for a new release with improved
functionality, see the
[Wiki Page](https://github.com/modelica-3rdparty/ExternalMedia/wiki)
for more info.

## Current release

The last stable release is 3.2.1.

See the Installation.txt file for installation instructions.

## License

This Modelica package is free software and the use is completely at your own
risk; it can be redistributed and/or modified under the terms of the
[Modelica License 2](https://modelica.org/licenses/ModelicaLicense2).

## Development and contribution

Main developers: 
 - [Francesco Casella](mailto:francesco.casella@polimi.it) and Christoph
   Richter for the initial development
 - [Jorrit Wronski](mailto:jowr@ipu.dk) and Ian Bell for the integration of
   CoolProp in the library

Please report problems using
[GitHub issues](https://github.com/modelica-3rdparty/ExternalMedia/issues).
