/* *****************************************************************
 * Header file for externaltwophasemedium_lib.cpp
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/
 
#ifndef EXTERNALTWOPHASEMEDIUM_LIB_H_
#define EXTERNALTWOPHASEMEDIUM_LIB_H_

#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	EXPORT int createMedium_(const char *mediumName, const char *libraryName, 
								const char *substanceName, int oldUniqueID);

	EXPORT double molarMass_(int uniqueID);

	EXPORT void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
							 const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
							 const char *mediumName, const char *libraryName, const char *substanceName);

	EXPORT double saturationPressure_(double T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double saturationTemperature_(double p, const char *mediumName, const char *libraryName, const char *substanceName);

	EXPORT double saturationPressure_sat_(int uniqueID);
	EXPORT double saturationTemperature_sat_(int uniqueID);

	EXPORT double bubbleDensity_(int uniqueID);
	EXPORT double dewDensity_(int uniqueID);
	EXPORT double bubbleEnthalpy_(int uniqueID);
	EXPORT double dewEnthalpy_(int uniqueID);
	EXPORT double bubbleEntropy_(int uniqueID);
	EXPORT double dewEntropy_(int uniqueID);
    EXPORT double dBubbleDensity_dPressure_(int uniqueID);
	EXPORT double dDewDensity_dPressure_(int uniqueID);
	EXPORT double dBubbleEnthalpy_dPressure_(int uniqueID);
	EXPORT double dDewEnthalpy_dPressure_(int uniqueID);

	EXPORT void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
								const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
								const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
								const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
								const char *mediumName, const char *libraryName, const char *substanceName);

	EXPORT double density_(int uniqueID);
	EXPORT double density_ph_der_(int uniqueID, double p_der, double h_der);

	EXPORT double pressure_(int uniqueID);
	EXPORT double specificEnthalpy_(int uniqueID);
	EXPORT double specificEntropy_(int uniqueID);
	EXPORT double temperature_(int uniqueID);

	EXPORT double isobaricExpansionCoefficient_(int uniqueID);
	EXPORT double isothermalCompressibility_(int uniqueID);
	EXPORT double specificHeatCapacityCp_(int uniqueID);
	EXPORT double specificHeatCapacityCv_(int uniqueID);

	EXPORT double dynamicViscosity_(int uniqueID);
	EXPORT double thermalConductivity_(int uniqueID);
	EXPORT double prandtlNumber_(int uniqueID);
	EXPORT double surfaceTension_(double psat, double Tsat, int uniqueID);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /*EXTERNALTWOPHASEMEDIUM_LIB_H_*/
