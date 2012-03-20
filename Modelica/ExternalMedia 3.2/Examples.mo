within ExternalMedia;
package Examples "Examples of external medium models"
  package WaterIF95 "RefProp water model"
    extends ExternalMedia.Media.FluidPropMedium(
      mediumName = "Water",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterIF95;

  package WaterTPSI "TPSI Water model"
    extends ExternalMedia.Media.FluidPropMedium(
      mediumName = "Water",
      libraryName = "FluidProp.TPSI",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterTPSI;

  package WaterIF97 "IF97 Water model"
    extends ExternalMedia.Media.FluidPropMedium(
      mediumName = "Water",
      libraryName = "FluidProp.IF97",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterIF97;

  package CarbonDioxideStanMix "StanMix model of CO2"
    extends ExternalMedia.Media.FluidPropMedium(
      mediumName = "Carbon Dioxide",
      libraryName = "FluidProp.StanMix",
      substanceNames = {"CO2"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end CarbonDioxideStanMix;

  package CarbonDioxideRefProp "RefProp model of CO2"
    extends ExternalMedia.Media.FluidPropMedium(
      mediumName = "Carbon Dioxide",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"CO2"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end CarbonDioxideRefProp;
end Examples;
