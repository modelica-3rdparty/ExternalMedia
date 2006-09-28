partial package ExternalTwoPhaseMedium 
  extends Modelica.Media.Interfaces.PartialTwoPhaseMedium;
  import IC = ExternalMedia.Common.InputChoices;
  
  constant IC.InputChoice inputChoice=IC.ph 
    "Default choice of input variables for property computations";
  
  redeclare replaceable record extends ThermodynamicState 
    Integer uniqueID "unique ID number";
  end ThermodynamicState;
  
  redeclare record extends SaturationProperties 
    Integer uniqueID "unique ID number";
  end SaturationProperties;
  
  redeclare model extends BaseProperties 
    import IC = ExternalMedia.Common.InputChoices;
    parameter IC.InputChoice basePropertiesInputChoice=inputChoice 
      "Choice of input variables for property computations";
    parameter FixedPhase phase=0 
      "2 for two-phase, 1 for one-phase, 0 if not known";
    
    Integer uniqueID(final start=0) "Unique ID of BaseProperty object";
    SpecificEntropy s "Specific entropy";
    SaturationProperties sat "saturation property record";
  algorithm 
    when (initial()) then
      uniqueID := createMedium(uniqueID);
      assert(uniqueID>0, "Error in external medium library");
      MM := getMolarMass(uniqueID);
      R := Modelica.Constants.R/max(MM,1e-12);
    end when;
  equation 
    if (basePropertiesInputChoice == IC.ph) then
      // Compute the state record (including the unique ID)
      // and the sat record
      state = setState_ph(p, h, phase, uniqueID);
      sat = setSat_p(p, uniqueID);
      // Compute the remaining variables.
      // It is not possible to use the standard functions like
      // d = density(state), because differentiation for index
      // reduction and change of state variables would not be supported
      // density_ph(), which has an appropriate derivative annotation,
      // is used instead. The implementation of density_ph() uses 
      // setState with the same inputs, so there's no actual overhead
      d = density_ph(p, h, state.phase, state.uniqueID);
      s = specificEntropy_ph(p, h, state.phase, state.uniqueID);
      T = temperature_ph(p, h, state.phase, state.uniqueID);
    elseif (basePropertiesInputChoice == IC.dT) then
      state = setState_dT(d, T, phase, uniqueID);
      sat = setSat_T(T, uniqueID);
      h = specificEnthalpy(state);
      p = pressure(state);
      s = specificEntropy(state);
    elseif (basePropertiesInputChoice == IC.pT) then
      state = setState_pT(p, T, phase, uniqueID);
      sat = setSat_p(p, uniqueID);
      d = density(state);
      h = specificEnthalpy(state);
      s = specificEntropy(state);
      /*
    elseif (basePropertiesInputChoice == IC.ps) then
      state = setState_ps(p, s, phase, uniqueID);
      sat = setSat_p(p, uniqueID);
      d = density(state);
      h = specificEnthalpy(state);
      T = temperature(state);
*/
    end if;
    u = h - p/max(d,1e-12);
  end BaseProperties;
  
  redeclare replaceable function setState_ph 
    input AbsolutePressure p "pressure";
    input SpecificEnthalpy h "specific enthalpy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID "unique ID number";
    output ThermodynamicState state;
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_ph_(p, h, phase, uniqueID, state.uniqueID, state.phase);
  end setState_ph;
  
  redeclare replaceable function setState_pT 
    input AbsolutePressure p "pressure";
    input Temperature T "temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID "unique ID number";
    output ThermodynamicState state;
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_pT_(p, T, phase, uniqueID, state.uniqueID, state.phase);
  end setState_pT;
  
  redeclare replaceable function setState_dT 
    input Density d "density";
    input Temperature T "temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID "unique ID number";
    output ThermodynamicState state;
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_dT_(d, T, phase, uniqueID, state.uniqueID, state.phase);
  end setState_dT;
  
  redeclare replaceable function setState_ps 
    input AbsolutePressure p "pressure";
    input SpecificEntropy s "specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID "unique ID number";
    output ThermodynamicState state;
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setState_ps_(p, s, phase, uniqueID, state.uniqueID, state.phase);
  end setState_ps;
  
  redeclare replaceable function setSat_p 
    input AbsolutePressure p "pressure";
    input Integer uniqueID "unique ID number";
    output SaturationProperties sat "saturation property record";
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setSat_p_(p, uniqueID, sat.psat, sat.Tsat, sat.uniqueID);
  end setSat_p;
  
  redeclare replaceable function setSat_T 
    input Temperature T "temperature";
    input Integer uniqueID "unique ID number";
    output SaturationProperties sat "saturation property record";
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" setSat_T_(T, uniqueID, sat.psat, sat.Tsat, sat.uniqueID);
  end setSat_T;
  
  redeclare function extends molarMass 
  algorithm 
    MM := getMolarMass(state.uniqueID);
  end molarMass;
  
  redeclare replaceable function extends density 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" d=  density_(state.uniqueID);
  end density;
  
  redeclare replaceable function extends pressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" p=  pressure_(state.uniqueID);
  end pressure;
  
  redeclare replaceable function extends specificEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" h=  specificEnthalpy_(state.uniqueID);
  end specificEnthalpy;
  
  redeclare replaceable function extends specificEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" s=  specificEntropy_(state.uniqueID);
  end specificEntropy;
  
  redeclare replaceable function extends temperature 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" T=  temperature_(state.uniqueID);
  end temperature;
  
  redeclare replaceable function extends isobaricExpansionCoefficient 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" beta=  beta_(state.uniqueID);
  end isobaricExpansionCoefficient;
  
  redeclare replaceable function extends isothermalCompressibility 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" kappa=  kappa_(state.uniqueID);
  end isothermalCompressibility;
  
  redeclare replaceable function extends specificHeatCapacityCp 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" cp=  specificHeatCapacityCp_(state.uniqueID);
  end specificHeatCapacityCp;
  
  redeclare replaceable function extends specificHeatCapacityCv 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" cv=  specificHeatCapacityCv_(state.uniqueID);
  end specificHeatCapacityCv;
  
  redeclare replaceable function extends bubbleDensity 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dl=  bubbleDensity_(sat.uniqueID);
  end bubbleDensity;
  
  redeclare replaceable function extends dewDensity 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dv=  dewDensity_(sat.uniqueID);
  end dewDensity;
  
  redeclare replaceable function extends bubbleEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" hl=  bubbleEnthalpy_(sat.uniqueID);
  end bubbleEnthalpy;
  
  redeclare replaceable function extends dewEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" hv=  dewEnthalpy_(sat.uniqueID);
  end dewEnthalpy;
  
  redeclare replaceable function extends bubbleEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sl=  bubbleEntropy_(sat.uniqueID);
  end bubbleEntropy;
  
  redeclare replaceable function extends dewEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sv=  dewDensity_(sat.uniqueID);
  end dewEntropy;
  
  replaceable function createMedium 
    input Integer oldUniqueID "old unique ID number";
    output Integer uniqueID "unique ID number";
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" uniqueID=  createMedium_(mediumName, oldUniqueID);
  end createMedium;
  
  replaceable function deleteMedium 
    input Integer uniqueID "unique ID number";
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" deleteMedium_(uniqueID);
  end deleteMedium;
  
  replaceable function getMolarMass 
    input Integer uniqueID "unique ID number";
    output MolarMass MM "molar mass";
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" MM=  molarMass_(uniqueID);
  end getMolarMass;
  
  redeclare function extends setState_phX 
    input Integer uniqueID = 0 "Unique ID";
  algorithm 
    // The composition is an empty vector
    state :=setState_ph(p, h, phase, uniqueID);
  end setState_phX;
  
  redeclare function extends setState_pTX 
    input Integer uniqueID = 0 "Unique ID";
  algorithm 
    // The composition is an empty vector
    state :=setState_pT(p, T, phase, uniqueID);
  end setState_pTX;
  
  redeclare function extends setState_dTX 
    input Integer uniqueID = 0 "Unique ID";
  algorithm 
    // The composition is an empty vector
    state :=setState_dT(d, T, phase, uniqueID);
  end setState_dTX;
  
  redeclare function extends setState_psX 
    input Integer uniqueID = 0 "Unique ID";
  algorithm 
    // The composition is an empty vector
    state :=setState_ps(p, s, phase, uniqueID);
  end setState_psX;
  
  redeclare replaceable function density_ph "Return density from p and h" 
    
      annotation(erivative = density_ph_der);
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output Density d "Density";
  algorithm 
    d := density(setState_ph(p, h, phase, uniqueID));
  end density_ph;
  
  redeclare replaceable function temperature_ph 
    "Return temperature from p and h" 
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output Temperature T "Temperature";
  algorithm 
    T := temperature(setState_ph(p, h, phase, uniqueID));
  end temperature_ph;
  
  replaceable function specificEntropy_ph 
    "Return specific entropy from p and h" 
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEnthalpy h "Specific enthalpy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output SpecificEntropy s "specific entropy";
  algorithm 
    s := specificEntropy(setState_ph(p, h, phase, uniqueID));
  end specificEntropy_ph;
  
  redeclare replaceable function specificEnthalpy_pT 
    "Return specific enthalpy from p and T" 
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm 
    h := specificEnthalpy(setState_pT(p, T, phase, uniqueID));
  end specificEnthalpy_pT;
  
  redeclare replaceable function density_pT "Return density from p and T" 
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output Density d "Density";
  algorithm 
    d := density(setState_pT(p, T, phase, uniqueID));
  end density_pT;
  
