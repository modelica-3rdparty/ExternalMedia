model TestSaturationProperties2 
  "Test saturation properties without supplying unique ID" 
  replaceable package Medium = 
      ExternalMedia.Media.TestMedium;
  
  Medium.SaturationProperties sat "saturation property record";
  Real dv;
  Real hv;
  Real sl;
  Real ps;
  Real Ts;
equation 
  sat = Medium.setSat_T(380.0);
  dv = Medium.dewDensity(sat);
  hv = Medium.dewEnthalpy(sat);
  sl = Medium.bubbleEntropy(sat);
  ps = Medium.saturationPressure_sat(sat);
  Ts = Medium.saturationTemperature_sat(sat);
end TestSaturationProperties2;
