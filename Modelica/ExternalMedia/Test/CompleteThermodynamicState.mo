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
