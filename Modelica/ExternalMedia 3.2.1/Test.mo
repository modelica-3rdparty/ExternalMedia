within ExternalMedia;
package Test "Test models"
  package TestMedium "Test cases for TestMedium"
    model TestStatesSat "Test case using TestMedium with state + sat records"
      replaceable package Medium = Media.TestMedium;
      Medium.BaseProperties baseProperties1;
      Medium.BaseProperties baseProperties2;
      Medium.ThermodynamicState state1;
      Medium.ThermodynamicState state2;
      Medium.SaturationProperties sat1;
      Medium.SaturationProperties sat2;

      Medium.Temperature Ts;
      Medium.AbsolutePressure ps;

      GenericModels.CompleteThermodynamicState completeState1(redeclare package
          Medium = Medium, state=state1);
      GenericModels.CompleteThermodynamicState completeState2(redeclare package
          Medium = Medium, state=state2);
      GenericModels.CompleteSaturationProperties completeSat1(redeclare package
          Medium = Medium, sat=sat1);
      GenericModels.CompleteSaturationProperties completeSat2(redeclare package
          Medium = Medium, sat=sat2);
      GenericModels.CompleteBubbleDewStates completeBubbleDewStates1(redeclare
          package Medium = Medium, sat=sat1);
      GenericModels.CompleteBubbleDewStates completeBubbleDewStates2(redeclare
          package Medium = Medium, sat=sat1);
    equation
      baseProperties1.p = 1e5 + 1e5*time;
      baseProperties1.h = 1e5;
      baseProperties2.p = 1e5;
      baseProperties2.h = 1e5 + 2e5*time;

      state1 = Medium.setState_ph(1e5 + 1e5*time, 1e5);
      state2 = Medium.setState_pT(1e5, 300 + 50*time);

      sat1 = Medium.setSat_p(1e5 + 1e5*time);
      sat2 = Medium.setSat_T(300 + 50*time);

      Ts = Medium.saturationTemperature(1e5 + 1e5*time);
      ps = Medium.saturationPressure(300 + 50*time);
    end TestStatesSat;

    model TestBasePropertiesExplicit
      "Test case using TestMedium and BaseProperties with explicit equations"
      replaceable package Medium = Media.TestMedium;
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBaseProperties
        medium1(redeclare package Medium = Medium)
        "Constant pressure, varying enthalpy";
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBaseProperties
        medium2(redeclare package Medium = Medium)
        "Varying pressure, constant enthalpy";
    equation
      medium1.baseProperties.p = 1e5 + 1e5*time;
      medium1.baseProperties.h = 1e5;
      medium2.baseProperties.p = 1e5;
      medium2.baseProperties.h = 1e5 + 2e5*time;
    end TestBasePropertiesExplicit;

    model TestBasePropertiesImplicit
      "Test case using TestMedium and BaseProperties with implicit equations"
      replaceable package Medium = Media.TestMedium;
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBaseProperties
        medium1(redeclare package Medium = Medium, baseProperties(h(start=1e5)))
        "Constant pressure, varying enthalpy";
      ExternalMedia.Test.TestMedium.GenericModels.CompleteBaseProperties
        medium2(redeclare package Medium = Medium, baseProperties(h(start=1e5)))
        "Varying pressure, constant enthalpy";
    equation
      medium1.baseProperties.p = 1e5*time;
      medium1.baseProperties.T = 300 + 25*time;
      medium2.baseProperties.p = 1e5 + 1e5*time;
      medium2.baseProperties.T = 300;
    end TestBasePropertiesImplicit;

    model TestBasePropertiesDynamic
      "Test case using TestMedium and dynamic equations"
      replaceable package Medium = Media.TestMedium;
      parameter SI.Volume V=1 "Storage Volume";
      parameter Real p_atm=101325 "Atmospheric pressure";
      parameter SI.Temperature Tstart=300;
      parameter Real Kv0=1.00801e-2 "Valve flow coefficient";
      Medium.BaseProperties medium(preferredMediumStates=true);
      SI.Mass M;
      SI.Energy U;
      SI.MassFlowRate win(start=100);
      SI.MassFlowRate wout;
      SI.SpecificEnthalpy hin;
      SI.SpecificEnthalpy hout;
      SI.Power Q;
      Real Kv;
    equation
      // Mass & energy balance equation
      M = medium.d*V;
      U = medium.u*M;
      der(M) = win - wout;
      der(U) = win*hin - wout*hout + Q;

      // Inlet pump equations
      medium.p - p_atm = 2e5 - (1e5/100^2)*win^2;
      hin = 1e5;

      // Outlet valve equation
      wout = Kv*sqrt(medium.d*(medium.p - p_atm));
      hout = medium.h;

      // Input variables
      Kv = if time < 50 then Kv0 else Kv0*1.1;
      Q = if time < 1 then 0 else 1e7;
    initial equation
      // Initial conditions

      // Fixed initial states
      // medium.p = 2e5;
      // medium.h = 1e5;

      // Steady state equations
      der(medium.p) = 0;
      der(medium.h) = 0;
      annotation (experiment(StopTime=80, Tolerance=1e-007),
          experimentSetupOutput(equdistant=false));
    end TestBasePropertiesDynamic;

    package GenericModels
      "Contains generic models to use for thorough medium model testing"

      model CompleteFluidConstants
        "Compute all available medium fluid constants"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // Fluid constants
        Medium.Temperature Tc=Medium.fluidConstants[1].criticalTemperature;
        Medium.AbsolutePressure pc=Medium.fluidConstants[1].criticalPressure;
        Medium.MolarVolume vc=Medium.fluidConstants[1].criticalMolarVolume;
        Medium.MolarMass MM=Medium.fluidConstants[1].molarMass;
      end CompleteFluidConstants;

      model CompleteThermodynamicState
        "Compute all available two-phase medium properties from a ThermodynamicState model"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // ThermodynamicState record
        input Medium.ThermodynamicState state;

        // Medium properties
        Medium.AbsolutePressure p=Medium.pressure(state);
        Medium.SpecificEnthalpy h=Medium.specificEnthalpy(state);
        Medium.Temperature T=Medium.temperature(state);
        Medium.Density d=Medium.density(state);
        Medium.SpecificEntropy s=Medium.specificEntropy(state);
        Medium.SpecificHeatCapacity cp=Medium.specificHeatCapacityCp(state);
        Medium.SpecificHeatCapacity cv=Medium.specificHeatCapacityCv(state);
        Medium.IsobaricExpansionCoefficient beta=
            Medium.isobaricExpansionCoefficient(state);
        SI.IsothermalCompressibility kappa=Medium.isothermalCompressibility(
            state);
        Medium.DerDensityByPressure d_d_dp_h=Medium.density_derp_h(state);
        Medium.DerDensityByEnthalpy d_d_dh_p=Medium.density_derh_p(state);
        Medium.MolarMass MM=Medium.molarMass(state);
      end CompleteThermodynamicState;

      model CompleteSaturationProperties
        "Compute all available saturation properties from a SaturationProperties record"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // SaturationProperties record
        input Medium.SaturationProperties sat;

        // Saturation properties
        Medium.Temperature Ts=Medium.saturationTemperature_sat(sat);
        Medium.Density dl=Medium.bubbleDensity(sat);
        Medium.Density dv=Medium.dewDensity(sat);
        Medium.SpecificEnthalpy hl=Medium.bubbleEnthalpy(sat);
        Medium.SpecificEnthalpy hv=Medium.dewEnthalpy(sat);
        Real d_Ts_dp=Medium.saturationTemperature_derp_sat(sat);
        Real d_dl_dp=Medium.dBubbleDensity_dPressure(sat);
        Real d_dv_dp=Medium.dDewDensity_dPressure(sat);
        Real d_hl_dp=Medium.dBubbleEnthalpy_dPressure(sat);
        Real d_hv_dp=Medium.dDewEnthalpy_dPressure(sat);
      end CompleteSaturationProperties;

      model CompleteBubbleDewStates
        "Compute all available properties for dewpoint and bubble point states corresponding to a sat record"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // SaturationProperties record
        input Medium.SaturationProperties sat;

        CompleteThermodynamicState dewStateOnePhase(state=Medium.setDewState(
              sat, 1), redeclare package Medium = Medium);
        CompleteThermodynamicState dewStateTwoPhase(state=Medium.setDewState(
              sat, 2), redeclare package Medium = Medium);
        CompleteThermodynamicState bubbleStateOnePhase(state=
              Medium.setBubbleState(sat, 1), redeclare package Medium = Medium);
        CompleteThermodynamicState bubbleStateTwoPhase(state=
              Medium.setBubbleState(sat, 2), redeclare package Medium = Medium);
      end CompleteBubbleDewStates;

      model CompleteBaseProperties
        "Compute all available two-phase medium properties from a BaseProperties model"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // BaseProperties object
        Medium.BaseProperties baseProperties;

        // All the complete properties
        CompleteThermodynamicState completeState(redeclare package Medium =
              Medium, state=baseProperties.state);
        CompleteSaturationProperties completeSat(redeclare package Medium =
              Medium, sat=baseProperties.sat);
        CompleteFluidConstants completeConstants(redeclare package Medium =
              Medium);
        CompleteBubbleDewStates completeBubbleDewStates(redeclare package
            Medium = Medium, sat=baseProperties.sat);
      end CompleteBaseProperties;
    end GenericModels;
  end TestMedium;

  package FluidProp "Test cases for FluidPropMedium"

    partial package GenericModels "Generic models for FluidProp media tests"
      model CompleteFluidConstants
        "Compute all available medium fluid constants"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // Fluid constants
        Medium.Temperature Tc=Medium.fluidConstants[1].criticalTemperature;
        Medium.AbsolutePressure pc=Medium.fluidConstants[1].criticalPressure;
        Medium.MolarVolume vc=Medium.fluidConstants[1].criticalMolarVolume;
        Medium.MolarMass MM=Medium.fluidConstants[1].molarMass;
      end CompleteFluidConstants;

      model CompleteThermodynamicState
        "Compute all available two-phase medium properties from a ThermodynamicState model"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // ThermodynamicState record
        input Medium.ThermodynamicState state;

        // Medium properties
        Medium.AbsolutePressure p=Medium.pressure(state);
        Medium.SpecificEnthalpy h=Medium.specificEnthalpy(state);
        Medium.Temperature T=Medium.temperature(state);
        Medium.Density d=Medium.density(state);
        Medium.SpecificEntropy s=Medium.specificEntropy(state);
        Medium.SpecificHeatCapacity cp=Medium.specificHeatCapacityCp(state);
        Medium.SpecificHeatCapacity cv=Medium.specificHeatCapacityCv(state);
        // Not yet implemented in FluidProp
        Medium.IsobaricExpansionCoefficient beta=
            Medium.isobaricExpansionCoefficient(state);
        SI.IsothermalCompressibility kappa=Medium.isothermalCompressibility(
            state);
        Medium.DerDensityByPressure d_d_dp_h=Medium.density_derp_h(state);
        Medium.DerDensityByEnthalpy d_d_dh_p=Medium.density_derh_p(state);
        Medium.MolarMass MM=Medium.molarMass(state);
      end CompleteThermodynamicState;

      model CompleteSaturationProperties
        "Compute all available saturation properties from a SaturationProperties record"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // SaturationProperties record
        input Medium.SaturationProperties sat;

        // Saturation properties
        Medium.Temperature Ts=Medium.saturationTemperature_sat(sat);
        Medium.Density dl=Medium.bubbleDensity(sat);
        Medium.Density dv=Medium.dewDensity(sat);
        Medium.SpecificEnthalpy hl=Medium.bubbleEnthalpy(sat);
        Medium.SpecificEnthalpy hv=Medium.dewEnthalpy(sat);
        Real d_Ts_dp=Medium.saturationTemperature_derp_sat(sat);
        Real d_dl_dp=Medium.dBubbleDensity_dPressure(sat);
        Real d_dv_dp=Medium.dDewDensity_dPressure(sat);
        Real d_hl_dp=Medium.dBubbleEnthalpy_dPressure(sat);
        Real d_hv_dp=Medium.dDewEnthalpy_dPressure(sat);
      end CompleteSaturationProperties;

      model CompleteBubbleDewStates
        "Compute all available properties for dewpoint and bubble point states corresponding to a sat record"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // SaturationProperties record
        input Medium.SaturationProperties sat;

        CompleteThermodynamicState dewStateOnePhase(state=Medium.setDewState(
              sat, 1), redeclare package Medium = Medium);
        CompleteThermodynamicState dewStateTwoPhase(state=Medium.setDewState(
              sat, 2), redeclare package Medium = Medium);
        CompleteThermodynamicState bubbleStateOnePhase(state=
              Medium.setBubbleState(sat, 1), redeclare package Medium = Medium);
        CompleteThermodynamicState bubbleStateTwoPhase(state=
              Medium.setBubbleState(sat, 2), redeclare package Medium = Medium);
      end CompleteBubbleDewStates;

      model CompleteBaseProperties
        "Compute all available two-phase medium properties from a BaseProperties model"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        // BaseProperties object
        Medium.BaseProperties baseProperties;

        // All the complete properties
        CompleteThermodynamicState completeState(redeclare package Medium =
              Medium, state=baseProperties.state);
        CompleteSaturationProperties completeSat(redeclare package Medium =
              Medium, sat=baseProperties.sat);
        CompleteFluidConstants completeConstants(redeclare package Medium =
              Medium);
      end CompleteBaseProperties;

      partial model TestStates "Test case with state"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        Medium.AbsolutePressure p1;
        Medium.SpecificEnthalpy h1;
        Medium.AbsolutePressure p2;
        Medium.Temperature T2;

        Medium.ThermodynamicState state1;
        Medium.ThermodynamicState state2;

        CompleteThermodynamicState completeState1(redeclare package Medium =
              Medium, state=state1);
        CompleteThermodynamicState completeState2(redeclare package Medium =
              Medium, state=state2);
      equation
        state1 = Medium.setState_ph(p1, h1);
        state2 = Medium.setState_pT(p2, T2);
      end TestStates;

      partial model TestStatesSat "Test case with state + sat records"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;

        Medium.AbsolutePressure p1;
        Medium.SpecificEnthalpy h1;
        Medium.AbsolutePressure p2;
        Medium.Temperature T2;

        Medium.ThermodynamicState state1;
        Medium.ThermodynamicState state2;
        Medium.SaturationProperties sat1;
        Medium.SaturationProperties sat2;

        Medium.Temperature Ts;
        Medium.AbsolutePressure ps;

        CompleteThermodynamicState completeState1(redeclare package Medium =
              Medium, state=state1);
        CompleteThermodynamicState completeState2(redeclare package Medium =
              Medium, state=state2);
        CompleteSaturationProperties completeSat1(redeclare package Medium =
              Medium, sat=sat1);
        CompleteSaturationProperties completeSat2(redeclare package Medium =
              Medium, sat=sat2);
        CompleteBubbleDewStates completeBubbleDewStates1(redeclare package
            Medium = Medium, sat=sat1);
        CompleteBubbleDewStates completeBubbleDewStates2(redeclare package
            Medium = Medium, sat=sat2);
      equation
        state1 = Medium.setState_ph(p1, h1);
        state2 = Medium.setState_pT(p2, T2);
        sat1 = Medium.setSat_p(p1);
        sat2 = Medium.setSat_T(T2);

        Ts = Medium.saturationTemperature(p1);
        ps = Medium.saturationPressure(T2);
      end TestStatesSat;

      partial model TestBasePropertiesExplicit
        "Test case using BaseProperties and explicit equations"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;
        CompleteBaseProperties medium1(redeclare package Medium = Medium)
          "Constant pressure, varying enthalpy";
        CompleteBaseProperties medium2(redeclare package Medium = Medium)
          "Varying pressure, constant enthalpy";
        Medium.AbsolutePressure p1;
        Medium.AbsolutePressure p2;
        Medium.SpecificEnthalpy h1;
        Medium.SpecificEnthalpy h2;
      equation
        medium1.baseProperties.p = p1;
        medium1.baseProperties.h = h1;
        medium2.baseProperties.p = p2;
        medium2.baseProperties.h = h2;
      end TestBasePropertiesExplicit;

      partial model TestBasePropertiesImplicit
        "Test case using BaseProperties and implicit equations"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;
        parameter Medium.SpecificEnthalpy hstart
          "Start value for specific enthalpy";
        CompleteBaseProperties medium1(redeclare package Medium = Medium,
            baseProperties(h(start=hstart)))
          "Constant pressure, varying enthalpy";
        CompleteBaseProperties medium2(redeclare package Medium = Medium,
            baseProperties(h(start=hstart)))
          "Varying pressure, constant enthalpy";
        Medium.AbsolutePressure p1;
        Medium.AbsolutePressure p2;
        Medium.Temperature T1;
        Medium.Temperature T2;
      equation
        medium1.baseProperties.p = p1;
        medium1.baseProperties.T = T1;
        medium2.baseProperties.p = p2;
        medium2.baseProperties.T = T2;
      end TestBasePropertiesImplicit;

      partial model TestBasePropertiesDynamic
        "Test case using BaseProperties and dynamic equations"
        replaceable package Medium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;
        parameter SI.Volume V=1 "Storage Volume";
        parameter Real p_atm=101325 "Atmospheric pressure";
        parameter SI.Temperature Tstart=300;
        parameter SI.SpecificEnthalpy hstart=1e5;
        parameter SI.Pressure pstart=p_atm;
        parameter Real Kv0 "Valve flow coefficient";
        Medium.BaseProperties medium(
          preferredMediumStates=true,
          h(start=hstart),
          p(start=pstart));
        SI.Mass M;
        SI.Energy U;
        SI.MassFlowRate win(start=100);
        SI.MassFlowRate wout;
        SI.SpecificEnthalpy hin;
        SI.SpecificEnthalpy hout;
        SI.Power Q;
        Real Kv;
      equation
        // Mass & energy balance equation
        M = medium.d*V;
        U = medium.u*M;
        der(M) = win - wout;
        der(U) = win*hin - wout*hout + Q;

        // Outlet valve equation
        wout = Kv*sqrt(medium.d*(medium.p - p_atm));
        hout = medium.h;

      initial equation
        // Steady state equations
        der(medium.p) = 0;
        der(medium.h) = 0;
        annotation (experiment(StopTime=80, Tolerance=1e-007),
            experimentSetupOutput(equdistant=false));
      end TestBasePropertiesDynamic;

      partial model CompareModelicaFluidProp
        "Comparison between Modelica and FluidProp models"
        replaceable package ModelicaMedium =
            Modelica.Media.Interfaces.PartialTwoPhaseMedium;
        replaceable package FluidPropMedium =
            ExternalMedia.Media.FluidPropMedium;
        CompleteBaseProperties modelicaMedium(redeclare package Medium =
              ModelicaMedium) "Modelica medium model";
        CompleteBaseProperties fluidPropMedium(redeclare package Medium =
              FluidPropMedium) "FluidProp medium model";
        parameter Modelica.SIunits.Pressure pmin;
        parameter Modelica.SIunits.Pressure pmax;
        parameter Modelica.SIunits.SpecificEnthalpy hmin;
        parameter Modelica.SIunits.SpecificEnthalpy hmax;
      equation
        modelicaMedium.baseProperties.p = pmin + (pmax - pmin)*time;
        modelicaMedium.baseProperties.h = hmin + (hmax - hmin)*time;
        fluidPropMedium.baseProperties.p = pmin + (pmax - pmin)*time;
        fluidPropMedium.baseProperties.h = hmin + (hmax - hmin)*time;
      end CompareModelicaFluidProp;
    end GenericModels;

    package WaterIF95 "Test suite for the FluidProp-Refprop IF95 medium model"
      model TestStates "Test case with state records"
        extends GenericModels.TestStates(redeclare package Medium =
              ExternalMedia.Examples.WaterIF95);
      equation
        p1 = 1e5;
        h1 = 1e5 + 2e5*time;
        p2 = 1e5;
        T2 = 300 + 50*time;
      end TestStates;

      model TestStatesSat "Test case with state + sat records"
        extends GenericModels.TestStatesSat(redeclare package Medium =
              ExternalMedia.Examples.WaterIF95);
      equation
        p1 = 1e5;
        h1 = 1e5 + 2e5*time;
        p2 = 1e5;
        T2 = 300 + 50*time;
      end TestStatesSat;

      model TestBasePropertiesExplicit
        "Test case using BaseProperties and explicit equations"
        extends GenericModels.TestBasePropertiesExplicit(redeclare package
            Medium = ExternalMedia.Examples.WaterIF95);

      equation
        p1 = 1e5 + 1e5*time;
        h1 = 1e5;
        p2 = 1e5;
        h2 = 1e5 + 2e5*time;
      end TestBasePropertiesExplicit;

      model TestBasePropertiesImplicit
        "Test case using BaseProperties and implicit equations"
        extends GenericModels.TestBasePropertiesImplicit(redeclare package
            Medium = ExternalMedia.Examples.WaterIF95, hstart=1e5);
      equation
        p1 = 1e5 + 1e5*time;
        T1 = 300 + 25*time;
        p2 = 1e5 + 1e5*time;
        T2 = 300;
      end TestBasePropertiesImplicit;

      model TestBasePropertiesDynamic
        "Test case using BaseProperties and dynamic equations"
        extends GenericModels.TestBasePropertiesDynamic(
          redeclare package Medium = ExternalMedia.Examples.WaterIF95,
          Tstart=300,
          Kv0=1.00801e-2);
      equation
        // Inlet pump equations
        medium.p - p_atm = 2e5 - (1e5/100^2)*win^2;
        hin = 1e5;

        // Input variables
        Kv = if time < 50 then Kv0 else Kv0*1.1;
        Q = if time < 1 then 0 else 1e7;
        annotation (experiment(StopTime=80, Tolerance=1e-007),
            experimentSetupOutput(equdistant=false));
      end TestBasePropertiesDynamic;

      model CompareModelicaFluidProp_liquid
        "Comparison between Modelica IF97 and FluidProp IF95 models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterIF95,
          pmin=1e5,
          pmax=1e5,
          hmin=1e5,
          hmax=4e5);

      end CompareModelicaFluidProp_liquid;

      model CompareModelicaFluidProp_twophase
        "Comparison between Modelica IF97 and FluidProp IF95 models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterIF95,
          pmin=60e5,
          pmax=60e5,
          hmin=1000e3,
          hmax=2000e3);

      end CompareModelicaFluidProp_twophase;

      model CompareModelicaFluidProp_vapour
        "Comparison between Modelica IF97 and FluidProp IF95 models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterIF95,
          pmin=60e5,
          pmax=60e5,
          hmin=2800e3,
          hmax=3200e3);

      end CompareModelicaFluidProp_vapour;
    end WaterIF95;

    package WaterIF97 "Test suite for the FluidProp IF97 medium model"
      model TestStates "Test case with state records"
        extends GenericModels.TestStates(redeclare package Medium =
              ExternalMedia.Examples.WaterIF95);
      equation
        p1 = 1e5;
        h1 = 1e5 + 2e5*time;
        p2 = 1e5;
        T2 = 300 + 50*time;
      end TestStates;

      model TestStatesSat "Test case with state + sat records"
        extends GenericModels.TestStatesSat(redeclare package Medium =
              ExternalMedia.Examples.WaterIF95);
      equation
        p1 = 1e5;
        h1 = 1e5 + 2e5*time;
        p2 = 1e5;
        T2 = 300 + 50*time;
      end TestStatesSat;

      model TestBasePropertiesExplicit
        "Test case using BaseProperties and explicit equations"
        extends GenericModels.TestBasePropertiesExplicit(redeclare package
            Medium = ExternalMedia.Examples.WaterIF95);

      equation
        p1 = 1e5 + 1e5*time;
        h1 = 1e5;
        p2 = 1e5;
        h2 = 1e5 + 2e5*time;
      end TestBasePropertiesExplicit;

      model TestBasePropertiesImplicit
        "Test case using BaseProperties and implicit equations"
        extends GenericModels.TestBasePropertiesImplicit(redeclare package
            Medium = ExternalMedia.Examples.WaterIF95, hstart=1e5);
      equation
        p1 = 1e5 + 1e5*time;
        T1 = 300 + 25*time;
        p2 = 1e5 + 1e5*time;
        T2 = 300;
      end TestBasePropertiesImplicit;

      model TestBasePropertiesDynamic
        "Test case using BaseProperties and dynamic equations"
        extends GenericModels.TestBasePropertiesDynamic(
          redeclare package Medium = ExternalMedia.Examples.WaterIF95,
          Tstart=300,
          Kv0=1.00801e-2);
      equation
        // Inlet pump equations
        medium.p - p_atm = 2e5 - (1e5/100^2)*win^2;
        hin = 1e5;

        // Input variables
        Kv = if time < 50 then Kv0 else Kv0*1.1;
        Q = if time < 1 then 0 else 1e7;
        annotation (experiment(StopTime=80, Tolerance=1e-007),
            experimentSetupOutput(equdistant=false));
      end TestBasePropertiesDynamic;

      model CompareModelicaFluidProp_liquid
        "Comparison between Modelica IF97 and FluidProp IF97 models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterIF97,
          pmin=1e5,
          pmax=1e5,
          hmin=1e5,
          hmax=4e5);

      end CompareModelicaFluidProp_liquid;

      model CompareModelicaFluidProp_twophase
        "Comparison between Modelica IF97 and FluidProp IF97 models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterIF97,
          pmin=60e5,
          pmax=60e5,
          hmin=1000e3,
          hmax=2000e3);

      end CompareModelicaFluidProp_twophase;

      model CompareModelicaFluidProp_vapour
        "Comparison between Modelica IF97 and FluidProp IF97 models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterIF97,
          pmin=60e5,
          pmax=60e5,
          hmin=2800e3,
          hmax=3200e3);

      end CompareModelicaFluidProp_vapour;
    end WaterIF97;

    package WaterTPSI "Test suite for the FluidProp TPSI water medium model"
      model TestStates "Test case with state records"
        extends GenericModels.TestStates(redeclare package Medium =
              ExternalMedia.Examples.WaterTPSI);
      equation
        p1 = 1e5;
        h1 = 1e5 + 2e5*time;
        p2 = 1e5;
        T2 = 300 + 50*time;
      end TestStates;

      model TestStatesSat "Test case with state + sat records"
        extends GenericModels.TestStatesSat(redeclare package Medium =
              ExternalMedia.Examples.WaterTPSI);
      equation
        p1 = 1e5;
        h1 = 1e5 + 2e5*time;
        p2 = 1e5;
        T2 = 300 + 50*time;
      end TestStatesSat;

      model TestBasePropertiesExplicit
        "Test case using BaseProperties and explicit equations"
        extends GenericModels.TestBasePropertiesExplicit(redeclare package
            Medium = ExternalMedia.Examples.WaterTPSI);

      equation
        p1 = 1e5 + 1e5*time;
        h1 = 1e5;
        p2 = 1e5;
        h2 = 1e5 + 2e5*time;
      end TestBasePropertiesExplicit;

      model TestBasePropertiesImplicit
        "Test case using BaseProperties and implicit equations"
        extends GenericModels.TestBasePropertiesImplicit(redeclare package
            Medium = ExternalMedia.Examples.WaterTPSI, hstart=1e5);
      equation
        p1 = 1e5 + 1e5*time;
        T1 = 300 + 25*time;
        p2 = 1e5 + 1e5*time;
        T2 = 300;
      end TestBasePropertiesImplicit;

      model TestBasePropertiesDynamic
        "Test case using BaseProperties and dynamic equations"
        extends GenericModels.TestBasePropertiesDynamic(
          redeclare package Medium = ExternalMedia.Examples.WaterTPSI,
          Tstart=300,
          Kv0=1.00801e-2);
      equation
        // Inlet pump equations
        medium.p - p_atm = 2e5 - (1e5/100^2)*win^2;
        hin = 1e5;

        // Input variables
        Kv = if time < 50 then Kv0 else Kv0*1.1;
        Q = if time < 1 then 0 else 1e7;
        annotation (experiment(StopTime=80, Tolerance=1e-007),
            experimentSetupOutput(equdistant=false));
      end TestBasePropertiesDynamic;

      model CompareModelicaFluidProp_liquid
        "Comparison between Modelica IF97 and FluidProp TPSI models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterTPSI,
          pmin=1e5,
          pmax=1e5,
          hmin=1e5,
          hmax=4e5);

      end CompareModelicaFluidProp_liquid;

      model CompareModelicaFluidProp_twophase
        "Comparison between Modelica IF97 and FluidProp TPSI models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterTPSI,
          pmin=60e5,
          pmax=60e5,
          hmin=1000e3,
          hmax=2000e3);

      end CompareModelicaFluidProp_twophase;

      model CompareModelicaFluidProp_vapour
        "Comparison between Modelica IF97 and FluidProp TPSI models - liquid"
        extends GenericModels.CompareModelicaFluidProp(
          redeclare package ModelicaMedium = Modelica.Media.Water.StandardWater,
          redeclare package FluidPropMedium = ExternalMedia.Examples.WaterTPSI,
          pmin=60e5,
          pmax=60e5,
          hmin=2800e3,
          hmax=3200e3);

      end CompareModelicaFluidProp_vapour;

    end WaterTPSI;

    package CO2StanMix "Test suite for the StanMix CO2 medium model"

      model TestStatesSupercritical
        "Test case with state records, supercritical conditions"
        extends GenericModels.TestStates(redeclare package Medium =
              ExternalMedia.Examples.CO2StanMix);
      equation
        p1 = 8e6;
        h1 = -4.2e5 + 6e5*time;
        p2 = 8e6;
        T2 = 280 + 50*time;
      end TestStatesSupercritical;

      model TestStatesTranscritical
        "Test case with state records, transcritical conditions"
        extends GenericModels.TestStates(redeclare package Medium =
              ExternalMedia.Examples.CO2StanMix);
      equation
        p1 = 1e6 + time*10e6;
        h1 = -4.2e5 + 0*time;
        p2 = 1e6 + time*10e6;
        T2 = 330;
      end TestStatesTranscritical;

      model TestStatesSatSubcritical
        "Test case with state + sat records, subcritical conditions"
        extends GenericModels.TestStatesSat(redeclare package Medium =
              ExternalMedia.Examples.CO2StanMix);
      equation
        p1 = 1e6;
        h1 = -4.2e5 + 6e5*time;
        p2 = 1e6;
        T2 = 250 + 50*time;
      end TestStatesSatSubcritical;

      model TestBasePropertiesExplicit
        "Test case using BaseProperties and explicit equations"
        extends GenericModels.TestBasePropertiesExplicit(redeclare package
            Medium = ExternalMedia.Examples.CO2StanMix);

      equation
        p1 = 8e6;
        h1 = -4.2e5 + 6e5*time;
        p2 = 1e6;
        h2 = -4.2e5 + 6e5*time;
      end TestBasePropertiesExplicit;

      model TestBasePropertiesImplicit
        "Test case using BaseProperties and implicit equations"
        extends GenericModels.TestBasePropertiesImplicit(redeclare package
            Medium = ExternalMedia.Examples.CO2StanMix, hstart=0);
      equation
        p1 = 8e6;
        T1 = 280 + 20*time;
        p2 = 1e6;
        T2 = 280 + 20*time;
      end TestBasePropertiesImplicit;

      model TestBasePropertiesDynamic
        "Test case using BaseProperties and dynamic equations"
        extends GenericModels.TestBasePropertiesDynamic(
          redeclare package Medium = ExternalMedia.Examples.CO2StanMix,
          Tstart=300,
          hstart=0,
          pstart=1e6,
          Kv0=1.00801e-4,
          V=0.1);
      equation
        // Inlet equations
        win = 1;
        hin = 0;

        // Input variables
        Kv = if time < 50 then Kv0 else Kv0*1.1;
        Q = if time < 1 then 0 else 1e4;
        annotation (experiment(StopTime=80, Tolerance=1e-007),
            experimentSetupOutput(equdistant=false));
      end TestBasePropertiesDynamic;

      model TestBasePropertiesTranscritical
        "Test case using BaseProperties and explicit equations"
        extends GenericModels.TestBasePropertiesExplicit(redeclare package
            Medium = ExternalMedia.Examples.CO2StanMix);

      equation
        p1 = 1e6 + time*10e6;
        h1 = -4.2e5 + 0*time;
        p2 = 1e6 + time*10e6;
        h2 = 2.0e5;
      end TestBasePropertiesTranscritical;
    end CO2StanMix;

    package CO2RefProp "Test suite for the StanMix CO2 medium model"

      model TestStatesSupercritical
        "Test case with state records, supercritical conditions"
        extends GenericModels.TestStates(redeclare package Medium =
              ExternalMedia.Examples.CO2RefProp);
      equation
        p1 = 8e6;
        h1 = 1.0e5 + 6e5*time;
        p2 = 8e6;
        T2 = 280 + 50*time;
      end TestStatesSupercritical;

      model TestStatesTranscritical
        "Test case with state records, transcritical conditions"
        extends GenericModels.TestStates(redeclare package Medium =
              ExternalMedia.Examples.CO2RefProp);
      equation
        p1 = 1e6 + time*10e6;
        h1 = 1.0e5;
        p2 = 1e6 + time*10e6;
        T2 = 330;
      end TestStatesTranscritical;

      model TestStatesSatSubcritical
        "Test case state + sat records, subcritical conditions"
        extends GenericModels.TestStatesSat(redeclare package Medium =
              ExternalMedia.Examples.CO2RefProp);
      equation
        p1 = 1e6;
        h1 = 1.0e5 + 6e5*time;
        p2 = 1e6;
        T2 = 250 + 50*time;
      end TestStatesSatSubcritical;

      model TestBasePropertiesExplicit
        "Test case using BaseProperties and explicit equations"
        extends GenericModels.TestBasePropertiesExplicit(redeclare package
            Medium = ExternalMedia.Examples.CO2RefProp);

      equation
        p1 = 8e6;
        h1 = 1.0e5 + 6e5*time;
        p2 = 1e6;
        h2 = 1.0e5 + 6e5*time;
      end TestBasePropertiesExplicit;

      model TestBasePropertiesImplicit
        "Test case using BaseProperties and implicit equations"
        extends GenericModels.TestBasePropertiesImplicit(redeclare package
            Medium = ExternalMedia.Examples.CO2RefProp, hstart=1e5);
      equation
        p1 = 8e6;
        T1 = 280 + 50*time;
        p2 = 1e6;
        T2 = 280 + 50*time;
      end TestBasePropertiesImplicit;

      model TestBasePropertiesDynamic
        "Test case using BaseProperties and dynamic equations"
        extends GenericModels.TestBasePropertiesDynamic(
          redeclare package Medium = ExternalMedia.Examples.CO2RefProp,
          Tstart=300,
          hstart=4e5,
          pstart=1e6,
          Kv0=1.00801e-4,
          V=0.1);
      equation
        // Inlet equations
        win = 1;
        hin = 5e5;

        // Input variables
        Kv = if time < 50 then Kv0 else Kv0*1.1;
        Q = if time < 1 then 0 else 1e4;
        annotation (experiment(StopTime=80, Tolerance=1e-007),
            experimentSetupOutput(equdistant=false));
      end TestBasePropertiesDynamic;

      model TestBasePropertiesTranscritical
        "Test case using BaseProperties and explicit equations"
        extends GenericModels.TestBasePropertiesExplicit(redeclare package
            Medium = ExternalMedia.Examples.CO2RefProp);

      equation
        p1 = 1e6 + time*10e6;
        h1 = 1.0e5;
        p2 = 1e6 + time*10e6;
        h2 = 7.0e5;
      end TestBasePropertiesTranscritical;
    end CO2RefProp;
  end FluidProp;

  package WrongMedium "Test cases with wrong medium models"

    model TestWrongMedium
      "Test the error reporting messages for unsupported external media"
      package Medium = Media.ExternalTwoPhaseMedium;
      Medium.BaseProperties medium;
    equation
      medium.p = 1e5;
      medium.h = 1e5;
    end TestWrongMedium;
  end WrongMedium;

  package TestOMC "Test cases for OpenModelica implementation"
    package TestHelium
      "Test for NIST Helium model using ExternalMedia and FluidProp"

      package Helium "Helium model from NIST RefProp database"
        extends ExternalMedia.Media.ExternalTwoPhaseMedium(
          mediumName="Helium",
          libraryName="FluidProp.RefProp",
          substanceNames={"He"},
          ThermoStates=Modelica.Media.Interfaces.Choices.IndependentVariables.ph,
          AbsolutePressure(
            min=500,
            max=44e5,
            nominal=1e5,
            start=1e5),
          Density(
            min=0.1,
            max=200,
            nominal=100,
            start=100),
          SpecificEnthalpy(
            min=-6000,
            max=1.7e6,
            nominal=1000,
            start=0),
          SpecificEntropy(
            min=-4000,
            max=30e3,
            nominal=1000,
            start=0),
          Temperature(
            min=2.17,
            max=310,
            nominal=10,
            start=5,
            displayUnit="K"));

      end Helium;

      model TestSupercriticalHelium
        package Medium = Helium;
        Medium.ThermodynamicState state;
        Medium.Temperature T;
        Medium.Temperature Tcrit=Medium.fluidConstants[1].criticalTemperature;
        Medium.AbsolutePressure p;
        Modelica.SIunits.Density d;
        Medium.AbsolutePressure pcrit=Medium.fluidConstants[1].criticalPressure;
      equation
        T = 300 - 297.5*time;
        p = 4e5 + 0*time;
        state = Medium.setState_pT(p, T);
        d = Medium.density(state);
      end TestSupercriticalHelium;

      model TestSaturatedHelium
        package Medium = Helium;
        Medium.SaturationProperties sat;
        Medium.Temperature T;
        Medium.AbsolutePressure p;
        Modelica.SIunits.Density dl;
        Modelica.SIunits.Density dv;
      equation
        p = 1e5 + 1.27e5*time;
        sat = Medium.setSat_p(p);
        dv = Medium.dewDensity(sat);
        dl = Medium.bubbleDensity(sat);
        T = Medium.saturationTemperature_sat(sat);
      end TestSaturatedHelium;

      model TypicalHeliumProperties
        package Medium = Helium;
        Medium.ThermodynamicState state;
        Medium.Temperature T;
        Medium.Temperature Tcrit=Medium.fluidConstants[1].criticalTemperature;
        Medium.AbsolutePressure p;
        Modelica.SIunits.Density d;
        Medium.AbsolutePressure pcrit=Medium.fluidConstants[1].criticalPressure;
        Modelica.SIunits.SpecificHeatCapacity cv=Medium.specificHeatCapacityCv(
            state);
      equation
        T = 5;
        p = 5e5;
        state = Medium.setState_pT(p, T);
        d = Medium.density(state);
      end TypicalHeliumProperties;
    end TestHelium;

    package TestHeliumHardCodedProperties
      "Test for NIST Helium model using ExternalMedia and FluidProp, hard-coded fluid properties package constants"

      package Helium "Helium model from NIST RefProp database"
        extends ExternalMedia.Media.ExternalTwoPhaseMedium(
          mediumName="Helium",
          libraryName="FluidProp.RefProp",
          substanceNames={"He"},
          externalFluidConstants=FluidConstants(
                    iupacName="unknown",
                    casRegistryNumber="unknown",
                    chemicalFormula="unknown",
                    structureFormula="unknown",
                    molarMass=4.0026e-3,
                    criticalTemperature=5.1953,
                    criticalPressure=2.2746e5,
                    criticalMolarVolume=1/69.641*4.0026e-3,
                    acentricFactor=0,
                    triplePointTemperature=280.0,
                    triplePointPressure=500.0,
                    meltingPoint=280,
                    normalBoilingPoint=380.0,
                    dipoleMoment=2.0),
          ThermoStates=Modelica.Media.Interfaces.Choices.IndependentVariables.ph,
          AbsolutePressure(
            min=500,
            max=44e5,
            nominal=1e5,
            start=1e5),
          Density(
            min=0.1,
            max=200,
            nominal=100,
            start=100),
          SpecificEnthalpy(
            min=-6000,
            max=1.7e6,
            nominal=1000,
            start=0),
          SpecificEntropy(
            min=-4000,
            max=30e3,
            nominal=1000,
            start=0),
          Temperature(
            min=2.17,
            max=310,
            nominal=10,
            start=5,
            displayUnit="K"));

      end Helium;

      model TestSupercriticalHelium
        package Medium = Helium;
        Medium.ThermodynamicState state;
        Medium.Temperature T;
        Medium.Temperature Tcrit=Medium.fluidConstants[1].criticalTemperature;
        Medium.AbsolutePressure p;
        Modelica.SIunits.Density d;
        Medium.AbsolutePressure pcrit=Medium.fluidConstants[1].criticalPressure;
      equation
        T = 300 - 297.5*time;
        p = 4e5 + 0*time;
        state = Medium.setState_pT(p, T);
        d = Medium.density(state);
      end TestSupercriticalHelium;

      model TestSaturatedHelium
        package Medium = Helium;
        Medium.SaturationProperties sat;
        Medium.Temperature T;
        Medium.AbsolutePressure p;
        Modelica.SIunits.Density dl;
        Modelica.SIunits.Density dv;
      equation
        p = 1e5 + 1.27e5*time;
        sat = Medium.setSat_p(p);
        dv = Medium.dewDensity(sat);
        dl = Medium.bubbleDensity(sat);
        T = Medium.saturationTemperature_sat(sat);
      end TestSaturatedHelium;

      model TypicalHeliumProperties
        package Medium = Helium;
        Medium.ThermodynamicState state;
        Medium.Temperature T;
        Medium.Temperature Tcrit=Medium.fluidConstants[1].criticalTemperature;
        Medium.AbsolutePressure p;
        Modelica.SIunits.Density d;
        Medium.AbsolutePressure pcrit=Medium.fluidConstants[1].criticalPressure;
        Modelica.SIunits.SpecificHeatCapacity cv=Medium.specificHeatCapacityCv(
            state);
      equation
        T = 5;
        p = 5e5;
        state = Medium.setState_pT(p, T);
        d = Medium.density(state);
      end TypicalHeliumProperties;
    end TestHeliumHardCodedProperties;
  end TestOMC;
end Test;
