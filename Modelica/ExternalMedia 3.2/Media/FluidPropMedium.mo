within ExternalMedia.Media;
package FluidPropMedium "Fluid prop medium package"

  extends ExternalTwoPhaseMedium;

  constant Real h_eps_sat = 1e-6
    "small delta h to ensure computation in the correct phase";

  redeclare replaceable function setBubbleState
    "Set the thermodynamic state on the bubble line"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation point";
    input FixedPhase phase = 0 "phase flag";
    output ThermodynamicState state "complete thermodynamic state info";
    // Standard definition
  algorithm
    if (phase == 0) then // phase doesn't matter
      state := setState_ph(sat.psat, sat.hl, phase);
    elseif (phase == 1) then // liquid
      state := setState_ph(sat.psat, sat.hl*(1-h_eps_sat), phase);
    else // two-phase
      state := setState_ph(sat.psat, sat.hl*(1+h_eps_sat), phase);
    end if;
    /*  // If special definition in "C"
  external "C" TwoPhaseMedium_setBubbleState_(sat, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
*/
    annotation(Inline = true);
  end setBubbleState;

  redeclare replaceable function setDewState
    "Set the thermodynamic state on the dew line"
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation point";
    input FixedPhase phase = 0 "phase flag";
    output ThermodynamicState state "complete thermodynamic state info";
    // Standard definition
  algorithm
    if (phase == 0) then // phase doesn't matter
      state := setState_ph(sat.psat, sat.hv, phase);
    elseif (phase == 1) then // vapour
      state := setState_ph(sat.psat, sat.hv*(1+h_eps_sat), phase);
    else // two-phase
      state := setState_ph(sat.psat, sat.hv*(1-h_eps_sat), phase);
    end if;
    /*  // If special definition in "C"
  external "C" TwoPhaseMedium_setDewState_(sat, phase, state, mediumName, libraryName, substanceName)
    annotation(Include="#include \"externalmedialib.h\"", Library="ExternalMediaLib");
*/
    annotation(Inline = true);
  end setDewState;

  redeclare function bubbleEntropy "Return bubble point specific entropy"
    input SaturationProperties sat "saturation property record";
    output SI.SpecificEntropy sl "boiling curve specific entropy";
  algorithm
    sl := specificEntropy(setBubbleState(sat));
  end bubbleEntropy;

  redeclare function dewEntropy "Return dew point specific entropy"
    input SaturationProperties sat "saturation property record";
    output SI.SpecificEntropy sv "dew curve specific entropy";
  algorithm
    sv := specificEntropy(setDewState(sat));
  end dewEntropy;

  redeclare function surfaceTension
    extends Modelica.Icons.Function;
    input SaturationProperties sat "saturation property record";
    output SurfaceTension sigma "Surface tension sigma in the two phase region";
  algorithm
    assert(false, "FluidProp interface does not provide surface tension");
  end surfaceTension;
end FluidPropMedium;
