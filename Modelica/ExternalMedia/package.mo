within ;
package ExternalMedia
  extends Modelica.Icons.Package;
  import SI = Modelica.SIunits;


  annotation(uses(Modelica(version="3.2.3")),
             version="3.3.2",
             conversion(noneFromVersion="3.3.1",
                        noneFromVersion="3.3.0"),
  Documentation(info="<html>
<p>The <b>ExternalMedia</b> library provides a framework for interfacing external codes computing fluid properties to Modelica.Media-compatible component models. The library has been designed with two main goals: maximizing the efficiency of the code, while minimizing the amount of extra code required to interface existing external codes to the library.</p>
<p>The library covers pure fluids models, possibly two-phase, compliant with the <a href=\"modelica://Modelica.Media.Interfaces.PartialTwoPhaseMedium\">Modelica.Media.Interfaces.PartialTwoPhaseMedium</a> interface. </p>
<p>Two external softwares for fluid property computation are currently supported by the ExternalMedia library:</p>
<ul>
<li><a href=\"http://www.fluidprop.com\">FluidProp</a>, formerly developed at TU Delft and currently devloped and maintained by Asimptote</li>
<li><a href=\"http://coolprop.org\">CoolProp</a>, developed at the University of Liege and at the Technical University of Denmark (DTU)</li>
</ul>
<p>A detailed documentation of the software is available in the <a href=\"modelica://ExternalMedia/Resources/manual.pdf\">technical manual</a>.</p>
<p>The library has been tested with the Dymola and OpenModelica tools under the Windows and Linux operating system. If you are interested in the support of other tools, operating systems, and external fluid property computation codes, please contact the developers.</p>
<p>Main contributors: Francesco Casella, Christoph Richter, Roberto Bonifetto, Ian Bell, Jorrit Wronski, Federico Terraneo.</p>
<p><b>The code is licensed under the BSD 3-clause license. </b>For license conditions visit <a href=\"https://opensource.org/licenses/BSD-3-Clause\">https://opensource.org/licenses/BSD-3-Clause</a>. </p>
<p>Copyright &copy; 2006-2023, Francesco Casella, Christoph Richter, Roberto Bonifetto, Ian Bell, Jorrit Wronski, Federico Terraneo.</p>
</html>"));
end ExternalMedia;
