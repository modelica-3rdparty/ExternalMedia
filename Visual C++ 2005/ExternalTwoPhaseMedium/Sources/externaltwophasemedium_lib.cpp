/* *****************************************************************
 * C/C++ layer for external medium models extending from 
 * PartialExternalTwoPhaseMedium.
 *
 * Francesco Casella, Christoph Richter, Sep 2006
 ********************************************************************/


#include "externaltwophasemedium_lib.h"

#include "mediummap.h"
#include "twophasemedium.h"

// Static boolean flag to keep track if the library hasn't already been initialized
static bool firstCall(true);

// Function to initialize library
static void initializeLibrary(const string &mediumName, const string &libraryName, const string &substanceName){
	// Set firstCall flag to false
	firstCall = false;
}

int createMedium_(const char *mediumName, const char *libraryName, const char *substanceName, int oldUniqueID){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);
	// Allocate a new object and return a unique ID if oldUniqueID == 0
	if (oldUniqueID == 0){
		return MediumMap::addMedium(mediumName, libraryName, substanceName);
	} else {
	// Do nothing if oldUniqueID > 0 (medium object already allocated)
		return oldUniqueID;
	}
}

double molarMass_(int uniqueID){
	return MediumMap::medium(uniqueID)->MM;
}

double criticalDensity_(int uniqueID){
	return MediumMap::medium(uniqueID)->dc;
}

double criticalPressure_(int uniqueID){
	return MediumMap::medium(uniqueID)->pc;
}

double criticalTemperature_(int uniqueID){
	return MediumMap::medium(uniqueID)->Tc;
}

void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);

	MediumMap::medium(uniqueID)->setSat_p(p);

	if (sat_uniqueID != NULL)
		*sat_uniqueID = uniqueID;
	if (sat_psat != NULL)
		*sat_psat = p;
	if (sat_Tsat != NULL)
		*sat_Tsat = MediumMap::medium(uniqueID)->Ts;
}

void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);

	MediumMap::medium(uniqueID)->setSat_T(T);
	
	if (sat_uniqueID != NULL)
		*sat_uniqueID = uniqueID;
	if (sat_psat != NULL)
		*sat_Tsat = T;
	if (sat_Tsat != NULL)
		*sat_psat = MediumMap::medium(uniqueID)->ps;
}

double saturationPressure_(double T, const char *mediumName,
						   const char *libraryName, const char *substanceName){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);

	return MediumMap::mediums[0]->saturationPressure(T, mediumName);
}

double saturationTemperature_(double p, const char *mediumName,
							  const char *libraryName, const char *substanceName){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);

	return MediumMap::mediums[0]->saturationTemperature(p, mediumName);
}

double bubbleDensity_(int uniqueID){
	return MediumMap::medium(uniqueID)->dl;
}

double dewDensity_(int uniqueID){
	return MediumMap::medium(uniqueID)->dv;
}

double bubbleEnthalpy_(int uniqueID){
	return MediumMap::medium(uniqueID)->hl;
}

double dewEnthalpy_(int uniqueID){
	return MediumMap::medium(uniqueID)->hv;
}

double bubbleEntropy_(int uniqueID){
	return MediumMap::medium(uniqueID)->sl;
}

double dewEntropy_(int uniqueID){
	return MediumMap::medium(uniqueID)->sv;
}

void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);

	MediumMap::medium(uniqueID)->setState_dT(d, T, phase);

	if (state_uniqueID != NULL)
		*state_uniqueID = uniqueID;
	if (state_phase != NULL)
		*state_phase = phase;
}

void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);

	MediumMap::medium(uniqueID)->setState_ph(p, h, phase);

	if (state_uniqueID != NULL)
		*state_uniqueID = uniqueID;
	if (state_phase != NULL)
		*state_phase = phase;
}

void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);

	MediumMap::medium(uniqueID)->setState_ps(p, s, phase);

	if (state_uniqueID != NULL)
		*state_uniqueID = uniqueID;
	if (state_phase != NULL)
		*state_phase = phase;
}

void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary(mediumName, libraryName, substanceName);

	MediumMap::medium(uniqueID)->setState_pT(p, T);

	if (state_uniqueID != NULL)
		*state_uniqueID = uniqueID;
	if (state_phase != NULL)
		*state_phase = phase;
}

double density_(int uniqueID){
	return MediumMap::medium(uniqueID)->d;
}

double density_ph_der_(int uniqueID, double p_der, double h_der){
	return MediumMap::medium(uniqueID)->dd_dp_h*p_der +
		   MediumMap::medium(uniqueID)->dd_dh_p*h_der;
}

double pressure_(int uniqueID){
	return MediumMap::medium(uniqueID)->p;
}

double specificEnthalpy_(int uniqueID){
	return MediumMap::medium(uniqueID)->h;
}

double specificEntropy_(int uniqueID){
	return MediumMap::medium(uniqueID)->s;
}

double temperature_(int uniqueID){
	return MediumMap::medium(uniqueID)->T;
}

double isobaricExpansionCoefficient_(int uniqueID){
	return MediumMap::medium(uniqueID)->beta;
}

double isothermalCompressibility_(int uniqueID){
	return MediumMap::medium(uniqueID)->kappa;
}

double specificHeatCapacityCp_(int uniqueID){
	return MediumMap::medium(uniqueID)->cp;
}

double specificHeatCapacityCv_(int uniqueID){
	return MediumMap::medium(uniqueID)->cv;
}

double dynamicViscosity_(int uniqueID){
	return MediumMap::medium(uniqueID)->eta;
}

double thermalConductivity_(int uniqueID){
	return MediumMap::medium(uniqueID)->lambda;
}

double prandtlNumber_(int uniqueID){
	return MediumMap::medium(uniqueID)->Pr;
}

double surfaceTension_(double psat, double Tsat, int uniqueID){
	return MediumMap::medium(uniqueID)->sigma;
}
