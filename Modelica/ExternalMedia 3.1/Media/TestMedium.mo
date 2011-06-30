within ExternalMedia.Media;
package TestMedium
  extends ExternalTwoPhaseMedium(
    mediumName = "TestMedium",
    libraryName = "TestMedium",
    ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.pT);
end TestMedium;
