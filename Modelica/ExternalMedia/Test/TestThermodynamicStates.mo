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
