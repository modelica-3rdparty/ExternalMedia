/* *****************************************************************
 * Header file for externaltwophasemedium_lib.cpp
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/
#ifndef EXTERNALTWOPHASEMEDIUM_LIB_H_
#define EXTERNALTWOPHASEMEDIUM_LIB_H_

#ifdef _USRDLL
#define DLLExport __declspec(dllexport)
#else
#define DLLExport
#endif /*_USRDLL*/

extern "C" DLLExport int createMedium_(const char *mediumName, const char *libraryName, 
									   const char *substanceName, int oldUniqueID);

extern "C" DLLExport double molarMass_(int uniqueID);
extern "C" DLLExport double criticalDensity_(int uniqueID);
extern "C" DLLExport double criticalPressure_(int uniqueID);
extern "C" DLLExport double criticalTemperature_(int uniqueID);
extern "C" DLLExport double criticalEnthalpy_(int uniqueID);
extern "C" DLLExport double criticalEntropy_(int uniqueID);

extern "C" DLLExport void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			                        const char *mediumName, const char *libraryName, const char *substanceName);
extern "C" DLLExport void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			                        const char *mediumName, const char *libraryName, const char *substanceName);

extern "C" DLLExport double saturationPressure_(double T, const char *mediumName, const char *libraryName, const char *substanceName);
extern "C" DLLExport double saturationTemperature_(double p, const char *mediumName, const char *libraryName, const char *substanceName);

extern "C" DLLExport double bubbleDensity_(int uniqueID);
extern "C" DLLExport double dewDensity_(int uniqueID);
extern "C" DLLExport double bubbleEnthalpy_(int uniqueID);
extern "C" DLLExport double dewEnthalpy_(int uniqueID);
extern "C" DLLExport double bubbleEntropy_(int uniqueID);
extern "C" DLLExport double dewEntropy_(int uniqueID);

extern "C" DLLExport void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
			   	                       const char *mediumName, const char *libraryName, const char *substanceName);
extern"C" DLLExport  void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				                       const char *mediumName, const char *libraryName, const char *substanceName);
extern "C" DLLExport void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				                       const char *mediumName, const char *libraryName, const char *substanceName);
extern "C" DLLExport void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				                       const char *mediumName, const char *libraryName, const char *substanceName);

extern "C" DLLExport double density_(int uniqueID);
extern "C" DLLExport double density_ph_der_(int uniqueID, double p_der, double h_der);

extern "C" DLLExport double pressure_(int uniqueID);
extern "C" DLLExport double specificEnthalpy_(int uniqueID);
extern "C" DLLExport double specificEntropy_(int uniqueID);
extern "C" DLLExport double temperature_(int uniqueID);

extern "C" DLLExport double isobaricExpansionCoefficient_(int uniqueID);
extern "C" DLLExport double isothermalCompressibility_(int uniqueID);
extern "C" DLLExport double specificHeatCapacityCp_(int uniqueID);
extern "C" DLLExport double specificHeatCapacityCv_(int uniqueID);

extern "C" DLLExport double dynamicViscosity_(int uniqueID);
extern "C" DLLExport double thermalConductivity_(int uniqueID);
extern "C" DLLExport double prandtlNumber_(int uniqueID);
extern "C" DLLExport double surfaceTension_(double psat, double Tsat, int uniqueID);

#endif /*EXTERNALTWOPHASEMEDIUM_LIB_H_*/
