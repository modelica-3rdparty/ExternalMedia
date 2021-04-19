/*!
  \file externalmedialib.h
  \brief Header file to be included in the Modelica tool, with external function interfaces

  C/C++ layer for external medium models extending from
  PartialExternalTwoPhaseMedium.

  Francesco Casella, Christoph Richter, Roberto Bonifetto
  2006-2012
  Copyright Politecnico di Milano, TU Braunschweig, Politecnico di Torino

  Minor additions in 2014 to make ExternalMedia compatible
  with GCC on Linux operating systems
  Jorrit Wronski (Technical University of Denmark)
*/

#ifndef EXTERNALMEDIALIB_H_
#define EXTERNALMEDIALIB_H_

// Constants for input choices (see ExternalMedia.Common.InputChoices)
#define CHOICE_dT 1
#define CHOICE_hs 2
#define CHOICE_ph 3
#define CHOICE_ps 4
#define CHOICE_pT 5

/*!
Portable definitions of the EXPORT macro
 */
#if !defined(EXTERNALMEDIA_EXPORT)
#  if !defined(EXTERNALMEDIA_LIBRARY_EXPORTS)
#    define EXTERNALMEDIA_EXPORT
#  else
#    if (EXTERNALMEDIA_LIBRARY_EXPORTS == 1)
#      if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__WIN64__)
#        if !defined(__EXTERNALMEDIA_ISWINDOWS__)
#          define __EXTERNALMEDIA_ISWINDOWS__
#        endif
#      elif __APPLE__
#        if !defined(__EXTERNALMEDIA_ISAPPLE__)
#          define __EXTERNALMEDIA_ISAPPLE__
#        endif
#      elif __linux
#        if !defined(__EXTERNALMEDIA_ISLINUX__)
#          define __EXTERNALMEDIA_ISLINUX__
#        endif
#      endif
#
#      if defined(__EXTERNALMEDIA_ISLINUX__)
#        define EXTERNALMEDIA_EXPORT
#      elif defined(__EXTERNALMEDIA_ISAPPLE__)
#        define EXTERNALMEDIA_EXPORT
#      else
#        define EXTERNALMEDIA_EXPORT __declspec(dllexport)
#      endif
#    else
#      define EXTERNALMEDIA_EXPORT
#    endif
#  endif
#endif

// Define struct
//! ExternalThermodynamicState property struct
/*!
  The ExternalThermodynamicState propery struct defines all the properties that
  are computed by external Modelica medium models extending from
  PartialExternalTwoPhaseMedium.
*/

typedef struct ExternalThermodynamicState {

	//! Temperature
    double T;
	//! Velocity of sound
    double a;
	//! Isobaric expansion coefficient
    double beta;
	//! Specific heat capacity cp
    double cp;
	//! Specific heat capacity cv
    double cv;
	//! Density
    double d;
	//! Derivative of density wrt enthalpy at constant pressure
    double ddhp;
	//! Derivative of density wrt pressure at constant enthalpy
    double ddph;
	//! Dynamic viscosity
    double eta;
	//! Specific enthalpy
    double h;
	//! Compressibility
    double kappa;
	//! Thermal conductivity
    double lambda;
	//! Pressure
    double p;
	//! Phase flag: 2 for two-phase, 1 for one-phase
    int phase;
	//! Specific entropy
    double s;

	//! Constructor.
	/*!
	  The constructor only initializes the variables.
	*/
	#ifdef __cplusplus
	ExternalThermodynamicState() : T(-1), a(-1), beta(-1), cp(-1), cv(-1), d(-1), ddhp(-1), ddph(-1), eta(-1), h(-1), kappa(-1), lambda(-1), p(-1), phase(-1), s(-1) {};
	#endif

} ExternalThermodynamicState;

//! ExternalSaturationProperties property struct
/*!
  The ExternalSaturationProperties propery struct defines all the saturation properties
  for the dew and the bubble line that are computed by external Modelica medium models
  extending from PartialExternalTwoPhaseMedium.
*/

typedef struct ExternalSaturationProperties {
	//! Saturation temperature
    double Tsat;
	//! Derivative of Ts wrt pressure
    double dTp;
	//! Derivative of dls wrt pressure
    double ddldp;
	//! Derivative of dvs wrt pressure
    double ddvdp;
	//! Derivative of hls wrt pressure
    double dhldp;
	//! Derivative of hvs wrt pressure
    double dhvdp;
	//! Density at bubble line (for pressure ps)
    double dl;
	//! Density at dew line (for pressure ps)
    double dv;
	//! Specific enthalpy at bubble line (for pressure ps)
    double hl;
	//! Specific enthalpy at dew line (for pressure ps)
    double hv;
	//! Saturation pressure
    double psat;
	//! Surface tension
    double sigma;
	//! Specific entropy at bubble line (for pressure ps)
    double sl;
	//! Specific entropy at dew line (for pressure ps)
    double sv;

	//! Constructor.
	/*!
	  The constructor only initializes the variables.
	*/
	#ifdef __cplusplus
	ExternalSaturationProperties() : Tsat(-1), dTp(-1), ddldp(-1), ddvdp(-1), dhldp(-1), dhvdp(-1), dl(-1), dv(-1), hl(-1), hv(-1), psat(-1), sigma(-1), sl(-1), sv(-1) {};
	#endif

} ExternalSaturationProperties;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_getMolarMass_C_impl(const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_getCriticalTemperature_C_impl(const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_getCriticalPressure_C_impl(const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_getCriticalMolarVolume_C_impl(const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_ph_C_impl(double p, double h, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_pT_C_impl(double p, double T, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_dT_C_impl(double d, double T, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_ps_C_impl(double p, double s, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_hs_C_impl(double h, double s, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_partialDeriv_state_C_impl(const char *of, const char *wrt, const char *cst, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_prandtlNumber_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_temperature_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_velocityOfSound_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_isobaricExpansionCoefficient_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_specificHeatCapacityCp_C_impl(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_specificHeatCapacityCv_C_impl(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_density_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_density_derh_p_C_impl(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_density_derp_h_C_impl(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dynamicViscosity_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_specificEnthalpy_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_isothermalCompressibility_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_thermalConductivity_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_pressure_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_specificEntropy_C_impl(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_density_ph_der_C_impl(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_isentropicEnthalpy_C_impl(double p_downstream, ExternalThermodynamicState *refState,	const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setSat_p_C_impl(double p, ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setSat_T_C_impl(double T, ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setBubbleState_C_impl(ExternalSaturationProperties *sat, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setDewState_C_impl(ExternalSaturationProperties *sat, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_saturationTemperature_C_impl(double p, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_saturationTemperature_derp_C_impl(double p, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_saturationTemperature_derp_sat_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dBubbleDensity_dPressure_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dDewDensity_dPressure_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dBubbleEnthalpy_dPressure_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dDewEnthalpy_dPressure_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_bubbleDensity_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dewDensity_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_bubbleEnthalpy_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dewEnthalpy_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_saturationPressure_C_impl(double T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_surfaceTension_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_bubbleEntropy_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dewEntropy_C_impl(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /*EXTERNALMEDIALIB_H_*/
