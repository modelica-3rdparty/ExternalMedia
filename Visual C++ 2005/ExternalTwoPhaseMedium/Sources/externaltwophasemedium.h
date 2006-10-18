/* *****************************************************************
 * Header file to include in the Dymola/Source directory 
 *
 * Provides function prototypes for all the external functions needed
 * by medium model extending from PartialExternalTwoPhaseMedium
 *
 * Francesco Casella, Christoph Richter Sep 2006
 ********************************************************************/


#ifndef EXTERNALTWOPHASEMEDIUM_H_
#define EXTERNALTWOPHASEMEDIUM_H_

extern "C" int createMedium_(const char *mediumName, const char *libraryName, const char *substanceName, int oldUniqueID);

extern "C" double molarMass_(int uniqueID);
extern "C" double criticalDensity_(int uniqueID);
extern "C" double criticalPressure_(int uniqueID);
extern "C" double criticalTemperature_(int uniqueID);

extern "C" void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName);
extern "C" void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName);

extern "C"double saturationPressure_(double T, const char *mediumName);
extern "C"double saturationTemperature_(double p, const char *mediumName);

extern "C" double bubbleDensity_(int uniqueID);
extern "C" double dewDensity_(int uniqueID);
extern "C" double bubbleEnthalpy_(int uniqueID);
extern "C" double dewEnthalpy_(int uniqueID);
extern "C" double bubbleEntropy_(int uniqueID);
extern "C" double dewEntropy_(int uniqueID);

extern "C" void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
			   	         const char *mediumName, const char *libraryName, const char *substanceName);
extern "C" void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				         const char *mediumName, const char *libraryName, const char *substanceName);
extern "C" void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				         const char *mediumName, const char *libraryName, const char *substanceName);
extern "C" void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				         const char *mediumName, const char *libraryName, const char *substanceName);

extern "C" double density_(int uniqueID);
extern "C" double density_ph_der_(int uniqueID, double p_der, double h_der);
extern "C" double pressure_(int uniqueID);
extern "C" double specificEnthalpy_(int uniqueID);
extern "C" double specificEntropy_(int uniqueID);
extern "C" double temperature_(int uniqueID);

extern "C" double isobaricExpansionCoefficient_(int uniqueID);
extern "C" double isothermalCompressibility_(int uniqueID);
extern "C" double specificHeatCapacityCp_(int uniqueID);
extern "C" double specificHeatCapacityCv_(int uniqueID);

extern "C" double dynamicViscosity_(int uniqueID);
extern "C" double thermalConductivity_(int uniqueID);
extern "C" double prandtlNumber_(int uniqueID);
extern "C" double surfaceTension_(double psat, double Tsat, int uniqueID);

#endif /*EXTERNALTWOPHASEMEDIUM_H_*/
