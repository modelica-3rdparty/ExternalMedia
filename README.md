# ExternalMedia

The `ExternalMedia` library provides a framework for interfacing external codes computing fluid properties to Modelica.Media-compatible component models.

## Library description

The ExternalMedia library provides a framework for interfacing external codes computing fluid properties to Modelica.Media-compatible component models.
It is compatible with Modelica Standard Library v 3.2 and 3.2.1

The current version of the library supports pure and pseudo-pure fluids models, possibly two-phase, compliant with the Modelica.Media.Interfaces.PartialTwoPhaseMedium interface.

The library includes a pre-compiled interface to the [FluidProp](http://www.asimptote.nl/software/fluidprop) software and built-in access to [CoolProp](http://www.coolprop.org).
If you use the FluidProp software, you need to have the proper licenses to access the media of your interest and to compute the property derivatives.

You can modify the library to add an interface to your own solver; if your solver is open-source, please contact the developers, we might add it to the official
ExternalMedia library.

As of June 2014, the library has been tested with Dymola and OpenModelica under Windows as well as with Dymola on 32bit Linux. Support for more tools and operating systems might
be added in the future.

## Current release

Currently there is no offical release available. However you can download the [latest development version](../../archive/master.zip) at any time.

See the Installation.txt file for installation instructions.

## License

This Modelica package is free software and the use is completely at your own risk;
it can be redistributed and/or modified under the terms of the [Modelica License 2](https://modelica.org/licenses/ModelicaLicense2).

## Development and contribution
Main developer: [Francesco Casella](mailto:francesco.casella@polimi.it)

You may report any issues by using the [Modelica Issue Tracker](https://trac.modelica.org/Modelica/newticket?component=_ExternalMedia).
