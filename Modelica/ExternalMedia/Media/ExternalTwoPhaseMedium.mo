package ExternalTwoPhaseMedium 
  "External two phase medium package - modify libraryName to set the external library" 
  extends ExternalMedia.Interfaces.PartialExternalTwoPhaseMedium;
  
  redeclare replaceable function extends createMedium 
  external "C" uniqueID=  TwoPhaseMedium_createMedium_(mediumName, libraryName, substanceName, oldUniqueID) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end createMedium;
  
  redeclare replaceable function extends getMolarMass 
  external "C" MM=  TwoPhaseMedium_getMolarMass_(mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getMolarMass;
  
  redeclare replaceable function extends getCriticalTemperature 
  external "C" Tc=  TwoPhaseMedium_getCriticalTemperature_(mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalTemperature;
  
  redeclare replaceable function extends getCriticalPressure 
  external "C" pc=  TwoPhaseMedium_getCriticalPressure_(mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalPressure;
  
  redeclare replaceable function extends getCriticalMolarVolume 
  external "C" vc=  TwoPhaseMedium_getCriticalMolarVolume_(mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end getCriticalMolarVolume;
  
  redeclare replaceable function extends setState_ph 
  external "C" TwoPhaseMedium_setState_ph_(p, h, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_ph;
  
  redeclare replaceable function extends setState_pT 
  external "C" TwoPhaseMedium_setState_pT_(p, T, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_pT;
  
  redeclare replaceable function extends setState_dT 
  external "C" TwoPhaseMedium_setState_dT_(d, T, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_dT;
  
  redeclare replaceable function extends setState_ps 
  external "C" TwoPhaseMedium_setState_ps_(p, s, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setState_ps;
  
  redeclare replaceable function extends setDewState 
  external "C" TwoPhaseMedium_setDewState_(sat.uniqueID, phase, state.uniqueID, state.phase, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setDewState;
  
  redeclare replaceable function extends setBubbleState 
  external "C" TwoPhaseMedium_setBubbleState_(sat.uniqueID, phase, state.uniqueID, state.phase, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setBubbleState;
  
  redeclare replaceable function extends density 
  external "C" d=  TwoPhaseMedium_density_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end density;
  
  redeclare replaceable function extends pressure 
  external "C" p=  TwoPhaseMedium_pressure_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end pressure;
  
  redeclare replaceable function extends specificEnthalpy 
  external "C" h=  TwoPhaseMedium_specificEnthalpy_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end specificEnthalpy;
  
  redeclare replaceable function extends specificEntropy 
    external "C" s=  TwoPhaseMedium_specificEntropy_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
    	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end specificEntropy;
  
  redeclare replaceable function extends temperature 
  external "C" T=  TwoPhaseMedium_temperature_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end temperature;
  
  redeclare replaceable function extends isentropicEnthalpy 
  external "C" h_is=  TwoPhaseMedium_isentropicEnthalpy_(p_downstream, refState.uniqueID, inputChoice, refState.d, refState.h, refState.p, refState.s, refState.T, refState.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end isentropicEnthalpy;
  
  redeclare replaceable function extends isobaricExpansionCoefficient 
  external "C" beta=  TwoPhaseMedium_isobaricExpansionCoefficient_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end isobaricExpansionCoefficient;
  
  redeclare replaceable function extends isothermalCompressibility 
  external "C" kappa=  TwoPhaseMedium_isothermalCompressibility_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end isothermalCompressibility;
  
  redeclare replaceable function extends specificHeatCapacityCp 
  external "C" cp=  TwoPhaseMedium_specificHeatCapacityCp_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end specificHeatCapacityCp;
  
  redeclare replaceable function extends specificHeatCapacityCv 
  external "C" cv=  TwoPhaseMedium_specificHeatCapacityCv_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end specificHeatCapacityCv;
  
  redeclare replaceable function extends density_derp_h 
  external "C" ddph=  TwoPhaseMedium_density_derp_h_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end density_derp_h;
  
  redeclare replaceable function extends density_derh_p 
    external "C" ddhp=  TwoPhaseMedium_density_derh_p_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
    	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end density_derh_p;
  
  redeclare replaceable function extends thermalConductivity 
  external "C" lambda=  TwoPhaseMedium_thermalConductivity_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end thermalConductivity;
  
  redeclare replaceable function extends dynamicViscosity 
  external "C" eta=  TwoPhaseMedium_dynamicViscosity_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dynamicViscosity;
  
  
  redeclare replaceable function extends velocityOfSound 
  external "C" a=  TwoPhaseMedium_velocityOfSound_(state.uniqueID, inputChoice, state.d, state.h, state.p, state.s, state.T, state.phase,
  	mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end velocityOfSound;
  
  redeclare replaceable function extends density_ph_der 
  external "C" d_der=  TwoPhaseMedium_density_ph_der_(uniqueID, p_der, h_der, p, h, phase, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end density_ph_der;
  
  redeclare replaceable function extends temperature_ph_der 
  external "C" T_der=  TwoPhaseMedium_temperature_ph_der_(uniqueID, p_der, h_der, p, h, phase, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end temperature_ph_der;
  
  redeclare replaceable function extends setSat_p 
  external "C" TwoPhaseMedium_setSat_p_(p, uniqueID, sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setSat_p;
  
  redeclare replaceable function extends setSat_T 
  external "C" TwoPhaseMedium_setSat_T_(T, uniqueID, sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setSat_T;
  
  redeclare replaceable function extends setSat_p_state 
  external "C" TwoPhaseMedium_setSat_p_state_(state.uniqueID, sat.psat, sat.Tsat, sat.uniqueID) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end setSat_p_state;
  
  redeclare replaceable function extends saturationPressure 
  external "C" p=  TwoPhaseMedium_saturationPressure_(T, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationPressure;
  
  redeclare replaceable function extends saturationTemperature 
  external "C" T=  TwoPhaseMedium_saturationTemperature_(p, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationTemperature;
  
  redeclare replaceable function extends saturationTemperature_derp 
  external "C" dTp=  TwoPhaseMedium_saturationTemperature_derp_(p, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationTemperature_derp;
  
  redeclare replaceable function saturationTemperature_derp_sat 
    "Returns derivative of saturation temperature w.r.t.. pressure" 
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation property record";
    output Real dTp "derivative of saturation temperature w.r.t. pressure";
  external "C" dTp=  TwoPhaseMedium_saturationTemperature_derp_sat_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end saturationTemperature_derp_sat;
  
  redeclare replaceable function extends bubbleDensity 
  external "C" dl=  TwoPhaseMedium_bubbleDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end bubbleDensity;
  
  redeclare replaceable function extends dewDensity 
  external "C" dv=  TwoPhaseMedium_dewDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dewDensity;
  
  redeclare replaceable function extends bubbleEnthalpy 
  external "C" hl=  TwoPhaseMedium_bubbleEnthalpy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end bubbleEnthalpy;
  
  redeclare replaceable function extends dewEnthalpy 
  external "C" hv=  TwoPhaseMedium_dewEnthalpy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dewEnthalpy;
  
  redeclare replaceable function extends bubbleEntropy 
  external "C" sl=  TwoPhaseMedium_bubbleEntropy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end bubbleEntropy;
  
  redeclare replaceable function extends dewEntropy 
  external "C" sv=  TwoPhaseMedium_dewDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dewEntropy;
  
  redeclare replaceable function extends dBubbleDensity_dPressure 
  external "C" ddldp=  TwoPhaseMedium_dBubbleDensity_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dBubbleDensity_dPressure;
  
  redeclare replaceable function extends dDewDensity_dPressure 
  external "C" ddvdp=  TwoPhaseMedium_dDewDensity_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dDewDensity_dPressure;
  
  redeclare replaceable function extends dBubbleEnthalpy_dPressure 
  external "C" dhldp=  TwoPhaseMedium_dBubbleEnthalpy_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dBubbleEnthalpy_dPressure;
  
  redeclare replaceable function extends dDewEnthalpy_dPressure 
  external "C" dhvdp=  TwoPhaseMedium_dDewEnthalpy_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end dDewEnthalpy_dPressure;
  
  redeclare replaceable function extends surfaceTension 
  external "C" sigma=  TwoPhaseMedium_surfaceTension_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) 
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
  end surfaceTension;
  
end ExternalTwoPhaseMedium;
