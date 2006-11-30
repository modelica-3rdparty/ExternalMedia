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
