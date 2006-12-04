/* *****************************************************************
 * C/C++ layer for external medium models extending from 
 * PartialExternalTwoPhaseMedium.
 *
 * Francesco Casella, Christoph Richter, Sep 2006
 ********************************************************************/


#include "externaltwophasemedium_lib.h"

#include "mediummap.h"
#include "twophasemedium.h"

int createMedium_(const char *mediumName, const char *libraryName, 
				  const char *substanceName, int oldUniqueID){
	// Allocate a new object and return a unique ID if oldUniqueID == 0
	if (oldUniqueID == 0){
		return MediumMap::addMedium(mediumName, libraryName, substanceName);
	} else {
	// Do nothing if oldUniqueID > 0 (medium object already allocated)
		return oldUniqueID;
	}
}

double getMolarMass_(const char *mediumName, const char *libraryName,  
					 const char *substanceName){
	// Return molar mass
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->molarMass();
}

double getCriticalTemperature_(const char *mediumName, const char *libraryName,  
				         	   const char *substanceName){
	// Return critical temperature
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalTemperature();
}

double getCriticalPressure_(const char *mediumName, const char *libraryName,  
				            const char *substanceName){
	// Return critical pressure
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalPressure();
}

double getCriticalMolarVolume_(const char *mediumName, const char *libraryName,
							   const char *substanceName){
	// Return critical molar volume
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalMolarVolume();
}

void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_dT was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = 
        MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_dT_ with the transientUniqueID
	  setState_dT_(d, T, phase, transientUniqueID, state_uniqueID, state_phase,
		           mediumName, libraryName, substanceName);
	}
	else {
      // setState_dT was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_dT function
	  MediumMap::medium(uniqueID)->setState_dT(d, T, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = phase;
    }
}

void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_ph was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = 
        MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_ph_ with the transientUniqueID
	  setState_ph_(p, h, phase, transientUniqueID, state_uniqueID, state_phase,
		           mediumName, libraryName, substanceName);
	}
	else {
      // setState_ph was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_ph function
	  MediumMap::medium(uniqueID)->setState_ph(p, h, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = phase;
    }
}

void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_ps was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = 
        MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_ps_ with the transientUniqueID
	  setState_ps_(p, s, phase, transientUniqueID, state_uniqueID, state_phase,
		           mediumName, libraryName, substanceName);
	}
	else {
      // setState_ps was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_ps function
	  MediumMap::medium(uniqueID)->setState_ps(p, s, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = phase;
    }
}

void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_pT was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = 
        MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_pT_ with the transientUniqueID
	  setState_pT_(p, T, phase, transientUniqueID, state_uniqueID, state_phase,
		           mediumName, libraryName, substanceName);
	}
	else {
      // setState_pT was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_pT function
	  MediumMap::medium(uniqueID)->setState_pT(p, T);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = phase;
    }
}

void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName){
	// Pointer to the medium object
	BaseTwoPhaseMedium *medium;

	if (uniqueID == 0){
	  // setSat_p was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = 
        MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setSat_p with the transientUniqueID
	  setSat_p_(p, transientUniqueID, sat_psat, sat_Tsat, sat_uniqueID,
		        mediumName, libraryName, substanceName);
	}
	else {
      // setSat_p was called with the uniqueID of an existing medium object
 	  // get the pointer to the medium object
 	  medium = MediumMap::medium(uniqueID);
 	  // Call the medium object's setSat_p function
	  medium->setSat_p(p);
	  // Return values
	  if (sat_uniqueID != NULL)
  		  *sat_uniqueID = uniqueID;
	  if (sat_psat != NULL)
		  *sat_psat = medium->ps();
	  if (sat_Tsat != NULL)
		  *sat_Tsat = medium->Ts();
	}
}

void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName){
	// Pointer to the medium object
	BaseTwoPhaseMedium *medium;

	if (uniqueID == 0){
	  // setSat_T was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = 
        MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setSat_T with the transientUniqueID
	  setSat_T_(T, transientUniqueID, sat_psat, sat_Tsat, sat_uniqueID,
		        mediumName, libraryName, substanceName);
	}
	else {
      // setSat_T was called with the uniqueID of an existing medium object
 	  // get the pointer to the medium object
 	  medium = MediumMap::medium(uniqueID);
 	  // Call the medium object's setSat_T function
	  medium->setSat_T(T);
	  // Set the output values 
	  if (sat_uniqueID != NULL)
  		  *sat_uniqueID = uniqueID;
	  if (sat_psat != NULL)
		  *sat_psat = medium->ps();
	  if (sat_Tsat != NULL)
		  *sat_Tsat = medium->Ts();
	}
}

