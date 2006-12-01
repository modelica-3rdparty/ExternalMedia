package Test 
  model CompleteFluidConstants "Compute all available medium fluid constants" 
    replaceable package Medium = 
        Modelica.Media.Interfaces.PartialTwoPhaseMedium;
    
    // Fluid constants
    Medium.Temperature Tc = Medium.fluidConstants[1].criticalTemperature;
    Medium.AbsolutePressure pc = Medium.fluidConstants[1].criticalPressure;
    Medium.MolarMass MM = Medium.fluidConstants[1].molarMass;
  end CompleteFluidConstants;

  model CompleteBaseProperties 
    "Compute all available two-phase medium properties from a BaseProperties model" 
    import SI = Modelica.SIunits;
    replaceable package Medium = 
        Modelica.Media.Interfaces.PartialTwoPhaseMedium;
    
    // BaseProperties object
    Medium.BaseProperties baseProperties;
    
    // All the complete properties
    CompleteThermodynamicState completeState(redeclare package Medium = Medium,
                                             state = baseProperties.state);
    CompleteSaturationProperties completeSat(redeclare package Medium = Medium,
                                             sat = baseProperties.sat);
    CompleteFluidConstants completeConstants(redeclare package Medium = Medium);
  end CompleteBaseProperties;
  
  model CompleteThermodynamicState 
    "Compute all available two-phase medium properties from a ThermodynamicState model" 
    import SI = Modelica.SIunits;
    replaceable package Medium = 
        Modelica.Media.Interfaces.PartialTwoPhaseMedium;
    
    // ThermodynamicState record
    Medium.ThermodynamicState state;
    
    // Critical properties
    Medium.Temperature Tc = Medium.fluidConstants[1].criticalTemperature;
    Medium.AbsolutePressure pc = Medium.fluidConstants[1].criticalPressure;
    
    // Medium properties
    Medium.Density d =                         Medium.density(state);
    Medium.AbsolutePressure p =                Medium.pressure(state);
    Medium.SpecificEnthalpy h =                Medium.specificEnthalpy(state);
    Medium.SpecificEntropy s =                 Medium.specificEntropy(state);
    Medium.Temperature T =                     Medium.temperature(state);
    Medium.IsobaricExpansionCoefficient beta = Medium.isobaricExpansionCoefficient(state);
    SI.IsothermalCompressibility kappa =       Medium.isothermalCompressibility(state);
    Medium.SpecificHeatCapacity cp =           Medium.specificHeatCapacityCp(state);
    Medium.SpecificHeatCapacity cv =           Medium.specificHeatCapacityCv(state);
    // Medium.DerDensityByPressure d_d_dp_h =     Medium.density_derp_h(state);
    // Medium.DerDensityByEnthalpy d_d_dh_p =     Medium.density_derh_p(state);
    Medium.MolarMass MM =                      Medium.molarMass(state);
    
  end CompleteThermodynamicState;
  
  model CompleteSaturationProperties 
    "Compute all available saturation properties from a SaturationProperties record" 
    replaceable package Medium = 
        Modelica.Media.Interfaces.PartialTwoPhaseMedium;
    
    // SaturationProperties record
    Medium.SaturationProperties sat;
    
    // Saturation properties
    Medium.Temperature Ts =      Medium.saturationTemperature_sat(sat);
    Medium.Density dl =          Medium.bubbleDensity(sat);
    Medium.Density dv =          Medium.dewDensity(sat);
    Medium.SpecificEnthalpy hl = Medium.bubbleEnthalpy(sat);
    Medium.SpecificEnthalpy hv = Medium.dewEnthalpy(sat);
    // Real d_Ts_dp =               Medium.saturationTemperature_derp_sat(sat);
    Real d_dl_dp =               Medium.dBubbleDensity_dPressure(sat);
    Real d_dv_dp =               Medium.dDewDensity_dPressure(sat);
    Real d_hl_dp =               Medium.dBubbleEnthalpy_dPressure(sat);
    Real d_hv_dp =               Medium.dDewEnthalpy_dPressure(sat);
  end CompleteSaturationProperties;

