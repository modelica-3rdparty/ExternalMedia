within ;
package ExternalMedia 
  import SI = Modelica.SIunits;


  annotation (uses(Modelica(version="3.2")), Documentation(info="<html>

  <p>The ExternalMedia library provides a framework for interfacing
  external codes computing fluid properties to Modelica.Media-compatible
  component models. The two main requirements are: maximizing the efficiency
  of the code and minimizing the amount of extra code required to use your
  own external code within the framework.</p>
  
  <p>The current version of the library supports pure fluids models, possibly
  two-phase, compliant with the 
  <a href=\"modelica://Modelica.Media.Interfaces.PartialTwoPhaseMedium\">
  Modelica.Media.Interfaces.PartialTwoPhaseMedium</a> interface.</p>
  
  <p>  The releases of the library available on the Modelica website include
  a pre-compiled interface to the FluidProp tool,
  <a href=\"http://www.fluidprop.com\">http://www.fluidprop.com</a>. 
  FluidProp features many built-in fluid models, and can optionally be used
  to access the whole NIST RefProp database, thus giving easy access to a wide
  range of fluid models with state-of-the-art accuracy. Make sure you download
  the Modelica package corresponding to the version of Microsoft Visual Studio
  that you use to compile the Modelica models, in order to avoid linker errors. </p>

  <p>Before using the library, download and install the latest version of FluidProp from 
  <a href=\"http://www.fluidprop.com\">http://www.fluidprop.com</a>.
  If you want to use the RefProp fluid models, you need to get the full version of
  FluidProp, which has an extra license fee.</p>
  
  <p>  You can now define medium models for all the libraries supported by FluidProp,
  by extending the <a href=\"modelica://ExternalMedia.Media.FluidPropMedium\">
  ExternalMedia.Media.FluidPropMedium</a> package. Set libraryName
  to FluidProp.RefProp, FluidProp.StanMix, FluidProp.TPSI, FluidProp.IF97,
  or FluidProp.GasMix, depending on the specific library you need to use.
  Set substanceNames to a single-element string array containing the name
  of the specific medium, as specified by the FluidProp documentation. Set
  mediumName to a string that describes the medium (this only used for
  documentation purposes but has no effect in selecting the medium model). 
  See ExternalMedia.Examples for examples. Please note that the medium models
  IF97 and GasMix are already available natively in Modelica.Media as 
  <a href=\"modelica://Modelica.Media.Water.StandardWater\">Water.StandardWater</a> and
  <a href=\"modelica://Modelica.Media.IdealGases.MixtureGases\">IdealGases.MixtureGases</a>,
  respectively - it is recommended to use the Modelica.Media models instead,
  since they are much faster to compute.
<p>

  <p> If you want to use your
  own fluid property computation code instead, then you need to check out the
  source code from the Modelica SVN server and add the interface to it. See
  the <a href=\"modelica://ExternalMedia/Resources/manual.pdf\">library
  documentation</a> for further details.</p>
  
  <p> Please contact the main developer, Francesco Casella
  (<a href=\"mailto:casella@elet.polimi.it\">casella@elet.polimi.it</a>)
  if you have questions or suggestions for improvement, or if you want to help
  futher developing the library.</p>
  
<p>Licensed by the Modelica Association under the Modelica License 2</p>
<p>Copyright &copy; 2006-2012 Politecnico di Milano, TU Braunschweig, Politecnico di Torino</p>
<p>Main contributors: Francesco Casella, Christoph Richter, Roberto Bonifetto</p>
<p>
<i>This Modelica package is <u>free</u> software and the use is completely at <u>your own risk</u>; it can be redistributed and/or modified under the terms of the Modelica License 2. For license conditions (including the disclaimer of warranty) see <a href=\"http://www.modelica.org/licenses/ModelicaLicense2\"> http://www.modelica.org/licenses/ModelicaLicense2</a>.</i>
</p>
</html>"));
end ExternalMedia;
