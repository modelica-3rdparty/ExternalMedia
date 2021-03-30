# Using the pre-packaged releases with FluidProp

Download and install the latest version of 
[FluidProp](http://www.fluidprop.com). If you want to use the RefProp fluid
models, you need to get the full version of FluidProp, which has an extra
license fee.

Download and unzip the library corresponding to the version of Microsoft Visual Studio
that you use to compile your Modelica models, in order to avoid linker errors.
Make sure that you load the ExternalMedia library in your Modelica tool
workspace, e.g. by opening the main package.mo file.

You can now define medium models for the different libraries supported by FluidProp,
by extending the ExternalMedia.Media.FluidPropMedium package. Please note that
only single-component fluids are supported. Set libraryName
to "FluidProp.RefProp", "FluidProp.StanMix", "FluidProp.TPSI", or "FluidProp.IF97",
depending on the specific library you need to use.
Set substanceNames to a single-element string array containing the name
of the specific medium, as specified by the FluidProp documentation. Set
mediumName to a string that describes the medium (this only used for
documentation purposes but has no effect in selecting the medium model). 
See ExternalMedia.Examples for examples.

Please note that the medium model IF97 is already available
natively in Modelica.Media as Water.StandardWater, which is much faster than
the FluidProp version. If you need ideal gas models (single-component or
mixtures), use the medium packages contained in Modelica.Media.IdealGases.
