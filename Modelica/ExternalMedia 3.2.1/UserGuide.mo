within ExternalMedia;
package UserGuide "User's Guide"
  extends Modelica.Icons.Information;
  class Overview "Overview"
    extends Modelica.Icons.Information;
    annotation (Documentation(info="<html>
<p>The ExternalMedia library provides a framework for interfacing external codes computing fluid properties to Modelica.Media-compatible component models. The two main requirements are: maximizing the efficiency of the code and minimizing the amount of extra code required to use your own external code within the framework. </p>
<p>The current version of the library supports pure fluids models, possibly two-phase, compliant with the <a href=\"modelica://Modelica.Media.Interfaces.PartialTwoPhaseMedium\">Modelica.Media.Interfaces.PartialTwoPhaseMedium</a> interface. </p>
<p>Two external sofwares for fluid property computation are currently suppored by the ExternalMedia library:</p>
<ul>
<li> <a href=\"http://www.fluidprop.com\">FluidProp</a>, formerly developed at TU Delft and currently devloped and maintained by Asimptote</li>
<li> <a href=\"http://coolprop.org\">CoolProp</a>, developed at the University of Liege and at the Technical University of Denmark (DTU)</li>
</ul>
<p>CoolProp is built in the binaries distributed with the library, while FluidProp must be downloaded and installed separately on your computer before using ExternalMedia.</p>
<p>You can now define medium models for all the libraries supported by FluidProp, by extending the <a href=\"modelica://ExternalMedia.Media.FluidPropMedium\">ExternalMedia.Media.FluidPropMedium</a> package. Set libraryName to FluidProp.RefProp, FluidProp.StanMix, FluidProp.TPSI or FluidProp.IF97, or FluidProp.GasMix (only single-component), depending on the specific library you need to use. Set substanceNames to a single-element string array containing the name of the specific medium, as specified by the FluidProp documentation. Set mediumName to a string that describes the medium (this only used for documentation purposes but has no effect in selecting the medium model). See ExternalMedia.Examples for examples. Please note that the medium models IF97 and GasMix are already available natively in Modelica.Media as <a href=\"modelica://Modelica.Media.Water.StandardWater\">Water.StandardWater</a> and <a href=\"modelica://Modelica.Media.IdealGases.MixtureGases\">IdealGases.MixtureGases</a>, respectively - it is recommended to use the Modelica.Media models instead, since they are much faster to compute. </p>
<p>If you want to use your own fluid property computation code instead, then you need to check out the source code from the Modelica SVN server and add the interface to it. See the <a href=\"modelica://ExternalMedia/Resources/manual.pdf\">library documentation</a> for further details. </p>
<p>Please contact the main developer, Francesco Casella (<a href=\"mailto:casella@elet.polimi.it\">casella@elet.polimi.it</a>) if you have questions or suggestions for improvement, or if you want to help futher developing the library. </p>
<p>Licensed by the Modelica Association under the Modelica License 2</p>
<p>Copyright &copy; 2006-2013 Politecnico di Milano, TU Braunschweig, Politecnico di Torino</p>
<p>Main contributors: Francesco Casella, Christoph Richter, Roberto Bonifetto</p>
<p><i>This Modelica package is <u>free</u> software and the use is completely at <u>your own risk</u>; it can be redistributed and/or modified under the terms of the Modelica License 2. For license conditions (including the disclaimer of warranty) see <a href=\"http://www.modelica.org/licenses/ModelicaLicense2\">http://www.modelica.org/licenses/ModelicaLicense2</a>.</i> </p>
</html>"),   DocumentationClass=true);
  end Overview;

  package Usage "Using the ExternalMedia Library"
    extends Modelica.Icons.Information;
    class FluidProp "FluidProp medium models"
      extends Modelica.Icons.Information;
      annotation (Documentation(info="<html>
<p>Pure (or pseudo-pure) medium models from all the libraries in FluidProp can be accessed by extending the <a href=\"modelica://ExternalMedia.Media.FluidPropMedium\">ExternalMedia.Media.FluidPropMedium</a> package.</p>
<p>Set libraryName to &QUOT;FluidProp.RefProp&QUOT;, &QUOT;FluidProp.StanMix&QUOT;, &QUOT;FluidProp.TPSI&QUOT;, &QUOT;FluidProp.IF97&QUOT;, or &QUOT;FluidProp.GasMix&QUOT; (only single-component), depending on the specific library you need to use. Set substanceNames to a single-element string array containing the name of the specific medium, as specified by the FluidProp documentation. Set mediumName to a string that describes the medium (this only used for documentation purposes but has no effect in selecting the medium model).</p>
<p>See <a href=\"modelica://ExternalMedia.Examples\">ExternalMedia.Examples</a> for examples.</p>
<p>Please note that the medium models IF97 and GasMix are already available natively in Modelica.Media as <a href=\"modelica://Modelica.Media.Water.StandardWater\">Water.StandardWater</a> and <a href=\"modelica://Modelica.Media.IdealGases.MixtureGases\">IdealGases.MixtureGases</a>, and are included here for comparison purposes. It is recommended to use the Modelica.Media models instead, since they are much faster to compute. </p>
</html>"));
    end FluidProp;

    class CoolProp "CoolProp medium models"
      extends Modelica.Icons.Information;
      annotation (Documentation(info="<html>
<p>Pure (or pseudo-pure) medium models from all the libraries in CoolProp can be accessed by extending the <a href=\"modelica://ExternalMedia.Media.FluidPropMedium\">ExternalMedia.Media.CoolPropMedium</a> package.</p>
<p>Set libraryName to &QUOT;CoolProp&QUOT;. Set substanceNames to a single-element string array containing the name of the specific medium, as specified by the CoolProp documentation. Set mediumName to a string that describes the medium (this only used for documentation purposes but has no effect in selecting the medium model).</p>
<p>See <a href=\"modelica://ExternalMedia.Examples\">ExternalMedia.Examples</a> for examples.</p>
</html>"));
    end CoolProp;
  end Usage;

  class Contact "Contact information"
    extends Modelica.Icons.Contact;
    annotation (Documentation(info="<html>
<p>For bug reports, suggestions and enquiry, please contact the main developer</p>
<p>Francesco Casella<br>Dipartimento di Elettronica, Informazione e Bioingegneria<br>Politecnico di Milano<br>Via Ponzio 34/5<br>I-20133 Milano ITALY</p>
<p><a href=\"mailto:francesco.casella@polimi.it\">francesco.casella@polimi.it</a></p>
</html>"));
  end Contact;
end UserGuide;
