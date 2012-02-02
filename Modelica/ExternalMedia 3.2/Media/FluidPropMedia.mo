within ExternalMedia.Media;
package FluidPropMedia
  package WaterIF95
    extends FluidPropMedium(
      mediumName = "Water",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterIF95;

  package WaterTPSI
    extends FluidPropMedium(
      mediumName = "Water",
      libraryName = "FluidProp.TPSI",
      substanceNames = {"H2O"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end WaterTPSI;

  package CarbonDioxide
    extends FluidPropMedium(
      mediumName = "Carbon Dioxide",
      libraryName = "FluidProp.RefProp",
      substanceNames = {"CO2"},
      ThermoStates = Modelica.Media.Interfaces.PartialMedium.Choices.IndependentVariables.ph);
  end CarbonDioxide;

  package FluidPropMedium "Fluid prop medium package - modify ExternalTwoPhaseMedium to change setBubbleState and setDewState functions,
  since the phase input is currently not supported"

    extends ExternalTwoPhaseMedium;

    redeclare replaceable function extends setBubbleState "set the thermodynamic state on the bubble line: change hl a little to guarantee the correct phase,
    since the phase input is currently not supported"
      extends Modelica.Icons.Function;
      input SaturationProperties sat "saturation point";
      input FixedPhase phase "phase flag";
      output ThermodynamicState state "complete thermodynamic state info";
      // Standard definition
    algorithm
      if (phase == 1) then // liquid
        state :=setState_ph(sat.psat, sat.hl*(1-1e-6), phase);
      else // two-phase
        state :=setState_ph(sat.psat, sat.hl*(1+1e-6), phase);
      end if;
      /*  // If special definition in "C"
  external "C" TwoPhaseMedium_setBubbleState_(sat, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
*/
      annotation(Inline = true);
    end setBubbleState;

    redeclare replaceable function extends setDewState "set the thermodynamic state on the dew line: change hv a little to guarantee the correct phase,
    since the phase input is currently not supported"
      extends Modelica.Icons.Function;
      input SaturationProperties sat "saturation point";
      input FixedPhase phase "phase flag";
      output ThermodynamicState state "complete thermodynamic state info";
      // Standard definition
    algorithm
      if (phase == 1) then // vapour
        state :=setState_ph(sat.psat, sat.hv*(1+1e-6), phase);
      else // two-phase
        state :=setState_ph(sat.psat, sat.hv*(1-1e-6), phase);
      end if;
      /*  // If special definition in "C"
  external "C" TwoPhaseMedium_setDewState_(sat, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
*/
      annotation(Inline = true);
    end setDewState;

  end FluidPropMedium;
end FluidPropMedia;
