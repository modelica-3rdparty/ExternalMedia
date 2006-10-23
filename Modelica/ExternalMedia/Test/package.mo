package Test 
  model TestMediumExplicit 
    replaceable package Medium = Media.TestMedium;
    Medium.BaseProperties medium_1;
    Medium.BaseProperties medium_2;
  equation 
    medium_1.p = 1e5;
    medium_1.h = 1e5 + 2e5*time;
    medium_2.p = 1e5;
    medium_2.h = 1e5 + 1e5*time;
  end TestMediumExplicit;
 
  model TestMediumImplicit 
    replaceable package Medium = Media.TestMedium;
    Medium.BaseProperties medium_1;
    Medium.BaseProperties medium_2;
  equation 
    medium_1.p = 1e5;
    medium_1.T = 300 + 25*time;
    medium_2.p = 1e5;
    medium_2.T = 300 + 50*time;
  end TestMediumImplicit;
  
  
model TestMediumDynamic 
  import SI = Modelica.SIunits;
  replaceable package Medium = ExternalMedia.Media.ExternalTwoPhaseMedium;
  parameter SI.Volume V = 1 "Storage Volume";
  parameter Real p_atm = 101325 "Atmospheric pressure";
  parameter SI.Temperature Tstart = 300;
  parameter Real Kv0 = 1.00801e-2 "Valve flow coefficient";
  Medium.BaseProperties medium(preferredMediumStates = true);
  SI.Mass M;
  SI.Energy U;
  SI.MassFlowRate win;
  SI.MassFlowRate wout;
  SI.SpecificEnthalpy hin;
  SI.SpecificEnthalpy hout;
  SI.Power Q;
  Real Kv;
  SI.Pressure p(stateSelect = StateSelect.prefer);
  SI.SpecificEnthalpy h(stateSelect = StateSelect.prefer);
equation 
  // Mass & energy balance equation
  M = medium.d*V;
  U = medium.u*M;
  der(M) = win - wout;
  der(U) = win*hin - wout*hout + Q;
    
  // Preferred states
  p = medium.p;
  h = medium.h;
    
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
  medium.p = 2e5;
  medium.h = 1e5;
  annotation (experiment(StopTime=80, Tolerance=1e-007),experimentSetupOutput(
        equdistant=false));
end TestMediumDynamic;
 
model FluidPropIF95Explicit 
  extends TestMediumExplicit(redeclare package Medium = 
        Media.FluidPropMedia.WaterIF95);
end FluidPropIF95Explicit;
  
model FluidPropIF95Implicit 
  extends TestMediumImplicit(redeclare package Medium = 
        Media.FluidPropMedia.WaterIF95);
end FluidPropIF95Implicit;
 
model FluidPropIF95Dynamic 
  extends TestMediumDynamic(redeclare package Medium = 
        Media.FluidPropMedia.WaterIF95);
    annotation (experiment(StopTime=80, Tolerance=1e-007), 
        experimentSetupOutput(equdistant=false));
end FluidPropIF95Dynamic;
end Test;