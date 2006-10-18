package Test 
  model TestExternalTwoPhaseMedium 
    replaceable package Medium = ExternalMedia.Media.ExternalTwoPhaseMedium;
    Medium.BaseProperties medium_1;
    Medium.BaseProperties medium_2;
    Medium.ThermodynamicState state_1;
    Medium.ThermodynamicState state_2;
  equation 
    medium_1.p = 1e5;
    medium_1.T = 293+50*time;
  
    medium_2.p = 1e5 + time * 1e5;
    medium_2.h = 1e5;
  
    state_1 = Medium.setState_pT(1e5,273+100*time);
    state_2 = Medium.setState_ph(1e5, 1e5+time*1e5);
  end TestExternalTwoPhaseMedium;


model TestExternalTwoPhaseMediumDynamic 
  import SI = Modelica.SIunits;
  // package Medium = Modelica.Media.Water.StandardWater;
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
end TestExternalTwoPhaseMediumDynamic;


model TestFluidPropIF95 
  extends TestExternalTwoPhaseMedium(redeclare package Medium = 
        Media.FluidPropMedia.WaterIF95);
end TestFluidPropIF95;


model TestFluidPropIF95Dynamic 
  extends TestExternalTwoPhaseMediumDynamic(redeclare package Medium = 
        Media.FluidPropMedia.WaterIF95);
  annotation (experiment(StopTime=100), experimentSetupOutput);
end TestFluidPropIF95Dynamic;

end Test;
