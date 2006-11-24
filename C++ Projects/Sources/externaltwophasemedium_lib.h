/* *****************************************************************
 * Header file for externaltwophasemedium_lib.cpp
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/
 
#ifndef EXTERNALTWOPHASEMEDIUM_LIB_H_
#define EXTERNALTWOPHASEMEDIUM_LIB_H_

#include "include.h"

#if defined BUILD_DLL
#define MEDIA_DLL __declspec(dllexport)
#elif defined BUILD_LIB
#define MEDIA_DLL __declspec(dllexport)
#else
#define MEDIA_DLL __declspec(dllimport)
#endif // BUILDING_MEDIA_DLL

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	MEDIA_DLL int createMedium_(const char *mediumName, const char *libraryName, 
								const char *substanceName, int oldUniqueID);

	MEDIA_DLL double molarMass_(int uniqueID);

	MEDIA_DLL void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
							 const char *mediumName, const char *libraryName, const char *substanceName);
	MEDIA_DLL void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
							 const char *mediumName, const char *libraryName, const char *substanceName);

	MEDIA_DLL double saturationPressure_(double T, const char *mediumName, const char *libraryName, const char *substanceName);
	MEDIA_DLL double saturationTemperature_(double p, const char *mediumName, const char *libraryName, const char *substanceName);

	MEDIA_DLL double saturationPressure_sat_(int uniqueID);
	MEDIA_DLL double saturationTemperature_sat_(int uniqueID);

	MEDIA_DLL double bubbleDensity_(int uniqueID);
	MEDIA_DLL double dewDensity_(int uniqueID);
	MEDIA_DLL double bubbleEnthalpy_(int uniqueID);
	MEDIA_DLL double dewEnthalpy_(int uniqueID);
	MEDIA_DLL double bubbleEntropy_(int uniqueID);
	MEDIA_DLL double dewEntropy_(int uniqueID);
    MEDIA_DLL double dBubbleDensity_dPressure_(int uniqueID);
	MEDIA_DLL double dDewDensity_dPressure_(int uniqueID);
	MEDIA_DLL double dBubbleEnthalpy_dPressure_(int uniqueID);
	MEDIA_DLL double dDewEnthalpy_dPressure_(int uniqueID);

	MEDIA_DLL void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
								const char *mediumName, const char *libraryName, const char *substanceName);
	MEDIA_DLL void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
								const char *mediumName, const char *libraryName, const char *substanceName);
	MEDIA_DLL void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
								const char *mediumName, const char *libraryName, const char *substanceName);
	MEDIA_DLL void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
								const char *mediumName, const char *libraryName, const char *substanceName);

	MEDIA_DLL double density_(int uniqueID);
	MEDIA_DLL double density_ph_der_(int uniqueID, double p_der, double h_der);

	MEDIA_DLL double pressure_(int uniqueID);
	MEDIA_DLL double specificEnthalpy_(int uniqueID);
	MEDIA_DLL double specificEntropy_(int uniqueID);
	MEDIA_DLL double temperature_(int uniqueID);

	MEDIA_DLL double isobaricExpansionCoefficient_(int uniqueID);
	MEDIA_DLL double isothermalCompressibility_(int uniqueID);
	MEDIA_DLL double specificHeatCapacityCp_(int uniqueID);
	MEDIA_DLL double specificHeatCapacityCv_(int uniqueID);

	MEDIA_DLL double dynamicViscosity_(int uniqueID);
	MEDIA_DLL double thermalConductivity_(int uniqueID);
	MEDIA_DLL double prandtlNumber_(int uniqueID);
	MEDIA_DLL double surfaceTension_(double psat, double Tsat, int uniqueID);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /*EXTERNALTWOPHASEMEDIUM_LIB_H_*/