model TestWrongMedium 
    "Test the error reporting messages for unsupported external media" 
  package Medium = Media.ExternalTwoPhaseMedium;
  Medium.BaseProperties medium;
equation 
  medium.p = 1e5;
  medium.h = 1e5;
end TestWrongMedium;

  model TestBasePropertiesExplicit_TestMedium 
    "Test case using TestMedium and explicit equations" 
    replaceable package Medium = Media.TestMedium;
    ExternalMedia.Test.CompleteBaseProperties medium1(
                               redeclare package Medium = Medium) 
      "Constant pressure, varying enthalpy";
    ExternalMedia.Test.CompleteBaseProperties medium2(
                               redeclare package Medium = Medium) 
      "Varying pressure, constant enthalpy";
  equation 
    medium1.baseProperties.p = 1e5+1e5*time;
    medium1.baseProperties.h = 1e5;
    medium2.baseProperties.p = 1e5;
    medium2.baseProperties.h = 1e5 + 2e5*time;
  end TestBasePropertiesExplicit_TestMedium;
  
  model TestBasePropertiesImplicit_TestMedium 
    "Test case using TestMedium and implicit equations" 
    replaceable package Medium = Media.TestMedium;
    ExternalMedia.Test.CompleteBaseProperties medium1(
                               redeclare package Medium = Medium,
                               baseProperties(h(start=1e5))) 
      "Constant pressure, varying enthalpy";
    ExternalMedia.Test.CompleteBaseProperties medium2(
                               redeclare package Medium = Medium,
                               baseProperties(h(start=1e5))) 
      "Varying pressure, constant enthalpy";
  equation 
    medium1.baseProperties.p = 1e5*time;
    medium1.baseProperties.T = 300 + 25*time;
    medium2.baseProperties.p = 1e5+1e5*time;
    medium2.baseProperties.T = 300;
  end TestBasePropertiesImplicit_TestMedium;
  
model TestBasePropertiesDynamic_TestMedium 
    "Test case using TestMedium and dynamic equations" 
    import SI = Modelica.SIunits;
  replaceable package Medium = Media.TestMedium;
  parameter SI.Volume V = 1 "Storage Volume";
  parameter Real p_atm = 101325 "Atmospheric pressure";
  parameter SI.Temperature Tstart = 300;
  parameter Real Kv0 = 1.00801e-2 "Valve flow coefficient";
  Medium.BaseProperties medium(preferredMediumStates = true);
  SI.Mass M;
  SI.Energy U;
  SI.MassFlowRate win(start = 100);
  SI.MassFlowRate wout;
  SI.SpecificEnthalpy hin;
  SI.SpecificEnthalpy hout;
  SI.Power Q;
  Real Kv;
//  SI.Pressure p(stateSelect = StateSelect.prefer, start = 1e5);
//  SI.SpecificEnthalpy h(stateSelect = StateSelect.prefer, start = 2e5);
equation 
  // Mass & energy balance equation
  M = medium.d*V;
  U = medium.u*M;
  der(M) = win - wout;
  der(U) = win*hin - wout*hout + Q;
    
  // Preferred states
//  p = medium.p;
//  h = medium.h;
    
  // Inlet pump equations
  medium.p - p_atm = 2e5 - (1e5/100^2)*win^2;
  hin = 1e5;
    
  // Outlet valve equation
  wout = Kv * sqrt(medium.d*(medium.p - p_atm));
  hout = medium.h;
    
  // Input variables
  Kv = if time<50 then Kv0 else Kv0*1.1;
  Q = if time < 1 then 0 else 1e7;
initial equation 
  // Initial conditions
    
  // Fixed initial states
  // medium.p = 2e5;
  // medium.h = 1e5;
    
  // Steady state equations
  der(medium.p) = 0;
  der(medium.h) = 0;
  annotation (experiment(StopTime=80, Tolerance=1e-007),experimentSetupOutput(
        equdistant=false));
end TestBasePropertiesDynamic_TestMedium;

model TestAll_TestMedium "Test case using TestMedium and dynamic equations" 
  import SI = Modelica.SIunits;
  replaceable package Medium = Media.TestMedium;
  TestBasePropertiesExplicit_TestMedium testBasePropertiesExplicit(redeclare 
        package Medium =                                                                      Medium);
  TestBasePropertiesImplicit_TestMedium testBasePropertiesImplicit(redeclare 
        package Medium =                                                                      Medium);
  TestBasePropertiesDynamic_TestMedium testBasePropertiesDynamic(redeclare 
        package Medium =                                                                    Medium);
    