/*
  redeclare replaceable function specificEntropy_pT 
    "Return specific entropy from p and T" 
    extends Modelica.Icons.Function;
    input Pressure p "Pressure";
    input Temperature T "Temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output SpecificEntropy s "specific entropy";
  algorithm 
    s := specificEntropy(setState_pT(p, T, phase, uniqueID));
  end specificEntropy_ph;
*/
  
  redeclare replaceable function pressure_dT "Return pressure from d and T" 
    extends Modelica.Icons.Function;
    input Density d "Density";
    input Temperature T "Temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output AbsolutePressure p "Pressure";
  algorithm 
    p := pressure(setState_dT(d, T, phase, uniqueID));
  end pressure_dT;
  
  redeclare replaceable function specificEnthalpy_dT 
    "Return specific enthalpy from d and T" 
    extends Modelica.Icons.Function;
    input Density d "Density";
    input Temperature T "Temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm 
    h := specificEnthalpy(setState_dT(d, T, phase, uniqueID));
  end specificEnthalpy_dT;
  
/*
  redeclare replaceable function specificEntropy_dT 
    "Return specific entropy from d and T" 
    extends Modelica.Icons.Function;
    input Density d "Density";
    input Temperature T "Temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output SpecificEntropy s "specific entropy";
  algorithm 
    s := specificEntropy(setState_dt(d, T, phase, uniqueID));
  end specificEntropy_ph;
*/
  
  redeclare replaceable function specificEnthalpy_ps 
    "Return specific enthalpy from p and s" 
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output SpecificEnthalpy h "specific enthalpy";
  algorithm 
    h := specificEnthalpy(setState_ps(p,s, phase, uniqueID));
  end specificEnthalpy_ps;
  
  redeclare replaceable function temperature_ps 
    "Return temperature from p and s" 
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output Temperature T "Temperature";
  algorithm 
    T := temperature(setState_ps(p, s, phase, uniqueID));
  end temperature_ps;
  
  redeclare replaceable function density_ps "Return density from p and s" 
    extends Modelica.Icons.Function;
    input AbsolutePressure p "Pressure";
    input SpecificEntropy s "Specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    input Integer uniqueID = 0 "Unique ID";
    output Density d "Density";
  algorithm 
    d := density(setState_ps(p, s, phase, uniqueID));
  end density_ps;
  
end ExternalTwoPhaseMedium;
