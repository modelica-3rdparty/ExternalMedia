package Test
  model CompleteProperties "Compute all available two-phase medium properties" 
    replaceable package Medium = 
        Modelica.Media.Interfaces.PartialTwoPhaseMedium;
    
    Medium.BaseProperties baseProperties;
    
    Medium.Temperature Tc = Medium.fluidConstants[1].criticalTemperature;
    Medium.AbsolutePressure pc = Medium.fluidConstants[1].criticalPressure;
    Medium.Temperature Ts = baseProperties.sat.Tsat;
    Medium.Density dl = Medium.bubbleDensity(baseProperties.sat);
    Medium.Density dv = Medium.dewDensity(baseProperties.sat);
    Medium.SpecificEnthalpy hl = Medium.bubbleEnthalpy(baseProperties.sat);
    Medium.SpecificEnthalpy hv = Medium.dewEnthalpy(baseProperties.sat);
    // Real d_Ts_dp = Medium.saturationTemperature_derp(baseProperties.sat);
    Real d_dl_dp = Medium.dBubbleDensity_dPressure(baseProperties.sat);
    Real d_dv_dp = Medium.dDewDensity_dPressure(baseProperties.sat);
    Real d_hl_dp = Medium.dBubbleEnthalpy_dPressure(baseProperties.sat);
    Real d_hv_dp = Medium.dDewEnthalpy_dPressure(baseProperties.sat);
  end CompleteProperties;
  
  model CompleteThermodynamicState 
    "Compute all available properties from the state" 
    replaceable package Medium = 
        Modelica.Media.Interfaces.PartialTwoPhaseMedium;
    Medium.ThermodynamicState state;
    Medium.Temperature T = Medium.temperature(state);
    Medium.Density d = Medium.density(state);
    Medium.SpecificEnthalpy h = Medium.specificEnthalpy(state);
    Medium.SpecificEntropy s = Medium.specificEntropy(state);
  end CompleteThermodynamicState;
  
  model TestMediumExplicit "Test case using TestMedium and explicit equations" 
    replaceable package Medium = Media.TestMedium;
    CompleteProperties medium1(redeclare package Medium = Medium) 
      "Constant pressure, varying enthalpy";
    CompleteProperties medium2(redeclare package Medium = Medium) 
      "Varying pressure, constant enthalpy";
  equation 
    medium1.baseProperties.p = 1e5+1e5*time;
    medium1.baseProperties.h = 1e5;
    medium2.baseProperties.p = 1e5;
    medium2.baseProperties.h = 1e5 + 2e5*time;
  end TestMediumExplicit;
  
  model TestMediumImplicit "Test case using TestMedium and implicit equations" 
    replaceable package Medium = Media.TestMedium;
    CompleteProperties medium1(redeclare package Medium = Medium,
                               baseProperties(h(start=1e5))) 
      "Constant pressure, varying enthalpy";
    CompleteProperties medium2(redeclare package Medium = Medium,
                               baseProperties(h(start=1e5))) 
      "Varying pressure, constant enthalpy";
  equation 
    medium1.baseProperties.p = 1e5*time;
    medium1.baseProperties.T = 300 + 25*time;
    medium2.baseProperties.p = 1e5+1e5*time;
    medium2.baseProperties.T = 300;
  end TestMediumImplicit;
  
model TestMediumDynamic "Test case using TestMedium and dynamic equations" 
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
end TestMediumDynamic;
  
model FluidPropIF95Explicit 
    "Test case using FluidProp - RefProp IF95 medium and explicit equations" 
  extends TestMediumExplicit(redeclare package Medium = 
        Media.FluidPropMedia.WaterIF95);
  annotation (experiment(NumberOfIntervals=50), experimentSetupOutput);
end FluidPropIF95Explicit;
  
model FluidPropIF95Implicit 
    "Test case using FluidProp - RefProp IF95 medium and implicit equations" 
  extends TestMediumImplicit(redeclare package Medium = 
        Media.FluidPropMedia.WaterIF95);
end FluidPropIF95Implicit;
  
model FluidPropIF95Dynamic "Test case using TestMedium and dynamic equations" 
  extends TestMediumDynamic(redeclare package Medium = 
        Media.FluidPropMedia.WaterIF95);
    annotation (experiment(StopTime=80, Tolerance=1e-007),
        experimentSetupOutput(equdistant=false));
end FluidPropIF95Dynamic;
  
model WrongMedium 
    "Test the error reporting messages for unsupported external media" 
  extends TestMediumExplicit(redeclare package Medium = 
        Media.ExternalTwoPhaseMedium);
end WrongMedium;
  
model TestWaterExplicit "Test case using TestMedium and explicit equations" 
  replaceable package Medium = Modelica.Media.Water.StandardWater;
  Medium.BaseProperties medium;
  Medium.BaseProperties medium2;
  Real Ts;
  Real dl;
  Real dv;
  Real hl;
  Real hv;
  Real d_Ts_dp;
  Real d_dl_dp;
  Real d_dv_dp;
  Real d_hl_dp;
  Real d_hv_dp;
    
equation 
//  medium.p = 1e5;
//  medium.h = 1e5 + 2e5*time;
  medium.p = 10e5+1e5*time;
  medium.h = 1e5;
  Ts = medium.sat.Tsat;
  dl = Medium.bubbleDensity(medium.sat);
  dv = Medium.dewDensity(medium.sat);
  hl = Medium.bubbleEnthalpy(medium.sat);
  hv = Medium.dewEnthalpy(medium.sat);
  d_Ts_dp = Medium.saturationTemperature_derp(medium.p);
  d_dl_dp = Medium.dBubbleDensity_dPressure(medium.sat);
  d_dv_dp = Medium.dDewDensity_dPressure(medium.sat);
  d_hl_dp = Medium.dBubbleEnthalpy_dPressure(medium.sat);
  d_hv_dp = Medium.dDewEnthalpy_dPressure(medium.sat);
  medium2.p = 1e5;
  medium2.h = 1e5 + 1e5*time;
    
end TestWaterExplicit;
  
  model TestSaturationProperties 
    "Test saturation properties without supplying unique ID" 
    replaceable package Medium = 
        ExternalMedia.Media.TestMedium;
    
    Medium.SaturationProperties sat "saturation property record";
    
  equation 
    sat = Medium.setSat_T(380.0);
    
  end TestSaturationProperties;
  
  model TestSaturationProperties2 
    "Test saturation properties without supplying unique ID" 
    replaceable package Medium = 
        ExternalMedia.Media.TestMedium;
    
    Medium.SaturationProperties sat "saturation property record";
    Real dv;
  equation 
    sat = Medium.setSat_T(380.0);
    dv = Medium.dewDensity(sat);
  end TestSaturationProperties2;

  model TestThermodynamicStates "Tests thermodynamic states without uniqueID" 
    replaceable package Medium = Media.TestMedium extends 
      Modelica.Media.Interfaces.PartialTwoPhaseMedium;
    CompleteThermodynamicState state1(redeclare package Medium = Medium);
    CompleteThermodynamicState state2(redeclare package Medium = Medium);
  equation 
    state1.state = Medium.setState_ph(1e5, 1e5+time*1e5,0);
    state2.state = Medium.setState_ph(1e5+time*1e5, 1e5,0);
    annotation (experiment(NumberOfIntervals=2000), experimentSetupOutput);
  end TestThermodynamicStates;
end Test;