end TestAll_TestMedium;
  
  model TestBasePropertiesExplicit_FluidPropIF95 
    "Test case using TestMedium and explicit equations" 
    replaceable package Medium = Media.FluidPropMedia.WaterIF95;
    ExternalMedia.Test.CompleteBaseProperties medium1(
                               redeclare package Medium = Medium) 
      "Constant pressure, varying enthalpy";
    ExternalMedia.Test.CompleteBaseProperties medium2(
                               redeclare package Medium = Medium) 
      "Varying pressure, constant enthalpy";
  equation 
    medium1.baseProperties.p = 1e5+1e5*time;
    medium1.baseProperties.h = 1e5;
    medium2.baseProperties.p = 1e5;
    medium2.baseProperties.h = 1e5 + 2e5*time;
  end TestBasePropertiesExplicit_FluidPropIF95;

  model TestBasePropertiesImplicit_FluidPropIF95 
    "Test case using TestMedium and implicit equations" 
    replaceable package Medium = Media.FluidPropMedia.WaterIF95;
    ExternalMedia.Test.CompleteBaseProperties medium1(
                               redeclare package Medium = Medium,
                               baseProperties(h(start=1e5))) 
      "Constant pressure, varying enthalpy";
    ExternalMedia.Test.CompleteBaseProperties medium2(
                               redeclare package Medium = Medium,
                               baseProperties(h(start=1e5))) 
      "Varying pressure, constant enthalpy";
  equation 
    medium1.baseProperties.p = 1e5*time;
    medium1.baseProperties.T = 300 + 25*time;
    medium2.baseProperties.p = 1e5+1e5*time;
    medium2.baseProperties.T = 300;
  end TestBasePropertiesImplicit_FluidPropIF95;

model TestBasePropertiesDynamic_FluidPropIF95 
    "Test case using TestMedium and dynamic equations" 
  import SI = Modelica.SIunits;
  replaceable package Medium = Media.FluidPropMedia.WaterIF95;
  parameter SI.Volume V = 1 "Storage Volume";
  parameter Real p_atm = 101325 "Atmospheric pressure";
  parameter SI.Temperature Tstart = 300;
  parameter Real Kv0 = 1.00801e-2 "Valve flow coefficient";
  Medium.BaseProperties medium(preferredMediumStates = true);
  SI.Mass M;
  SI.Energy U;
  SI.MassFlowRate win(start = 100);
  SI.MassFlowRate wout;
  SI.SpecificEnthalpy hin;
  SI.SpecificEnthalpy hout;
  SI.Power Q;
  Real Kv;
//  SI.Pressure p(stateSelect = StateSelect.prefer, start = 1e5);
//  SI.SpecificEnthalpy h(stateSelect = StateSelect.prefer, start = 2e5);
equation 
  // Mass & energy balance equation
  M = medium.d*V;
  U = medium.u*M;
  der(M) = win - wout;
  der(U) = win*hin - wout*hout + Q;
    
  // Preferred states
//  p = medium.p;
//  h = medium.h;
    
  // Inlet pump equations
  medium.p - p_atm = 2e5 - (1e5/100^2)*win^2;
  hin = 1e5;
    
  // Outlet valve equation
  wout = Kv * sqrt(medium.d*(medium.p - p_atm));
  hout = medium.h;
    
  // Input variables
  Kv = if time<50 then Kv0 else Kv0*1.1;
  Q = if time < 1 then 0 else 1e7;
initial equation 
  // Initial conditions
    
  // Fixed initial states
  // medium.p = 2e5;
  // medium.h = 1e5;
    
  // Steady state equations
  der(medium.p) = 0;
  der(medium.h) = 0;
  annotation (experiment(StopTime=80, Tolerance=1e-007),experimentSetupOutput(
        equdistant=false));
end TestBasePropertiesDynamic_FluidPropIF95;
  
  
  
  
  
  
  
end Test;
