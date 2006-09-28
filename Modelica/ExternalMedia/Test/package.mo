package Test 
  model TestExternalTwoPhaseMedium 
    package Medium = ExternalMedia.Media.ExternalTwoPhaseMedium;
    Medium.BaseProperties medium;
  equation 
    medium.p = 1e6;
    medium.h = 1e5;
  end TestExternalTwoPhaseMedium;
end Test;
