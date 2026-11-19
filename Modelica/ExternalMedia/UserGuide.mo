within ExternalMedia;
package UserGuide "User's Guide"
  extends Modelica.Icons.Information;
  class Overview "Overview"
    extends Modelica.Icons.Information;
    annotation (Documentation(info="<html>
<p>The <b>ExternalMedia</b> library provides a framework for interfacing external codes computing fluid properties to Modelica.Media-compatible component models. The library has been designed with two main goals: maximizing the efficiency of the code, while minimizing the amount of extra code required to interface existing external codes to the library.</p>
<p>The library provides medium packages covering pure fluids models, possibly two-phase, which are 100&percnt; compatible with the <a href=\"modelica://Modelica.Media.Interfaces.PartialTwoPhaseMedium\">Modelica.Media.Interfaces.PartialTwoPhaseMedium</a> interface. </p>
<p>Two external softwares for fluid property computation are currently supported by the ExternalMedia library:</p>
<ul>
<li><a href=\"http://www.fluidprop.com\">FluidProp</a>, formerly developed at TU Delft and currently developed and maintained by Asimptote</li>
<li><a href=\"http://coolprop.org\">CoolProp</a>, developed at the University of Liege and at the Technical University of Denmark (DTU)</li>
</ul>
<p>The library has been tested with Dymola and OpenModelica, both under Windows and Linux. Precompiled binaries are included for Windows (Visual Studio, and OpenModelica 1.23 or later on 64 bit), Linux and macOS (Intel and Apple Silicon, macOS 11 or later).</p>
<p>Please report any issue on the library <a href=\"https://github.com/modelica-3rdparty/ExternalMedia/issues\">GitHub issue tracker</a>.</p>
</html>"));
  end Overview;

  package Usage "Using the ExternalMedia Library"
    extends Modelica.Icons.Information;
    class FluidProp "FluidProp medium models"
      extends Modelica.Icons.Information;
      annotation (Documentation(info="<html>
<p>Pure (or pseudo-pure) medium models from all the libraries in FluidProp can be accessed by extending the <a href=\"modelica://ExternalMedia.Media.FluidPropMedium\">ExternalMedia.Media.FluidPropMedium</a> package. You need to download and install FluidProp on your computer for these models to work: ExternalMedia accesses them through a COM interface.</p>
<p>Set libraryName to &QUOT;FluidProp.RefProp&QUOT;, &QUOT;FluidProp.StanMix&QUOT;, &QUOT;FluidProp.TPSI&QUOT;, &QUOT;FluidProp.IF97&QUOT;, or &QUOT;FluidProp.GasMix&QUOT; (only single-component), depending on the specific library you need to use. Set substanceNames to a single-element string array containing the name of the specific medium, as specified by the FluidProp documentation. Set mediumName to a string that describes the medium (this only used for documentation purposes but has no effect in selecting the medium model).</p>
<p>See <a href=\"modelica://ExternalMedia.Examples\">ExternalMedia.Examples</a> for examples.</p>
<p>Please note that the medium models IF97 and GasMix are already available natively in Modelica.Media as <a href=\"modelica://Modelica.Media.Water.StandardWater\">Water.StandardWater</a> and <a href=\"modelica://Modelica.Media.IdealGases.MixtureGases\">IdealGases.MixtureGases</a>, and are included here for comparison purposes. It is recommended to use the Modelica.Media models instead, since they are much faster to compute. </p>
</html>"));
    end FluidProp;

    class CoolProp "CoolProp medium models"
      extends Modelica.Icons.Information;
      annotation (Documentation(info="<html>
<p>Pure (or pseudo-pure) medium models in <a href=\"http://www.coolprop.org/\">CoolProp</a> can be accessed by extending the <a href=\"modelica://ExternalMedia.Media.CoolPropMedium\">ExternalMedia.Media.CoolPropMedium</a> package.</p>
<p>Set substanceNames to a single-element string array containing the name of the specific medium, as specified by the CoolProp documentation. Additional flags can be passed to CoolProp by separating them from the name of the medium,
e.g., &quot;CO2|enable_BICUBIC=1&quot; or &quot;CO2|enable_TTSE=1|calc_transport=0&quot;. Useful flags include:
<ul>
<li>enable_TTSE (default 0) Enables TTSE tabular interpolation</li>
<li>enable_BICUBIC (default 0) Enables bicubic tabular interpolation</li>
<li>enable_SVDSBTL (default 0) Uses the SVDSBTL tables of CoolProp 8 as starting values for the reference equation of state, see <a href=\"modelica://ExternalMedia.UserGuide.Usage.CoolPropSVDSBTL\">CoolPropSVDSBTL</a></li>
<li>calc_transport (default 1) Enables the computation of transport properties</li>
<li>debug (default 0) Set the debug level, 0-1000</li>
</ul>
</p>
<p>Only one of enable_TTSE, enable_BICUBIC and enable_SVDSBTL can be set at a time.</p>
<p>Set mediumName to a string that describes the medium (this only used for documentation purposes but has no effect in selecting the medium model).</p>
<p>See <a href=\"modelica://ExternalMedia.Examples\">ExternalMedia.Examples</a> for examples.</p>
</html>"));
    end CoolProp;

    class CoolPropSVDSBTL "Faster CoolProp media with SVDSBTL tables (enable_SVDSBTL)"
      extends Modelica.Icons.Information;
      annotation (Documentation(info="<html>
<h4>What it does</h4>
<p>The option enable_SVDSBTL, e.g., &quot;Water|enable_SVDSBTL=1&quot;, makes the CoolProp medium models faster without changing the results: all properties still come from the reference equation of state (the Helmholtz energy equation of state, HEOS, of CoolProp by default).</p>
<p>A Helmholtz energy equation of state is explicit in temperature and density: at a given (T, d), every property and every derivative is a direct evaluation. What makes the plain model slow is the iteration that finds (T, d) for other inputs, for example for pressure and specific enthalpy (p, h), the usual inputs of a fluid model. With enable_SVDSBTL, ExternalMedia</p>
<ol>
<li>looks up T and d for the given inputs in the SVDSBTL tables of CoolProp 8 (SVD-compressed spline-based tables of the p-h, p-T and p-s planes),</li>
<li>evaluates the reference equation of state at that (T, d), with the phase given, so CoolProp skips its own iteration and phase check, and</li>
<li>corrects T and d with Newton steps on the reference equation of state until p and h (or p and s, or p at the given T) match the inputs to a relative tolerance of 1e-9. One or two steps are usually enough.</li>
</ol>
<p>All outputs, including cp, cv, the speed of sound, the isothermal compressibility, the isobaric expansion coefficient, the density derivatives and the transport properties, are then evaluated by the reference equation of state at the input state, with its analytical derivatives. The table never provides an output value, it only provides the starting point.</p>
<p>The saturation properties (setSat_p, setSat_T) work the same way: the table gives the saturation temperature and the saturated densities (from the CoolProp SuperAncillary functions), the reference equation of state is evaluated on the liquid and vapour side, and the derivatives along the saturation line follow from the Clausius-Clapeyron equation. Inputs of density and temperature, of specific enthalpy and entropy, and all two-phase states go to the reference equation of state directly, as without the option.</p>

<h4>When to use it</h4>
<p>Use it when the fluid properties of a pure fluid take a large part of the simulation time and most states are single-phase (liquid, vapour or supercritical), for example in heat exchanger, compressor and pipe models with p-h states. It does not speed up states in the two-phase region.</p>

<h4>Measured accuracy</h4>
<p>The results were compared with the plain model (&quot;Water&quot;, &quot;CO2&quot;, &quot;R134a&quot; without options) through the ExternalMedia interface (Projects/Tests/svdsbtl_solver_check.cpp), at p/p_c = 0.001 to 0.99 for subcooled liquid and superheated vapour at 0.001, 0.01, 0.1, 1 and 10 K from the saturation line, for two-phase states with a vapour quality of 0.001 to 0.999, and for supercritical states up to 4 p_c and 2 T_c, for p-h, p-T, p-s, d-T and h-s inputs, the saturation properties and the bubble and dew states:</p>
<ul>
<li>All outputs agree with the plain model to within 4e-5 (relative). The largest differences, 2e-5 to 4e-5 for cp, the isothermal compressibility and the isobaric expansion coefficient, occur at p/p_c = 0.99 within 0.01 K of the saturation line, where cp of CO2 is about 1e5 J/(kg K). There the plain model itself is only converged to about this level; the SVDSBTL route matches the inputs more closely. Away from the critical point the differences are 1e-7 or smaller.</li>
<li>Saturation properties and their derivatives agree to 1e-14.</li>
<li>No call failed. The tables classified every test point as one-phase or two-phase in the same way as the reference equation of state.</li>
</ul>
<p>A prototype study with 6000 additional random points close to the saturation lines and around the critical point gave the same picture (Projects/Tests/svdsbtl_prototype_results.md). For comparison, the derivatives from the TTSE and bicubic tables (enable_TTSE, enable_BICUBIC) deviate by 3 to 60 &percnt; near the saturation lines and by several percent elsewhere.</p>

<h4>Measured speed</h4>
<p>Time per call of setState_ph through the ExternalMedia interface, CoolProp 8.0.0, Linux, GCC 13 release build, one core of an Intel Xeon at 2.1 GHz, single-phase test points as above:</p>
<table border=\"1\" cellspacing=\"0\" cellpadding=\"2\">
<tr><th>Fluid</th><th>Transport properties</th><th>Plain model</th><th>enable_SVDSBTL</th></tr>
<tr><td>Water</td><td>yes</td><td>191 us</td><td>22 us</td></tr>
<tr><td>Water</td><td>no (calc_transport=0)</td><td>181 us</td><td>9.5 us</td></tr>
<tr><td>CO2</td><td>yes</td><td>180 us</td><td>15 us</td></tr>
<tr><td>R134a</td><td>yes</td><td>40 us</td><td>5.7 us</td></tr>
</table>
<p>For two-phase states both take the same time (8 to 10 us without transport properties). In a stand-alone benchmark (Projects/Tests/svdsbtl_benchmark.cpp), setSat_p took 7 us instead of 19 us for water and CO2, and p-T inputs 10 us instead of 25 us for water. The transport properties of water from the reference model alone take about 12 us per call. The actual gain in a simulation depends on how much of the time is spent in the fluid properties and on the share of single-phase states.</p>

<h4>Limitations</h4>
<ul>
<li>Pure fluids with the HEOS backend (default) or the REFPROP backend (&quot;REFPROP::Water|enable_SVDSBTL=1&quot;) only. Mixtures, incompressible fluids and the IF97 backend are rejected with an error. The REFPROP backend has not been tested.</li>
<li>The first use of a fluid builds the tables from the reference equation of state. For the p-h and p-T tables this took about 100 s for water and CO2 and 30 s for R134a on 4 cores (several minutes on one core). The p-s table is built the first time p-s inputs are used (about 90 s for water). A simulation started with an empty table cache pauses while the tables are built. Later runs load the tables in well under a second.</li>
<li>The tables are stored in the folder .CoolProp/SVDTables in the home directory of the user, 14 to 16 MB per table, so this folder must be writable. Set the environment variable COOLPROP_ALTERNATIVE_SVDTABLES_DIRECTORY to use another folder, for example a shared, pre-filled one. ExternalMedia builds tables with all processor cores unless the environment variable COOLPROP_SVDSBTL_SAMPLING_THREADS sets the number of threads.</li>
<li>No speed-up in the two-phase region and close to the critical point, where the SVDSBTL tables themselves use the reference equation of state.</li>
<li>The state matches the inputs to a relative tolerance of 1e-9 in density and specific enthalpy (or entropy), not exactly. The returned p and h (or s, or T) are the inputs.</li>
<li>If the table route fails (outside the table range, or no convergence), the plain flash of the reference equation of state is used for that call, so the call is slower but the result is the same.</li>
<li>Close to the saturation line the tables decide whether a state is one-phase or two-phase. SVDSBTL checks such points against the same saturation functions that the reference equation of state uses, and no difference was found in the tests, but a state within a fraction of a J/kg of the saturation line could in principle be computed as a (metastable) single-phase state where the plain model gives a two-phase state with a vapour quality of practically 0 or 1.</li>
<li>Tested with CoolProp 8.0.0 for water, CO2 and R134a on Linux, including the test suite <a href=\"modelica://ExternalMedia.Test.CoolProp.CO2_SVDSBTL\">Test.CoolProp.CO2_SVDSBTL</a> in OpenModelica.</li>
</ul>

<h4>Alternatives</h4>
<ul>
<li>The plain CoolProp model (no option) gives the same results and needs no table cache.</li>
<li>calc_transport=0 saves the time for the transport properties if they are not needed, with or without enable_SVDSBTL.</li>
<li>enable_TTSE and enable_BICUBIC are faster per call, but their derivatives (cp, cv, density derivatives) can deviate by several percent, and by much more close to the saturation lines.</li>
<li>For water, <a href=\"modelica://Modelica.Media.Water.StandardWater\">Modelica.Media.Water.StandardWater</a> (IAPWS-IF97) is fast and needs no external library.</li>
</ul>
<p>See <a href=\"modelica://ExternalMedia.Examples\">ExternalMedia.Examples</a> for example media (WaterCoolPropSVDSBTL, CO2CoolPropSVDSBTL, R134aCoolPropSVDSBTL).</p>
</html>"));
    end CoolPropSVDSBTL;
  end Usage;

  class Contact "Contact information"
    extends Modelica.Icons.Contact;
    annotation (Documentation(info="<html>
<p>For suggestions and enquiries regarding the library development and the support of more tools, operating systems and external codes, please contact the main developer:</p>
<p>Francesco Casella
<br>Dipartimento di Elettronica, Informazione e Bioingegneria
<br>Politecnico di Milano
<br>Via Ponzio 34/5
<br>I-20133 Milano ITALY<br>
<a href=\"mailto:francesco.casella@polimi.it\">francesco.casella@polimi.it</a></p>
<p>Submit bug reports to <a href=\"https://github.com/modelica-3rdparty/ExternalMedia/issues\">
https://github.com/modelica-3rdparty/ExternalMedia/issues</p>
</html>"));
  end Contact;
  annotation(DocumentationClass = true);
end UserGuide;
