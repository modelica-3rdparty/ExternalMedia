package Test 
  model TestExternalTwoPhaseMedium 
    package Medium = ExternalMedia.Media.ExternalTwoPhaseMedium;
    Medium.BaseProperties medium_1;
    Medium.BaseProperties medium_2;
    Medium.ThermodynamicState state_1;
    Medium.ThermodynamicState state_2;
  equation 
    medium_1.p = 1e5;
    medium_1.T = 273+100*time;
  
    medium_2.p = 1e5 + time * 1e5;
    medium_2.h = 1e5;
  
    state_1 = Medium.setState_pT(1e5,273+100*time);
    state_2 = Medium.setState_ph( + time * 1e5, 1e5);
  end TestExternalTwoPhaseMedium;
end Test;
