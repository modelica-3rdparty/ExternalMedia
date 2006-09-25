partial package PartialExternalTwoPhaseMedium 
  extends Modelica.Media.Interfaces.PartialTwoPhaseMedium;
  import IC = ExternalMedia.Common.InputChoices;
  
  constant IC.InputChoice inputChoice=IC.ph "input variables";
  
  redeclare replaceable record extends ThermodynamicState 
    Integer uniqueID "unique ID number";
  end ThermodynamicState;
  
  redeclare record extends SaturationProperties 
    Integer uniqueID "unique ID number";
  end SaturationProperties;
  
  redeclare function extends molarMass 
  algorithm 
    MM := ExternalFunctions.molarMass(state.uniqueID);
  end molarMass;
  
  redeclare function setSat_p 
    input AbsolutePressure p "pressure";
    output SaturationProperties sat "saturation property record";
  algorithm 
    sat := ExternalFunctions.setSat_p(p, 0);
  end setSat_p;
  
  redeclare function setSat_T 
    input Temperature T "temperature";
    output SaturationProperties sat "saturation property record";
  algorithm 
    sat := ExternalFunctions.setSat_T(T, 0);
  end setSat_T;
  
  redeclare function extends bubbleDensity 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dl=  bubbleDensity_(sat.uniqueID);
  end bubbleDensity;
  
  redeclare function extends dewDensity 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" dv=  dewDensity_(sat.uniqueID);
  end dewDensity;
  
  redeclare function extends bubbleEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" hl=  bubbleEnthalpy_(sat.uniqueID);
  end bubbleEnthalpy;
  
  redeclare function extends dewEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" hv=  dewEnthalpy_(sat.uniqueID);
  end dewEnthalpy;
  
  redeclare function extends bubbleEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sl=  bubbleEntropy_(sat.uniqueID);
  end bubbleEntropy;
  
  redeclare function extends dewEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" sv=  dewDensity_(sat.uniqueID);
  end dewEntropy;
  
  redeclare function setState_dT 
    input Density d "density";
    input Temperature T "temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  algorithm 
    state := ExternalFunctions.setState_dT(d, T, phase, 0);
  end setState_dT;
  
  redeclare function setState_ph 
    input AbsolutePressure p "pressure";
    input SpecificEnthalpy h "specific enthalpy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  algorithm 
    state := ExternalFunctions.setState_ph(p, h, phase, 0);
  end setState_ph;
  
  redeclare function setState_ps 
    input AbsolutePressure p "pressure";
    input SpecificEntropy s "specific entropy";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  algorithm 
    state := ExternalFunctions.setState_ps(p, s, phase, 0);
  end setState_ps;
  
  redeclare function setState_pT 
    input AbsolutePressure p "pressure";
    input Temperature T "temperature";
    input FixedPhase phase=0 "2 for two-phase, 1 for one-phase, 0 if not known";
    output ThermodynamicState state;
  algorithm 
    state := ExternalFunctions.setState_pT(p, T, phase, 0);
  end setState_pT;
  
  redeclare function extends density 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" d=  density_(state.uniqueID);
  end density;
  
  redeclare function extends pressure 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" p=  pressure_(state.uniqueID);
  end pressure;
  
  redeclare function extends specificEnthalpy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" h=  specificEnthalpy_(state.uniqueID);
  end specificEnthalpy;
  
  redeclare function extends specificEntropy 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" s=  specificEntropy_(state.uniqueID);
  end specificEntropy;
  
  redeclare function extends temperature 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" T=  temperature_(state.uniqueID);
  end temperature;
  
  redeclare function extends isobaricExpansionCoefficient 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" beta=  beta_(state.uniqueID);
  end isobaricExpansionCoefficient;
  
  redeclare function extends isothermalCompressibility 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" kappa=  kappa_(state.uniqueID);
  end isothermalCompressibility;
  
  redeclare function extends specificHeatCapacityCp 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" cp=  specificHeatCapacityCp_(state.uniqueID);
  end specificHeatCapacityCp;
  
  redeclare function extends specificHeatCapacityCv 
    
    annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  external "C" cv=  specificHeatCapacityCv_(state.uniqueID);
  end specificHeatCapacityCv;
  
  model extends BaseProperties 
    import IC = ExternalMedia.Common.InputChoices;
    
    parameter IC.InputChoice basePropertiesInputChoice=inputChoice 
      "Choice of input variables";
    
    Integer uniqueID(final start=0) "Unique ID of BaseProperty object";
    SpecificEntropy s "Specific entropy";
    
    SaturationProperties sat "saturation property record";
    
    parameter FixedPhase phase=0 
      "2 for two-phase, 1 for one-phase, 0 if not known";
  algorithm 
    when (initial()) then
      uniqueID := ExternalFunctions.createMedium(uniqueID);
      assert(uniqueID>0, "Error in external medium library");
      MM := ExternalFunctions.molarMass(uniqueID);
      R := Modelica.Constants.R/max(MM,1e-12);
    end when;
  equation 
    if (basePropertiesInputChoice == IC.dT) then
      state = ExternalFunctions.setState_dT(d, T, phase, uniqueID);
      sat = ExternalFunctions.setSat_T(T, uniqueID);
      h = specificEnthalpy(state);
      p = pressure(state);
      s = specificEntropy(state);
    elseif (basePropertiesInputChoice == IC.ph) then
      state = ExternalFunctions.setState_ph(p, h, phase, uniqueID);
      sat = ExternalFunctions.setSat_p(p, uniqueID);
      d = density(state);
      s = specificEntropy(state);
      T = temperature(state);
    elseif (basePropertiesInputChoice == IC.ps) then
      state = ExternalFunctions.setState_ps(p, s, phase, uniqueID);
      sat = ExternalFunctions.setSat_p(p, uniqueID);
      d = density(state);
      h = specificEnthalpy(state);
      T = temperature(state);
    elseif (basePropertiesInputChoice == IC.pT) then
      state = ExternalFunctions.setState_pT(p, T, phase, uniqueID);
      sat = ExternalFunctions.setSat_p(p, uniqueID);
      d = density(state);
      h = specificEnthalpy(state);
      s = specificEntropy(state);
    end if;
    u = h - p/max(d,1e-12);
  end BaseProperties;
  
  replaceable package ExternalFunctions 
    replaceable function createMedium 
      input Integer oldUniqueID "old unique ID number";
      output Integer uniqueID "unique ID number";
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" uniqueID = createMedium_(mediumName, oldUniqueID);
    end createMedium;
    
    replaceable function deleteMedium 
      input Integer uniqueID "unique ID number";
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" deleteMedium_(uniqueID);
    end deleteMedium;
    
    replaceable function setSat_p 
      input AbsolutePressure p "pressure";
      input Integer uniqueID "unique ID number";
      output SaturationProperties sat "saturation property record";
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" setSat_p_(p, uniqueID, sat.psat, sat.Tsat, sat.uniqueID);
    end setSat_p;
    
    replaceable function setSat_T 
      input Temperature T "temperature";
      input Integer uniqueID "unique ID number";
      output SaturationProperties sat "saturation property record";
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" setSat_T_(T, uniqueID, sat.psat, sat.Tsat, sat.uniqueID);
    end setSat_T;
    
    replaceable function setState_dT 
      input Density d "density";
      input Temperature T "temperature";
      input FixedPhase phase=0 
        "2 for two-phase, 1 for one-phase, 0 if not known";
      input Integer uniqueID "unique ID number";
      output ThermodynamicState state;
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" setState_dT_(d, T, phase, uniqueID, state.uniqueID, state.phase);
    end setState_dT;
    
    replaceable function setState_ph 
      input AbsolutePressure p "pressure";
      input SpecificEnthalpy h "specific enthalpy";
      input FixedPhase phase=0 
        "2 for two-phase, 1 for one-phase, 0 if not known";
      input Integer uniqueID "unique ID number";
      output ThermodynamicState state;
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" setState_ph_(p, h, phase, uniqueID, state.uniqueID, state.phase);
    end setState_ph;
    
    replaceable function setState_ps 
      input AbsolutePressure p "pressure";
      input SpecificEntropy s "specific entropy";
      input FixedPhase phase=0 
        "2 for two-phase, 1 for one-phase, 0 if not known";
      input Integer uniqueID "unique ID number";
      output ThermodynamicState state;
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" setState_ps_(p, s, phase, uniqueID, state.uniqueID, state.phase);
    end setState_ps;
    
    replaceable function setState_pT 
      input AbsolutePressure p "pressure";
      input Temperature T "temperature";
      input FixedPhase phase=0 
        "2 for two-phase, 1 for one-phase, 0 if not known";
      input Integer uniqueID "unique ID number";
      output ThermodynamicState state;
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" setState_pT_(p, T, phase, uniqueID, state.uniqueID, state.phase);
    end setState_pT;
    
    replaceable function molarMass 
      input Integer uniqueID "unique ID number";
      output MolarMass MM "molar mass";
      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
    external "C" MM = molarMass_(uniqueID);
    end molarMass;
  end ExternalFunctions;
end PartialExternalTwoPhaseMedium;

