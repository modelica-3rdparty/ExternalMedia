package Test 
  model TestExternalTwoPhaseMedium 
    package Medium = ExternalMedia.Media.ExternalTwoPhaseMedium;
    Medium.BaseProperties medium;
  equation 
    medium.p = 1e5;
    medium.T = 273+100*time;
  end TestExternalTwoPhaseMedium;
end Test;
