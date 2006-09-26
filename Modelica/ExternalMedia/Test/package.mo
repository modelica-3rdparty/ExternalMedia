package Test 
  model TestDummyTwoPhaseMedium 
    package Medium = ExternalMedia.Media.DummyTwoPhaseMedium;
    Medium.BaseProperties medium;
  equation 
    medium.p = 1e6;
    medium.h = 1e5;
  end TestDummyTwoPhaseMedium;
end Test;
