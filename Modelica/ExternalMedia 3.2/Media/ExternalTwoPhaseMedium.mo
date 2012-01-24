within ExternalMedia.Media;
package ExternalTwoPhaseMedium
  "External two phase medium package - modify libraryName to set the external library"

  extends Modelica.Media.Interfaces.PartialTwoPhaseMedium(
    mediumName = "ExternalMedium",
    singleState = false,
    onePhase = false,
    smoothModel = false,
    fluidConstants = {externalFluidConstants});
  import ExternalMedia.Common.InputChoice;

  constant String libraryName = "UnusableExternalMedium"
    "Name of the external fluid property computation library";
  final constant String substanceName = substanceNames[1]
    "Only one substance can be specified";

  constant FluidConstants externalFluidConstants = FluidConstants(
    iupacName=  "unknown",
    casRegistryNumber=  "unknown",
    chemicalFormula=  "unknown",
    structureFormula=  "unknown",
    molarMass=  getMolarMass(),
    criticalTemperature=  getCriticalTemperature(),
    criticalPressure=  getCriticalPressure(),
    criticalMolarVolume=  getCriticalMolarVolume(),
    acentricFactor=  0,
    triplePointTemperature=  280.0,
    triplePointPressure=  500.0,
    meltingPoint=  280,
    normalBoilingPoint=  380.0,
    dipoleMoment=  2.0);

  constant InputChoice inputChoice=InputChoice.ph
    "Default choice of input variables for property computations";

  redeclare replaceable record extends ThermodynamicState
    Density d "density";
    SpecificEnthalpy h "specific enthalpy";
    AbsolutePressure p "pressure";
    SpecificEntropy s "specific entropy";
    Temperature T "temperature";
    Integer phase "phase flag: 2 for two-phase, 1 for one-phase";
    Modelica.SIunits.CubicExpansionCoefficient beta
      "isobaric expansion coefficient";
    SpecificHeatCapacity cp "specific heat capacity cp";
    SpecificHeatCapacity cv "specific heat capacity cv";
    DerDensityByPressure ddph
      "derivative of density wrt pressure at constant enthalpy";
    DerDensityByEnthalpy ddhp
      "derivative of density wrt enthalpy at constant pressure";
    Modelica.SIunits.Compressibility kappa "compressibility";
    Real dT_dp_h "derivative of temperature wrt pressure at constant enthalpy";
    Real dT_dh_p "derivative of temperature wrt enthalpy at constant pressure";
    DynamicViscosity eta "dynamic viscosity";
    ThermalConductivity lambda "thermal conductivity";
    PrandtlNumber Pr "prandtl number";
    VelocityOfSound a "velocity of sound";
  end ThermodynamicState;

  redeclare record extends SaturationProperties
    AbsolutePressure psat "saturation pressure";
    Temperature Tsat "saturation temperature";
    Density dl "density at bubble line (for pressure ps)";
    Density dv "density at dew line (for pressure ps)";
    SpecificEnthalpy hl "specific enthalpy at bubble line (for pressure ps)";
    SpecificEnthalpy hv "specific enthalpy at dew line (for pressure ps)";
    SpecificEntropy sl "specific entropy at bubble line (for pressure ps)";
    SpecificEntropy sv "specific entropy at dew line (for pressure ps)";
    SurfaceTension sigma "surface tension";
    Real dTp "derivative of Ts wrt pressure";
    DerDensityByPressure ddldp "derivative of dls wrt pressure";
    DerDensityByPressure ddvdp "derivative of dvs wrt pressure";
    DerEnthalpyByPressure dhldp "derivative of hls wrt pressure";
    DerEnthalpyByPressure dhvdp "derivative of hvs wrt pressure";
  end SaturationProperties;

  redeclare replaceable model extends BaseProperties(
    p(stateSelect = if preferredMediumStates and
                       (basePropertiesInputChoice == InputChoice.ph or
                        basePropertiesInputChoice == InputChoice.pT or
                        basePropertiesInputChoice == InputChoice.ps) then
                            StateSelect.prefer else StateSelect.default),
    T(stateSelect = if preferredMediumStates and
                       (basePropertiesInputChoice == InputChoice.pT or
                       basePropertiesInputChoice == InputChoice.dT) then
                         StateSelect.prefer else StateSelect.default),
    h(stateSelect = if preferredMediumStates and
                       basePropertiesInputChoice == InputChoice.ph then
                         StateSelect.prefer else StateSelect.default),
    d(stateSelect = if preferredMediumStates and
                       basePropertiesInputChoice == InputChoice.dT then
                         StateSelect.prefer else StateSelect.default))
    import ExternalMedia.Common.InputChoice;
    parameter InputChoice basePropertiesInputChoice=inputChoice
      "Choice of input variables for property computations";
    FixedPhase phaseInput
      "Phase input for property computation functions, 2 for two-phase, 1 for one-phase, 0 if not known";
    Integer phaseOutput
      "Phase output for medium, 2 for two-phase, 1 for one-phase";
    SpecificEntropy s(
      stateSelect = if basePropertiesInputChoice == InputChoice.ps then
                       StateSelect.prefer else StateSelect.default)
      "Specific entropy";
    SaturationProperties sat "saturation property record";
  equation
    MM = externalFluidConstants.molarMass;
    R = Modelica.Constants.R/MM;
    if (onePhase or (basePropertiesInputChoice == InputChoice.pT)) then
      phaseInput = 1 "Force one-phase property computation";
    else
      phaseInput = 0 "Unknown phase";
    end if;
    if (basePropertiesInputChoice == InputChoice.ph) then
      // Compute the state record (including the unique ID)
      state = setState_ph(p, h, phaseInput);
      // Compute the remaining variables.
      // It is not possible to use the standard functions like
      // d = density(state), because differentiation for index
      // reduction and change of state variables would not be supported
      // density_ph(), which has an appropriate derivative annotation,
      // is used instead. The implementation of density_ph() uses
      // setState with the same inputs, so there's no actual overhead
      d = density_ph(p, h, phaseInput);
      s = specificEntropy_ph(p, h, phaseInput);
      T = temperature_ph(p, h, phaseInput);
    elseif (basePropertiesInputChoice == InputChoice.dT) then
      state = setState_dT(d, T, phaseInput);
      h = specificEnthalpy(state);
      p = pressure(state);
      s = specificEntropy(state);
    elseif (basePropertiesInputChoice == InputChoice.pT) then
      state = setState_pT(p, T, phaseInput);
      d = density(state);
      h = specificEnthalpy(state);
      s = specificEntropy(state);
    elseif (basePropertiesInputChoice == InputChoice.ps) then
      state = setState_ps(p, s, phaseInput);
      d = density(state);
      h = specificEnthalpy(state);
      T = temperature(state);
    end if;
    // Compute the internal energy
    u = h - p/d;
    // Compute the saturation properties record
    sat = setSat_p_state(state);
    // Event generation for phase boundary crossing
    if smoothModel then
      // No event generation
      phaseOutput = state.phase;
    else
      // Event generation at phase boundary crossing
      if basePropertiesInputChoice == InputChoice.ph then
        phaseOutput = if ((h > bubbleEnthalpy(sat) and h < dewEnthalpy(sat)) and
                           p < fluidConstants[1].criticalPressure) then 2 else 1;
      elseif basePropertiesInputChoice == InputChoice.dT then
        phaseOutput = if  ((d < bubbleDensity(sat) and d > dewDensity(sat)) and
                            T < fluidConstants[1].criticalTemperature) then 2 else 1;
      elseif basePropertiesInputChoice == InputChoice.ps then
        phaseOutput = if ((s > bubbleEntropy(sat) and s < dewEntropy(sat)) and
                           p < fluidConstants[1].criticalPressure) then 2 else 1;
      else
        // basePropertiesInputChoice == pT
        phaseOutput = 1;
      end if;
    end if;
  end BaseProperties;

  redeclare function molarMass "Return the molar mass of the medium"
      input ThermodynamicState state;
      output MolarMass MM "Mixture molar mass";
  algorithm
    MM := fluidConstants[1].molarMass;
  end molarMass;

  redeclare function extends setState_phX
  algorithm
    // The composition is an empty vector
    state :=setState_ph(p, h, phase);
  end setState_phX;

  redeclare function extends setState_pTX
  algorithm
    // The composition is an empty vector
    state :=setState_pT(p, T, phase);
  end setState_pTX;

  redeclare function extends setState_dTX
  algorithm
    // The composition is an empty vector
    state :=setState_dT(d, T, phase);
  end setState_dTX;

  redeclare function extends setState_psX
  algorithm
    // The composition is an empty vector
    state :=setState_ps(p, s, phase);
  end setState_psX;

  redeclare replaceable function density_ph "Return density from p and h"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Density d "Density";
  algorithm
    d := density(setState_ph(p, h, phase));
    annotation(derivative(noDerivative = phase) = density_ph_der,
               Inline = true);
  end density_ph;

  redeclare replaceable function density_pT "Return density from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Density d "Density";
  algorithm
    d := density(setState_pT(p, T, phase));
    annotation(derivative(noDerivative = phase) = density_pT_der,
               Inline = true);
  end density_pT;

  replaceable partial function density_pT_der "Total derivative of density_pT"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Real p_der;
    input Real T_der;
    output Real d_der;
    // To be implemented
    annotation(Inline = true);
  end density_pT_der;

  redeclare replaceable function density_ps "Return density from p and s"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Density d "Density";
  algorithm
    d := density(setState_ps(p, s, phase));
    annotation(derivative(noDerivative = phase) = density_ps_der,
               Inline = true);
  end density_ps;

  replaceable partial function density_ps_der "Total derivative of density_ps"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Real p_der;
    input Real h_der;
    output Real d_der;
    // To be implemented
    annotation(Inline = true);
  end density_ps_der;

  redeclare replaceable function temperature_ph
    "Return temperature from p and h"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Temperature T "Temperature";
  algorithm
    T := temperature(setState_ph(p, h, phase));
    annotation(Inline = true);
  end temperature_ph;

  redeclare replaceable function temperature_ps
    "Return temperature from p and s"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output Temperature T "Temperature";
  algorithm
    T := temperature(setState_ps(p, s, phase));
    annotation(Inline = true);
  end temperature_ps;

  replaceable function specificEntropy_ph
    "Return specific entropy from p and h"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output SpecificEntropy s "specific entropy";
  algorithm
    s := specificEntropy(setState_ph(p, h, phase));
    annotation(Inline = true);
  end specificEntropy_ph;

  redeclare replaceable function pressure_dT "Return pressure from d and T"
    extends Modelica.Icons.Function;
    input Density d "Density";
    input Temperature T "Temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output AbsolutePressure p "Pressure";
  algorithm
    p := pressure(setState_dT(d, T, phase));
    annotation(Inline = true);
  end pressure_dT;

  redeclare replaceable function specificEnthalpy_pT
    "Return specific enthalpy from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm
    h := specificEnthalpy(setState_pT(p, T, phase));
    annotation(Inline = true);
  end specificEnthalpy_pT;

  redeclare replaceable function specificEnthalpy_dT
    "Return specific enthalpy from d and T"
    extends Modelica.Icons.Function;
    input Density d "Density";
    input Temperature T "Temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm
    h := specificEnthalpy(setState_dT(d, T, phase));
    annotation(Inline = true);
  end specificEnthalpy_dT;

  redeclare replaceable function specificEnthalpy_ps
    "Return specific enthalpy from p and s"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm
    h := specificEnthalpy(setState_ps(p,s, phase));
    annotation(Inline = true);
  end specificEnthalpy_ps;

  replaceable partial function getMolarMass
    output MolarMass MM "molar mass";
    external "C" MM=  TwoPhaseMedium_getMolarMass_(mediumName, libraryName, substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getMolarMass;

  replaceable partial function getCriticalTemperature
    output Temperature Tc "Critical temperature";
    external "C" Tc=  TwoPhaseMedium_getCriticalTemperature_(mediumName, libraryName, substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalTemperature;

  replaceable partial function getCriticalPressure
    output AbsolutePressure pc "Critical temperature";
    external "C" pc=  TwoPhaseMedium_getCriticalPressure_(mediumName, libraryName, substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalPressure;

  replaceable partial function getCriticalMolarVolume
    output MolarVolume vc "Critical molar volume";
    external "C" vc=  TwoPhaseMedium_getCriticalMolarVolume_(mediumName, libraryName, substanceName)
      annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalMolarVolume;

  redeclare replaceable function setState_ph
    "Return thermodynamic state record from p and h"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    input SpecificEnthalpy h "specific enthalpy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  external "C" TwoPhaseMedium_setState_ph_(p, h, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_ph;

  redeclare replaceable function setState_pT
    "Return thermodynamic state record from p and T"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    input Temperature T "temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  external "C" TwoPhaseMedium_setState_pT_(p, T, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_pT;

  redeclare replaceable function setState_dT
    "Return thermodynamic state record from d and T"
    extends Modelica.Icons.Function;
    input Density d "density";
    input Temperature T "temperature";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  external "C" TwoPhaseMedium_setState_dT_(d, T, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_dT;

  redeclare replaceable function setState_ps
    "Return thermodynamic state record from p and s"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    input SpecificEntropy s "specific entropy";
    input FixedPhase phase = 0
      "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  external "C" TwoPhaseMedium_setState_ps_(p, s, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_ps;

  replaceable partial function setSat_p_state
    "Return saturation properties from the state"
    extends Modelica.Icons.Function;
    input ThermodynamicState state;
    output SaturationProperties sat "saturation property record";
  algorithm
    sat:=setSat_p(state.p);
    //Redeclare this function for more efficient implementations avoiding the repeated computation of saturation properties
    annotation(Inline = true);
  end setSat_p_state;

  redeclare replaceable function extends setDewState
    "set the thermodynamic state on the dew line"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation point";
    input FixedPhase phase =  1 "phase: default is one phase";
    output ThermodynamicState state "complete thermodynamic state info";
  external "C" TwoPhaseMedium_setDewState_(sat, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setDewState;

  redeclare replaceable function extends setBubbleState
    "set the thermodynamic state on the bubble line"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation point";
    input FixedPhase phase =  1 "phase: default is one phase";
    output ThermodynamicState state "complete thermodynamic state info";
  external "C" TwoPhaseMedium_setBubbleState_(sat, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setBubbleState;

  redeclare replaceable function extends density "Return density from state"
  algorithm
    d := state.d;
    annotation(Inline = true);
  end density;

  redeclare replaceable function extends pressure "Return pressure from state"
  algorithm
    p := state.p;
    annotation(Inline = true);
  end pressure;

  redeclare replaceable function extends specificEnthalpy
    "Return specific enthalpy from state"
  algorithm
    h := state.h;
    annotation(Inline = true);
  end specificEnthalpy;

  redeclare replaceable function extends specificEntropy
    "Return specific entropy from state"
  algorithm
    s := state.s;
    annotation(Inline = true);
  end specificEntropy;

  redeclare replaceable function extends temperature
    "Return temperature from state"
  algorithm
    T := state.T;
    annotation(Inline = true);
  end temperature;

  redeclare replaceable function extends isentropicEnthalpy
  external "C" h_is=  TwoPhaseMedium_isentropicEnthalpy_(p_downstream, refState,
   mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end isentropicEnthalpy;

  redeclare replaceable function extends isobaricExpansionCoefficient
    "Return isobaric expansion coefficient from state"
  algorithm
    beta := state.beta;
    annotation(Inline = true);
  end isobaricExpansionCoefficient;

  redeclare replaceable function extends isothermalCompressibility
    "Return isothermal compressibility from state"
  algorithm
    kappa := state.kappa;
    annotation(Inline = true);
  end isothermalCompressibility;

  redeclare replaceable function extends specificHeatCapacityCp
    "Return specific heat capacity cp from state"
  algorithm
    cp := state.cp;
    annotation(Inline = true);
  end specificHeatCapacityCp;

  redeclare replaceable function extends specificHeatCapacityCv
    "Return specific heat capacity cv from state"
  algorithm
    cv := state.cv;
    annotation(Inline = true);
  end specificHeatCapacityCv;

  redeclare replaceable function extends density_derp_h
    "Return derivative of density wrt pressure at constant enthalpy from state"
  algorithm
    ddph := state.ddph;
    annotation(Inline = true);
  end density_derp_h;

  redeclare replaceable function extends density_derh_p
    "Return derivative of density wrt enthalpy at constant pressure from state"
  algorithm
    ddhp := state.ddhp;
    annotation(Inline = true);
  end density_derh_p;

  redeclare replaceable function extends thermalConductivity
    "Return thermal conductivity from state"
  algorithm
    lambda := state.lambda;
    annotation(Inline = true);
  end thermalConductivity;

  redeclare replaceable function extends dynamicViscosity
    "Return dynamic viscosity from state"
  algorithm
    eta := state.eta;
    annotation(Inline = true);
  end dynamicViscosity;

  redeclare replaceable function extends velocityOfSound
    "Return velocity of sound from state"
  algorithm
    a := state.a;
    annotation(Inline = true);
  end velocityOfSound;

  replaceable function density_ph_der "Total derivative of density_ph"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase "2 for two-phase, 1 for one-phase, 0 if not known";
    input Real p_der;
    input Real h_der;
    output Real d_der;
  algorithm
    d_der:=density_derp_h(setState_ph(p, h))*p_der + density_derh_p(setState_ph(
      p, h))*h_der;
    annotation(Inline = true);
  end density_ph_der;

  redeclare replaceable function setSat_p "Return saturation properties from p"
    extends Modelica.Icons.Function;
    input AbsolutePressure p "pressure";
    output SaturationProperties sat "saturation property record";
  external "C" TwoPhaseMedium_setSat_p_(p, sat, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setSat_p;

  redeclare replaceable function setSat_T "Return saturation properties from p"
    extends Modelica.Icons.Function;
    input Temperature T "temperature";
    output SaturationProperties sat "saturation property record";
  external "C" TwoPhaseMedium_setSat_T_(T, sat, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setSat_T;

  redeclare replaceable function extends saturationPressure
  external "C" p=  TwoPhaseMedium_saturationPressure_(T, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationPressure;

  redeclare replaceable function extends saturationTemperature
  external "C" T=  TwoPhaseMedium_saturationTemperature_(p, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationTemperature;

  redeclare function extends saturationPressure_sat

    annotation(Inline = true);
  end saturationPressure_sat;

  redeclare function extends saturationTemperature_sat

    annotation(Inline = true);
  end saturationTemperature_sat;

  redeclare replaceable function extends saturationTemperature_derp
  external "C" dTp=  TwoPhaseMedium_saturationTemperature_derp_(p, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationTemperature_derp;

  redeclare replaceable function saturationTemperature_derp_sat
    "Returns derivative of saturation temperature w.r.t.. pressure"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation property record";
    output Real dTp "derivative of saturation temperature w.r.t. pressure";
  algorithm
    dTp := sat.dTp;
    annotation(Inline = true);
  end saturationTemperature_derp_sat;

  redeclare replaceable function extends bubbleDensity
    "Returns bubble point density"
  algorithm
    dl := sat.dl;
    annotation(Inline = true);
  end bubbleDensity;

  redeclare replaceable function extends dewDensity "Returns dew point density"
  algorithm
    dv := sat.dv;
    annotation(Inline = true);
  end dewDensity;

  redeclare replaceable function extends bubbleEnthalpy
    "Returns bubble point specific enthalpy"
  algorithm
    hl := sat.hl;
    annotation(Inline = true);
  end bubbleEnthalpy;

  redeclare replaceable function extends dewEnthalpy
    "Returns dew point specific enthalpy"
  algorithm
    hv := sat.hv;
    annotation(Inline = true);
  end dewEnthalpy;

  redeclare replaceable function extends bubbleEntropy
    "Returns bubble point specific entropy"
  algorithm
    sl := sat.sl;
    annotation(Inline = true);
  end bubbleEntropy;

  redeclare replaceable function extends dewEntropy
    "Returns dew point specific entropy"
  algorithm
    sv := sat.sv;
    annotation(Inline = true);
  end dewEntropy;

  redeclare replaceable function extends dBubbleDensity_dPressure
    "Returns bubble point density derivative"
  algorithm
    ddldp := sat.ddldp;
    annotation(Inline = true);
  end dBubbleDensity_dPressure;

  redeclare replaceable function extends dDewDensity_dPressure
    "Returns dew point density derivative"
  algorithm
    ddvdp := sat.ddvdp;
    annotation(Inline = true);
  end dDewDensity_dPressure;

  redeclare replaceable function extends dBubbleEnthalpy_dPressure
    "Returns bubble point specific enthalpy derivative"
  algorithm
    dhldp := sat.dhldp;
    annotation(Inline = true);
  end dBubbleEnthalpy_dPressure;

  redeclare replaceable function extends dDewEnthalpy_dPressure
    "Returns dew point specific enthalpy derivative"
  algorithm
    dhvdp := sat.dhvdp;
    annotation(Inline = true);
  end dDewEnthalpy_dPressure;

  redeclare replaceable function extends surfaceTension
    "Returns surface tension sigma in the two phase region"
  algorithm
    sigma := sat.sigma;
    annotation(Inline = true);
  end surfaceTension;

end ExternalTwoPhaseMedium;
