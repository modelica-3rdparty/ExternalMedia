# ExternalMedia

The `ExternalMedia` library provides a framework for interfacing external codes computing fluid properties to Modelica.Media-compatible component models.

## Library description

The two main requirements are: maximizing the efficiency of the code and minimizing the amount of extra code required to use your own external code within the framework.

The ExternalMedia library provides a framework for interfacing external codes computing fluid properties to Modelica.Media-compatible component models. The two main requirements are: maximizing the efficiency of the code and minimizing the amount of extra code required to use your own external code within the framework.

The current version of the library supports pure fluids models, possibly two-phase, compliant with the Modelica.Media.Interfaces.PartialTwoPhaseMedium interface.

The releases of the library available on the Modelica website include a pre-compiled interface to the FluidProp tool, http://www.fluidprop.com. FluidProp features many built-in fluid models, and can optionally be used to access the whole NIST RefProp database, thus giving easy access to a wide range of fluid models with state-of-the-art accuracy. Make sure you download the Modelica package corresponding to the version of Microsoft Visual Studio that you use to compile the Modelica models, in order to avoid linker errors.

Before using the library, download and install the latest version of FluidProp from http://www.fluidprop.com. If you want to use the RefProp fluid models, you need to get the full version of FluidProp, which has an extra license fee.


## Current release

Currently there is no offical release available. However you can download the [latest development version](../../archive/master.zip) at any time.

## License

This Modelica package is free software and the use is completely at your own risk;
it can be redistributed and/or modified under the terms of the [Modelica License 2](https://modelica.org/licenses/ModelicaLicense2).

## Development and contribution
Main developer: [Francesco Casella](mailto:francesco.casella@polimi.it)

You may report any issues by using the [Modelica Issue Tracker](https://trac.modelica.org/Modelica/newticket?component=_ExternalMedia).
