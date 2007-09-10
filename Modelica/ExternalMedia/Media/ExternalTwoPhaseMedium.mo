within ExternalMedia.Media;
package ExternalTwoPhaseMedium 
  "External two phase medium package - modify libraryName to set the external library" 
  extends ExternalMedia.Interfaces.PartialExternalTwoPhaseMedium;
  redeclare replaceable function extends setState_ph 
    
  external "C" setState_ph_(p, h, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setState_ph;
  
  redeclare replaceable function extends setState_pT 
    
  external "C" setState_pT_(p, T, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setState_pT;
  
  redeclare replaceable function extends setState_dT 
    
  external "C" setState_dT_(d, T, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setState_dT;
  
  redeclare replaceable function extends setState_ps 
    
  external "C" setState_ps_(p, s, phase, uniqueID, state.uniqueID, state.phase, state.d, state.h, state.p, state.s, state.T, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setState_ps;
  
  redeclare replaceable function extends setSat_p 
    
  external "C" setSat_p_(p, uniqueID, sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setSat_p;
  
  redeclare replaceable function extends setSat_T 
    
  external "C" setSat_T_(T, uniqueID, sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setSat_T;
  
  redeclare replaceable function extends setSat_p_state 
    
  external "C" setSat_p_state_(state.uniqueID, sat.psat, sat.Tsat, sat.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setSat_p_state;
  
  redeclare replaceable function extends setDewState 
    
  external "C" setDewState_(sat.uniqueID, phase, state.uniqueID, state.phase, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setDewState;
  
  redeclare replaceable function extends setBubbleState 
    
  external "C" setBubbleState_(sat.uniqueID, phase, state.uniqueID, state.phase, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end setBubbleState;
  
  redeclare replaceable function extends saturationPressure 
    
  external "C" p=  saturationPressure_(T, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end saturationPressure;
  
  redeclare replaceable function extends saturationTemperature 
    
  external "C" T=  saturationTemperature_(p, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end saturationTemperature;
  
  redeclare replaceable function extends saturationTemperature_derp 
    
  external "C" dTp=  saturationTemperature_derp_(p, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end saturationTemperature_derp;
  
  redeclare replaceable function saturationTemperature_derp_sat 
    "Returns derivative of saturation temperature w.r.t.. pressure" 
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation property record";
    output Real dTp "derivative of saturation temperature w.r.t. pressure";
  external "C" dTp=  saturationTemperature_derp_sat_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end saturationTemperature_derp_sat;
  
  redeclare replaceable function extends density 
    
  external "C" d=  density_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end density;
  
  redeclare replaceable function extends pressure 
    
  external "C" p=  pressure_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end pressure;
  
  redeclare replaceable function extends specificEnthalpy 
    
  external "C" h=  specificEnthalpy_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end specificEnthalpy;
  
  redeclare replaceable function extends specificEntropy 
    
  external "C" s=  specificEntropy_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end specificEntropy;
  
  redeclare replaceable function extends temperature 
    
  external "C" T=  temperature_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end temperature;
  
  redeclare replaceable function extends isentropicEnthalpy 
    
  external "C" h_is=  isentropicEnthalpy_(p_downstream, refState.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end isentropicEnthalpy;
  
  redeclare replaceable function extends isobaricExpansionCoefficient 
    
  external "C" beta=  isobaricExpansionCoefficient_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end isobaricExpansionCoefficient;
  
  redeclare replaceable function extends isothermalCompressibility 
    
  external "C" kappa=  isothermalCompressibility_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end isothermalCompressibility;
  
  redeclare replaceable function extends specificHeatCapacityCp 
    
  external "C" cp=  specificHeatCapacityCp_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end specificHeatCapacityCp;
  
  redeclare replaceable function extends specificHeatCapacityCv 
    
  external "C" cv=  specificHeatCapacityCv_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end specificHeatCapacityCv;
  
  redeclare replaceable function extends density_derp_h 
    
    external "C" ddph=  density_derp_h_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end density_derp_h;
  
  redeclare replaceable function extends density_derh_p 
    
    external "C" ddhp=  density_derh_p_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end density_derh_p;
  
  redeclare replaceable function extends bubbleDensity 
    
  external "C" dl=  bubbleDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end bubbleDensity;
  
  redeclare replaceable function extends dewDensity 
    
  external "C" dv=  dewDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end dewDensity;
  
  redeclare replaceable function extends bubbleEnthalpy 
    
  external "C" hl=  bubbleEnthalpy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end bubbleEnthalpy;
  
  redeclare replaceable function extends dewEnthalpy 
    
  external "C" hv=  dewEnthalpy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end dewEnthalpy;
  
  redeclare replaceable function extends bubbleEntropy 
    
  external "C" sl=  bubbleEntropy_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end bubbleEntropy;
  
  redeclare replaceable function extends dewEntropy 
    
  external "C" sv=  dewDensity_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end dewEntropy;
  
  redeclare replaceable function extends dBubbleDensity_dPressure 
    
  external "C" ddldp=  dBubbleDensity_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end dBubbleDensity_dPressure;
  
  redeclare replaceable function extends dDewDensity_dPressure 
    
  external "C" ddvdp=  dDewDensity_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end dDewDensity_dPressure;
  
  redeclare replaceable function extends dBubbleEnthalpy_dPressure 
    
  external "C" dhldp=  dBubbleEnthalpy_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end dBubbleEnthalpy_dPressure;
  
  redeclare replaceable function extends dDewEnthalpy_dPressure 
    
  external "C" dhvdp=  dDewEnthalpy_dPressure_(sat.psat, sat.Tsat, sat.uniqueID, mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end dDewEnthalpy_dPressure;
  
  redeclare replaceable function extends density_ph_der 
    
  external "C" d_der=  density_ph_der_(uniqueID, p_der, h_der) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end density_ph_der;
  
  redeclare replaceable function extends temperature_ph_der 
    
  external "C" T_der=  temperature_ph_der_(uniqueID, p_der, h_der) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end temperature_ph_der;
  
  redeclare replaceable function extends createMedium 
    
  external "C" uniqueID=  createMedium_(mediumName, libraryName, substanceName, oldUniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end createMedium;
  
  redeclare replaceable function extends getMolarMass 
    
  external "C" MM=  getMolarMass_(mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end getMolarMass;
  
  redeclare replaceable function extends getCriticalTemperature 
    
  external "C" Tc=  getCriticalTemperature_(mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end getCriticalTemperature;
  
  redeclare replaceable function extends getCriticalPressure 
    
  external "C" pc=  getCriticalPressure_(mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end getCriticalPressure;
  
  redeclare replaceable function extends getCriticalMolarVolume 
    
  external "C" vc=  getCriticalMolarVolume_(mediumName, libraryName, substanceName) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end getCriticalMolarVolume;
  
  redeclare replaceable function extends thermalConductivity 
    
  external "C" lambda=  thermalConductivity_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end thermalConductivity;
  
  redeclare replaceable function extends dynamicViscosity 
    
  external "C" eta=  dynamicViscosity_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end dynamicViscosity;
  
  redeclare replaceable function extends prandtlNumber 
    
  external "C" Pr=  prandtlNumber_(state.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end prandtlNumber;
  
  redeclare replaceable function extends surfaceTension 
    
  external "C" sigma=  surfaceTension_(sat.psat, sat.Tsat, sat.uniqueID) annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end surfaceTension;
  
  redeclare replaceable function extends velocityOfSound 
    
  external "C" a=  velocityOfSound_(state.uniqueID)                      annotation(Include="#include \"externaltwophasemedium.h\"", Library="ExternalTwoPhaseMedium");
  end velocityOfSound;
  
end ExternalTwoPhaseMedium;
