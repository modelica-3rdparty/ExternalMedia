/*! 
  \file externalmedialib.h
  \brief Header file to be included in Dymola
  
  This is the header file to be included in the Dymola/Source directory.
  It proived function prototypes for all the external functions needed
  by medium models extending from PartialExternalTwoPhaseMedium.

  Please be aware that other Modelica tools might require a slightly
  different header files depending on the compiler.

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/

#ifndef EXTERNALMEDIALIB_H_
#define EXTERNALMEDIALIB_H_

// Constants for input choices (see ExternalMedia.Common.InputChoices)
#define CHOICE_dT 0
#define CHOICE_ph 1
#define CHOICE_ps 2
#define CHOICE_pT 3

// Define export
#ifdef __cplusplus
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	EXPORT int TwoPhaseMedium_createMedium_(const char *mediumName, const char *libraryName, const char *substanceName, int oldUniqueID);
	
	EXPORT double TwoPhaseMedium_getMolarMass_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_getCriticalTemperature_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_getCriticalPressure_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_getCriticalMolarVolume_(const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT void TwoPhaseMedium_setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setSat_p_state_(int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID);
	EXPORT void TwoPhaseMedium_setDewState_(int uniqueID, int phase, int *state_uniqueID, int *state_phase, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setBubbleState_(int uniqueID, int phase, int *state_uniqueID, int *state_phase, const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double TwoPhaseMedium_saturationPressure_(double T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_saturationTemperature_(double p, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_saturationPressure_sat_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_saturationTemperature_sat_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_saturationTemperature_derp_sat_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);

	EXPORT double TwoPhaseMedium_bubbleDensity_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dewDensity_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_bubbleEnthalpy_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dewEnthalpy_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_bubbleEntropy_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dewEntropy_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dBubbleDensity_dPressure_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dDewDensity_dPressure_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dBubbleEnthalpy_dPressure_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dDewEnthalpy_dPressure_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT void TwoPhaseMedium_setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double TwoPhaseMedium_density_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_density_ph_der_(int uniqueID, double p_der, double h_der, double p, double h, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_density_derp_h_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_density_derh_p_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_pressure_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_specificEnthalpy_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_specificEntropy_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_temperature_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double TwoPhaseMedium_isobaricExpansionCoefficient_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_isothermalCompressibility_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_specificHeatCapacityCp_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_specificHeatCapacityCv_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);

	EXPORT double TwoPhaseMedium_isentropicEnthalpy_(double p_iso, int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double TwoPhaseMedium_dynamicViscosity_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_thermalConductivity_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_prandtlNumber_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_surfaceTension_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);

	EXPORT double TwoPhaseMedium_velocityOfSound_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double TwoPhaseMedium_dDensity_dPressure_h_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dDensity_dEnthalpy_p_(int uniqueID, int choice, double d, double h, double p, double s, double T, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_temperature_ph_der_(int uniqueID, double p_der, double h_der, double p, double h, int phase,
		const char *mediumName, const char *libraryName, const char *substanceName);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /*EXTERNALMEDIALIB_H_*/
