/*! 
  \file externaltwophasemedium.h
  \brief Header file to be included in Dymola
  
  This is the header file to be included in the Dymola/Source directory.
  It proived function prototypes for all the external functions needed
  by medium models extending from PartialExternalTwoPhaseMedium.

  Please be aware that other Modelica tools might require a slightly
  different header files depending on the compiler.

  Francesco Casella, Christoph Richter Sep 2006
*/

#ifndef EXTERNALTWOPHASEMEDIUM_H_
#define EXTERNALTWOPHASEMEDIUM_H_

// Define export
#ifdef __cplusplus
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	EXPORT int createMedium_(const char *mediumName, const char *libraryName, const char *substanceName, int oldUniqueID);
	
	EXPORT double getMolarMass_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double getCriticalTemperature_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double getCriticalPressure_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double getCriticalMolarVolume_(const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setSat_p_state_(int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID);
	EXPORT void setDewState_(int uniqueID, int phase, int *state_uniqueID, int *state_phase, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setBubbleState_(int uniqueID, int phase, int *state_uniqueID, int *state_phase, const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double saturationPressure_(double T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double saturationTemperature_(double p, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double saturationPressure_sat_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double saturationTemperature_sat_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double saturationTemperature_derp_sat_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);

	EXPORT double bubbleDensity_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double dewDensity_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double bubbleEnthalpy_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double dewEnthalpy_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double bubbleEntropy_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double dewEntropy_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double dBubbleDensity_dPressure_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double dDewDensity_dPressure_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double dBubbleEnthalpy_dPressure_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double dDewEnthalpy_dPressure_(double psat, double Tsat, int uniqueID, const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T, const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double density_(int uniqueID);
	EXPORT double density_ph_der_(int uniqueID, double p_der, double h_der);
	EXPORT double density_derp_h_(int uniqueID);
	EXPORT double density_derh_p_(int uniqueID);
	EXPORT double pressure_(int uniqueID);
	EXPORT double specificEnthalpy_(int uniqueID);
	EXPORT double specificEntropy_(int uniqueID);
	EXPORT double temperature_(int uniqueID);
	
	EXPORT double isobaricExpansionCoefficient_(int uniqueID);
	EXPORT double isothermalCompressibility_(int uniqueID);
	EXPORT double specificHeatCapacityCp_(int uniqueID);
	EXPORT double specificHeatCapacityCv_(int uniqueID);

	EXPORT double isentropicEnthalpy_(double p, int uniqueID);
	
	EXPORT double dynamicViscosity_(int uniqueID);
	EXPORT double thermalConductivity_(int uniqueID);
	EXPORT double prandtlNumber_(int uniqueID);
	EXPORT double surfaceTension_(double psat, double Tsat, int uniqueID);
	
	EXPORT double dDensity_dPressure_h_(int uniqueID);
	EXPORT double dDensity_dEnthalpy_p_(int uniqueID);
	EXPORT double temperature_ph_der_(int uniqueID, double p_der, double h_der);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /*EXTERNALTWOPHASEMEDIUM_H_*/