void setSat_p_state_(int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID){
	// Check for the validity of the uniqueID
	if (uniqueID == 0)
		errorMessage("setSat_p_state called without a valid uniqueID)");
	// Set a pointer to the medium
	BaseTwoPhaseMedium *medium;
	medium = MediumMap::medium(uniqueID);
	// Call the medium object setSat_p_state() function, which will
	// set the saturation properties corresponding to the medium pressure
	medium->setSat_p_state();
	// Set the output values 
	  if (sat_uniqueID != NULL)
  		  *sat_uniqueID = uniqueID;
	  if (sat_psat != NULL)
		  *sat_psat = medium->ps();
	  if (sat_Tsat != NULL)
		  *sat_Tsat = medium->Ts();
}

double density_(int uniqueID){
	return MediumMap::medium(uniqueID)->d();
}

double density_ph_der_(int uniqueID, double p_der, double h_der){
	return MediumMap::medium(uniqueID)->dd_dp_h()*p_der +
		   MediumMap::medium(uniqueID)->dd_dh_p()*h_der;
}

double pressure_(int uniqueID){
	return MediumMap::medium(uniqueID)->p();
}

double specificEnthalpy_(int uniqueID){
	return MediumMap::medium(uniqueID)->h();
}

double specificEntropy_(int uniqueID){
	return MediumMap::medium(uniqueID)->s();
}

double temperature_(int uniqueID){
	return MediumMap::medium(uniqueID)->T();
}

double saturationPressure_sat_(double psat, double Tsat, int uniqueID,
							   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->ps();
	else
		return MediumMap::medium(uniqueID)->ps();
}

double saturationTemperature_sat_(double psat, double Tsat, int uniqueID,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->Ts();
	else
		return MediumMap::medium(uniqueID)->Ts();
}

double bubbleDensity_(double psat, double Tsat, int uniqueID,
					  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->dl();
	else
		return MediumMap::medium(uniqueID)->dl();
}

double dewDensity_(double psat, double Tsat, int uniqueID,
				   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->dv();
	else
		return MediumMap::medium(uniqueID)->dv();
}

double bubbleEnthalpy_(double psat, double Tsat, int uniqueID,
					   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->hl();
	else
		return MediumMap::medium(uniqueID)->hl();
}

double dewEnthalpy_(double psat, double Tsat, int uniqueID,
					const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->hv();
	else
		return MediumMap::medium(uniqueID)->hv();
}

double bubbleEntropy_(double psat, double Tsat, int uniqueID,
					  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->sl();
	else
		return MediumMap::medium(uniqueID)->sl();
}

double dewEntropy_(double psat, double Tsat, int uniqueID,
				   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->sv();
	else
		return MediumMap::medium(uniqueID)->sv();
}

double dBubbleDensity_dPressure_(double psat, double Tsat, int uniqueID,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->d_dl_dp();
	else
		return MediumMap::medium(uniqueID)->d_dl_dp();
}

double dDewDensity_dPressure_(double psat, double Tsat, int uniqueID,
							  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->d_dv_dp();
	else
		return MediumMap::medium(uniqueID)->d_dv_dp();
}

double dBubbleEnthalpy_dPressure_(double psat, double Tsat, int uniqueID,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->d_hl_dp();
	else
		return MediumMap::medium(uniqueID)->d_hl_dp();
}

double dDewEnthalpy_dPressure_(double psat, double Tsat, int uniqueID,
							   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
		return MediumMap::solverMedium(mediumName, libraryName, substanceName)->d_hv_dp();
	else
		return MediumMap::medium(uniqueID)->d_hv_dp();
}

double isobaricExpansionCoefficient_(int uniqueID){
	return MediumMap::medium(uniqueID)->beta();
}

double isothermalCompressibility_(int uniqueID){
	return MediumMap::medium(uniqueID)->kappa();
}

double specificHeatCapacityCp_(int uniqueID){
	return MediumMap::medium(uniqueID)->cp();
}

double specificHeatCapacityCv_(int uniqueID){
	return MediumMap::medium(uniqueID)->cv();
}

double dynamicViscosity_(int uniqueID){
	return MediumMap::medium(uniqueID)->eta();
}

double thermalConductivity_(int uniqueID){
	return MediumMap::medium(uniqueID)->lambda();
}

double prandtlNumber_(int uniqueID){
	return MediumMap::medium(uniqueID)->Pr();
}

double surfaceTension_(double psat, double Tsat, int uniqueID){
	return MediumMap::medium(uniqueID)->sigma();
}

double dDensity_dPressure_h_(int uniqueID){
	return MediumMap::medium(uniqueID)->dd_dp_h();
}

double dDensity_dEnthalpy_p_(int uniqueID){
	return MediumMap::medium(uniqueID)->dd_dh_p();
}

double saturationPressure_(double T, const char *mediumName,
						   const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
	// Compute saturation pressure
	medium->setSat_T(T);
	// Return saturation pressure
	return medium->ps();
}

double saturationTemperature_(double p, const char *mediumName,
							  const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
	// Compute saturation pressure
	medium->setSat_p(p);
	return medium->Ts();
}

