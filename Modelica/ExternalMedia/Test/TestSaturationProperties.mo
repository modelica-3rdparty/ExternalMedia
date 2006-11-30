model TestSaturationProperties 
  "Test saturation properties without supplying unique ID" 
  replaceable package Medium = 
      ExternalMedia.Media.TestMedium;
  
  Medium.SaturationProperties sat "saturation property record";
  
equation 
  sat = Medium.setSat_T(380.0);
  
end TestSaturationProperties;
