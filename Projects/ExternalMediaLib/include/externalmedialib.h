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
  
  Adapted to work with dynamically linked libraries by Francesco Casella and
  Federico Terraneo 2022 (Politecnico di Milano)
*/

#ifndef EXTERNALMEDIALIB_H_
#define EXTERNALMEDIALIB_H_

/* Constants for input choices (see ExternalMedia.Common.InputChoices) */
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
#        define EXTERNALMEDIA_EXPORT __attribute__((visibility("default")))
#      elif defined(__EXTERNALMEDIA_ISAPPLE__)
#        define EXTERNALMEDIA_EXPORT __attribute__((visibility("default")))
#      else
#        define EXTERNALMEDIA_EXPORT __declspec(dllexport)
#      endif
#    else
#      define EXTERNALMEDIA_EXPORT
#    endif
#  endif
#endif

#include "ThermodynamicStructs.h"




#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_getMolarMass_C_impl(const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_getCriticalTemperature_C_impl(const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_getCriticalPressure_C_impl(const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_getCriticalMolarVolume_C_impl(const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_ph_C_impl(double p, double h, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_pT_C_impl(double p, double T,            void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_dT_C_impl(double d, double T, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_ps_C_impl(double p, double s, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_hs_C_impl(double h, double s, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName);

	/* These functions implement a workaround to handle ModelicaError and ModelicaWarning on Windows until a proper solution based on exporting symbols becomes available in Modelica tools */
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_ph_C_impl_err(double p, double h, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *));
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_pT_C_impl_err(double p, double T,            void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *));
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_dT_C_impl_err(double d, double T, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *));
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_ps_C_impl_err(double p, double s, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *));
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setState_hs_C_impl_err(double h, double s, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *));

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_partialDeriv_state_C_impl(const char *of, const char *wrt, const char *cst, void *state, const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_prandtlNumber_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_temperature_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_velocityOfSound_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_isobaricExpansionCoefficient_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_specificHeatCapacityCp_C_impl(void *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_specificHeatCapacityCv_C_impl(void *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_density_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_density_derh_p_C_impl(void *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_density_derp_h_C_impl(void *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dynamicViscosity_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_specificEnthalpy_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_isothermalCompressibility_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_thermalConductivity_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_pressure_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_specificEntropy_C_impl(void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_density_ph_der_C_impl(void *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_isentropicEnthalpy_C_impl(double p_downstream, void *refState,	const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setSat_p_C_impl(double p, void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setSat_T_C_impl(double T, void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setBubbleState_C_impl(void *sat, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT void TwoPhaseMedium_setDewState_C_impl(void *sat, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_saturationTemperature_C_impl(double p, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_saturationTemperature_derp_C_impl(double p, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_saturationTemperature_derp_sat_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);

	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dBubbleDensity_dPressure_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dDewDensity_dPressure_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dBubbleEnthalpy_dPressure_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dDewEnthalpy_dPressure_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_bubbleDensity_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dewDensity_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_bubbleEnthalpy_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dewEnthalpy_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_saturationPressure_C_impl(double T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_surfaceTension_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_bubbleEntropy_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXTERNALMEDIA_EXPORT double TwoPhaseMedium_dewEntropy_C_impl(void *sat, const char *mediumName, const char *libraryName, const char *substanceName);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /*EXTERNALMEDIALIB_H_*/
