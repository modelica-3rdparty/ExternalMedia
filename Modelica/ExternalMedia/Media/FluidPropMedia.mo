package FluidPropMedia 
  package WaterIF95 
    extends ExternalTwoPhaseMedium(
      mediumName = "Water",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"H2O"});
  end WaterIF95;
  
  package CarbonDioxide 
    extends ExternalTwoPhaseMedium(
      mediumName = "Carbon Dioxide",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"CO2"});
  end CarbonDioxide;
end FluidPropMedia;
