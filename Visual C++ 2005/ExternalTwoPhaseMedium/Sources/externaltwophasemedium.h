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

extern int createMedium_(const char *mediumName, 
						 const char *libraryName,
						 const char *substanceName,
						 int oldUniqueID);

extern void deleteMedium_(int uniqueID);

extern double molarMass_(int uniqueID);
extern double criticalDensity_(int uniqueID);
extern double criticalPressure_(int uniqueID);
extern double criticalTemperature_(int uniqueID);

void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName);
void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName);

extern double saturationPressure_(double T, const char *mediumName);
extern double saturationTemperature_(double p, const char *mediumName);

extern double bubbleDensity_(int uniqueID);
extern double dewDensity_(int uniqueID);
extern double bubbleEnthalpy_(int uniqueID);
extern double dewEnthalpy_(int uniqueID);
extern double bubbleEntropy_(int uniqueID);
extern double dewEntropy_(int uniqueID);

extern void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
			   	         const char *mediumName, const char *libraryName, const char *substanceName);
extern void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				         const char *mediumName, const char *libraryName, const char *substanceName);
extern void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				         const char *mediumName, const char *libraryName, const char *substanceName);
extern void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				         const char *mediumName, const char *libraryName, const char *substanceName);

extern double density_(int uniqueID);
extern double density_ph_der_(int uniqueID, double p_der, double h_der);
extern double pressure_(int uniqueID);
extern double specificEnthalpy_(int uniqueID);
extern double specificEntropy_(int uniqueID);
extern double temperature_(int uniqueID);

extern double isobaricExpansionCoefficient_(int uniqueID);
extern double isothermalCompressibility_(int uniqueID);
extern double specificHeatCapacityCp_(int uniqueID);
extern double specificHeatCapacityCv_(int uniqueID);

extern double dynamicViscosity_(int uniqueID);
extern double thermalConductivity_(int uniqueID);
extern double prandtlNumber_(int uniqueID);
extern double surfaceTension_(double psat, double Tsat, int uniqueID);

#endif /*EXTERNALTWOPHASEMEDIUM_H_*/
