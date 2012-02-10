within ExternalMedia;
package Examples "Examples of external medium models"
  package WaterIF95
    extends ExternalMedia.Media.FluidPropMedium(
      mediumName = "Water",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterIF95;

  package WaterTPSI
    extends ExternalMedia.Media.FluidPropMedium(
      mediumName = "Water",
      libraryName = "FluidProp.TPSI",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterTPSI;

  package CarbonDioxide
    extends ExternalMedia.Media.FluidPropMedium(
      mediumName = "Carbon Dioxide",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"CO2"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end CarbonDioxide;
end Examples;
