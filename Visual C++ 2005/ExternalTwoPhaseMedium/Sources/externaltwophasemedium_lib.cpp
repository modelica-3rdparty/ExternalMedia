/* *****************************************************************
 * C/C++ layer for external medium models extending from 
 * PartialExternalTwoPhaseMedium.
 *
 * This file needs to be customized, see the next 10 lines
 *
 * Francesco Casella, Christoph Richter, Sep 2006
 ********************************************************************/

// Replace "dummytwophasemedium.h" with the .h file of your own
// TwoPhaseMedium-derived class
#include "dummytwophasemedium.h"

// Replace DummyTwoPhaseMedium with the name of your own
// TwoPhaseMedium-derived class
#define SpecificTwoPhaseMedium DummyTwoPhaseMedium  

/**********************************************************
 * No more modifications required from this point onwards!*
 **********************************************************/

#include "externaltwophasemedium_lib.h"
#include "mediummap.h"

// Static integer for the unique ID number
static int uniqueID(0);

// Static boolean to decide whether library has already been initialized
static bool firstCall(true);

// Function to initialize library
static void initializeLibrary(){
	// Create medium at MediumMap::mediums[0]
	MediumMap::mediums[0] = new SpecificTwoPhaseMedium("Empty");  // see #define on top
	// Set first call to be false
	firstCall = false;
}

int createMedium_(const char *mediumName, int oldUniqueID){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary();
	// Return only a unique ID if oldUniqueID == 0
	if (oldUniqueID == 0){
		++uniqueID;
		MediumMap::mediums[uniqueID] = new SpecificTwoPhaseMedium(mediumName);  // see #define on top
		return uniqueID;
	} else {
		return oldUniqueID;
	}
}

void deleteMedium_(int uniqueID){
	delete MediumMap::mediums[uniqueID]; MediumMap::mediums[uniqueID] = 0;
}

double molarMass_(int uniqueID){
	return MediumMap::mediums[uniqueID]->MM();
}

double criticalDensity_(int uniqueID){
	return MediumMap::mediums[uniqueID]->dc();
}

double criticalPressure_(int uniqueID){
	return MediumMap::mediums[uniqueID]->pc();
}

double criticalTemperature_(int uniqueID){
	return MediumMap::mediums[uniqueID]->Tc();
}

void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary();

	MediumMap::mediums[uniqueID]->setSat_p(p);

	if (sat_uniqueID != NULL)
		*sat_uniqueID = uniqueID;
	if (sat_psat != NULL)
		*sat_psat = p;
	if (sat_Tsat != NULL)
		*sat_Tsat = MediumMap::mediums[uniqueID]->Ts();
}

void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary();

	MediumMap::mediums[uniqueID]->setSat_T(T);
	
	if (sat_uniqueID != NULL)
		*sat_uniqueID = uniqueID;
	if (sat_psat != NULL)
		*sat_Tsat = T;
	if (sat_Tsat != NULL)
		*sat_psat = MediumMap::mediums[uniqueID]->ps();
}

double saturationPressure_(double T, const char *mediumName){
	return MediumMap::mediums[uniqueID]->saturationPressure(T, mediumName);
}

double saturationTemperature_(double p, const char *mediumName){
	return MediumMap::mediums[uniqueID]->saturationTemperature(p, mediumName);
}

double bubbleDensity_(int uniqueID){
	return MediumMap::mediums[uniqueID]->dl();
}

double dewDensity_(int uniqueID){
	return MediumMap::mediums[uniqueID]->dv();
}

double bubbleEnthalpy_(int uniqueID){
	return MediumMap::mediums[uniqueID]->hl();
}

double dewEnthalpy_(int uniqueID){
	return MediumMap::mediums[uniqueID]->hv();
}

double bubbleEntropy_(int uniqueID){
	return MediumMap::mediums[uniqueID]->sl();
}

double dewEntropy_(int uniqueID){
	return MediumMap::mediums[uniqueID]->sv();
}

void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary();

	MediumMap::mediums[uniqueID]->setState_dT(d, T, phase);

	if (state_uniqueID != NULL)
		*state_uniqueID = uniqueID;
	if (state_phase != NULL)
		*state_phase = phase;
}

void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary();

	MediumMap::mediums[uniqueID]->setState_ph(p, h, phase);

	if (state_uniqueID != NULL)
		*state_uniqueID = uniqueID;
	if (state_phase != NULL)
		*state_phase = phase;
}

void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary();

	MediumMap::mediums[uniqueID]->setState_ps(p, s, phase);

	if (state_uniqueID != NULL)
		*state_uniqueID = uniqueID;
	if (state_phase != NULL)
		*state_phase = phase;
}

void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase){
	// Check whether this is the first call to the library
	if (firstCall)
		initializeLibrary();

	MediumMap::mediums[uniqueID]->setState_pT(p, T);

	if (state_uniqueID != NULL)
		*state_uniqueID = uniqueID;
	if (state_phase != NULL)
		*state_phase = phase;
}

double density_(int uniqueID){
	return MediumMap::mediums[uniqueID]->d();
}

double pressure_(int uniqueID){
	return MediumMap::mediums[uniqueID]->p();
}

double specificEnthalpy_(int uniqueID){
	return MediumMap::mediums[uniqueID]->h();
}

double specificEntropy_(int uniqueID){
	return MediumMap::mediums[uniqueID]->s();
}

double temperature_(int uniqueID){
	return MediumMap::mediums[uniqueID]->T();
}

double isobaricExpansionCoefficient_(int uniqueID){
	return MediumMap::mediums[uniqueID]->beta();
}

double isothermalCompressibility_(int uniqueID){
	return MediumMap::mediums[uniqueID]->kappa();
}

double specificHeatCapacityCp_(int uniqueID){
	return MediumMap::mediums[uniqueID]->cp();
}

double specificHeatCapacityCv_(int uniqueID){
	return MediumMap::mediums[uniqueID]->cv();
}

double dynamicViscosity_(int uniqueID){
	return MediumMap::mediums[uniqueID]->eta();
}

double thermalConductivity_(int uniqueID){
	return MediumMap::mediums[uniqueID]->lambda();
}

double prandtlNumber_(int uniqueID){
	return MediumMap::mediums[uniqueID]->Pr();
}

double surfaceTension_(double psat, double Tsat, int uniqueID){
	return MediumMap::mediums[uniqueID]->sigma();
}
