model TestSaturationFunctions 
  replaceable package Medium = ExternalMedia.Media.TestMedium 
    extends ExternalMedia.Interfaces.PartialExternalTwoPhaseMedium 
    annotation(choicesAllMatching);
  
  Medium.AbsolutePressure ps "saturation pressure";
  Medium.Temperature Ts "saturation temperature";
  
  Medium.SaturationProperties sat "saturation property record";
  Medium.AbsolutePressure ps_sat "saturation pressure";
  Medium.Temperature Ts_sat "saturation temperature";
  
  Integer uniqueID(final start=0) "unique ID";
algorithm 
  when (initial()) then
    uniqueID := Medium.createMedium(uniqueID);
  end when;
equation 
  ps = Medium.saturationPressure(300.0);
  Ts = Medium.saturationTemperature(ps);
  
  sat = Medium.setSat_p(ps, uniqueID);
  ps_sat = Medium.saturationPressure_sat(sat);
  Ts_sat = Medium.saturationTemperature_sat(sat);
end TestSaturationFunctions;
