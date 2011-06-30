within ExternalMedia.Media;
package FluidPropMedia
  package WaterIF95
    extends ExternalTwoPhaseMedium(
      mediumName = "Water",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterIF95;

  package WaterTPSI
    extends ExternalTwoPhaseMedium(
      mediumName = "Water",
      libraryName = "FluidProp.TPSI",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterTPSI;

  package CarbonDioxide
    extends ExternalTwoPhaseMedium(
      mediumName = "Carbon Dioxide",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"CO2"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end CarbonDioxide;
end FluidPropMedia;
