model TestBaseProperties 
  replaceable package Medium = ExternalMedia.Media.ExternalTwoPhaseMedium;
  
  Medium.BaseProperties medium(basePropertiesInputChoice=ExternalMedia.Common.
        InputChoices.ph) "Medium model" 
    annotation(extent=[0,0; 20,20]);
equation 
  medium.p = 0.0;
  medium.h = 0.0;
end TestBaseProperties;
