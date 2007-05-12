/*!
  \file externaltwophasemedium.cpp
  \brief Interface layer

  C/C++ layer for external medium models extending from 
  PartialExternalTwoPhaseMedium.

  Francesco Casella, Christoph Richter, Sep 2006
*/

#include "externaltwophasemedium.h"
#include "mediummap.h"
#include "twophasemedium.h"

//! Create medium
/*!
  This function creates a new medium with the specified medium name, library name,
  and substance name. The old unique ID is required to check whether a medium
  has already been created.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
  @param oldUniqueID Old unique ID number
*/
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

//! Get molar mass
/*!
  This function returns the molar mass of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double getMolarMass_(const char *mediumName, const char *libraryName,  
					 const char *substanceName){
	// Return molar mass
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->molarMass();
}

//! Get critical temperature
/*!
  This function returns the critical temperature of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double getCriticalTemperature_(const char *mediumName, const char *libraryName,  
				         	   const char *substanceName){
	// Return critical temperature
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalTemperature();
}

//! Get critical pressure
/*!
  This function returns the critical pressure of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double getCriticalPressure_(const char *mediumName, const char *libraryName,  
				            const char *substanceName){
	// Return critical pressure
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalPressure();
}

//! Get critical molar volume
/*!
  This function returns the critical molar volume of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double getCriticalMolarVolume_(const char *mediumName, const char *libraryName,
							   const char *substanceName){
	// Return critical molar volume
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalMolarVolume();
}

//! Compute properties from d, T, and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param d Density
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void setState_dT_(double d, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_dT was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_dT_ with the transientUniqueID
	  setState_dT_(d, T, phase, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, mediumName, libraryName, substanceName);
	} else {
      // setState_dT was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_dT function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_dT(d, T, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
    }
}

//! Compute properties from p, h, and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param p Pressure
  @param h Specific enthalpy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void setState_ph_(double p, double h, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_ph was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_ph_ with the transientUniqueID
	  setState_ph_(p, h, phase, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, mediumName, libraryName, substanceName);
	} else {
      // setState_ph was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_ph function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_ph(p, h, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
    }
}

//! Compute properties from p, s, and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param p Pressure
  @param s Specific entropy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void setState_ps_(double p, double s, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_ps was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_ps_ with the transientUniqueID
	  setState_ps_(p, s, phase, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, mediumName, libraryName, substanceName);
	} else {
      // setState_ps was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_ps function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_ps(p, s, phase);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = MediumMap::medium(uniqueID)->phase();
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
    }
}

//! Compute properties from p, T, and phase
/*!
  This function computes the properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.

  Attention: The phase input is ignored for this function!
  @param p Pressure
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param uniqueID Unique ID number
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param state_d Pointer to return density for state record
  @param state_h Pointer to return specific enthalpy for state record
  @param state_p Pointer to return pressure for state record
  @param state_s Pointer to return specific entropy for state record
  @param state_T Pointer to return temperature for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void setState_pT_(double p, double T, int phase, int uniqueID, int *state_uniqueID, int *state_phase, double *state_d, double *state_h, double *state_p, double *state_s, double *state_T,
				  const char *mediumName, const char *libraryName, const char *substanceName){
	if (uniqueID == 0){
	  // setState_pT was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setState_pT_ with the transientUniqueID
	  setState_pT_(p, T, phase, transientUniqueID, state_uniqueID, state_phase, state_d, state_h, state_p, state_s, state_T, mediumName, libraryName, substanceName);
	} else {
      // setState_pT was called with the uniqueID of an existing medium object
 	  // Call the medium object's setState_pT function
	  BaseTwoPhaseMedium *medium = MediumMap::medium(uniqueID);
	  medium->setState_pT(p, T);
	  // Return values
	  if (state_uniqueID != NULL)
  		  *state_uniqueID = uniqueID;
	  if (state_phase != NULL)
	 	  *state_phase = 1;
	  if (state_d != NULL)
		  *state_d = medium->d();
	  if (state_h != NULL)
		  *state_h = medium->h();
	  if (state_p != NULL)
		  *state_p = medium->p();
	  if (state_s != NULL)
		  *state_s = medium->s();
	  if (state_T != NULL)
		  *state_T = medium->T();
    }
}

//! Compute saturation properties from p
/*!
  This function computes the saturation properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param p Pressure
  @param uniqueID Unique ID number
  @param sat_psat Pointer to return pressure for saturation record
  @param sat_Tsat Pointer to return temperature for saturation record
  @param sat_uniqueID Pointer to return unique ID number for saturation record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void setSat_p_(double p, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName){
	// Pointer to the medium object
	BaseTwoPhaseMedium *medium;
	if (uniqueID == 0){
	  // setSat_p was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setSat_p with the transientUniqueID
	  setSat_p_(p, transientUniqueID, sat_psat, sat_Tsat, sat_uniqueID, mediumName, libraryName, substanceName);
	} else {
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

//! Compute saturation properties from T
/*!
  This function computes the saturation properties for the specified inputs. If the function
  is called with uniqueID == 0 a new transient unique ID is assigned and the function
  is called again with this new transient unique ID number.
  @param T Temperature
  @param uniqueID Unique ID number
  @param sat_psat Pointer to return pressure for saturation record
  @param sat_Tsat Pointer to return temperature for saturation record
  @param sat_uniqueID Pointer to return unique ID number for saturation record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void setSat_T_(double T, int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID,
			   const char *mediumName, const char *libraryName, const char *substanceName){
	// Pointer to the medium object
	BaseTwoPhaseMedium *medium;
	if (uniqueID == 0){
	  // setSat_T was called with uniqueID == 0
	  // create a new transient medium object and get a transientUniqueID
	  int transientUniqueID = MediumMap::addTransientMedium(mediumName, libraryName, substanceName);
      // run setSat_T with the transientUniqueID
	  setSat_T_(T, transientUniqueID, sat_psat, sat_Tsat, sat_uniqueID, mediumName, libraryName, substanceName);
	} else {
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

//! Compute saturation properties from within BaseProperties
/*!
  This function computes the saturation properties and is designed to be called
  from within the BaseProperties model. The saturation properties are set according
  to the medium pressure
  @param uniqueID Unique ID number
  @param sat_psat Pointer to return pressure for saturation record
  @param sat_Tsat Pointer to return temperature for saturation record
  @param sat_uniqueID Pointer to return unique ID number for saturation record
*/
void setSat_p_state_(int uniqueID, double *sat_psat, double *sat_Tsat, int *sat_uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("setSat_p_state called without a valid uniqueID");
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

//! Compute dew state
/*!
  This function computes the dew state for the specified medium.
  @param uniqueID Unique ID number
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void setDewState_(int uniqueID, int phase, int *state_uniqueID, int *state_phase,
  				  const char *mediumName, const char *libraryName, const char *substanceName){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID or phase inputs
	if (uniqueID == 0)
		errorMessage("setDewState_ called without a valid uniqueID");
	if (phase < 1 || phase > 2)
		errorMessage("setDewState_ called with invalid phase");
	// Get the unique ID of the the dewState object, and allocate a new medium
	// object and set the dewState uniqueID if necessary
	int dewUniqueID = MediumMap::medium(uniqueID)->getDewUniqueID(phase);
	// Call the original medium object's setDewState function
	// which will compute the properties of the dew state and store them
	// in the medium with the dew state unique ID
    MediumMap::medium(uniqueID)->setDewState(phase);
    // Return values
	if (state_uniqueID != NULL)
  	  *state_uniqueID = dewUniqueID;
	if (state_phase != NULL)
	  *state_phase = phase;
}

//! Compute bubble state
/*!
  This function computes the  bubble state for the specified medium.
  @param uniqueID Unique ID number
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state_uniqueID Pointer to return unique ID number for state record
  @param state_phase Pointer to return phase for state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void setBubbleState_(int uniqueID, int phase, int *state_uniqueID, int *state_phase,
  				     const char *mediumName, const char *libraryName, const char *substanceName){
	// Check for the validity of the inputs - this function should never be 
	// called with a zero unique ID or phase inputs
	if (uniqueID == 0)
		errorMessage("setBubbleState_ called without a valid uniqueID");
	if (phase < 1 || phase > 2)
		errorMessage("setBubbleState_ called with invalid phase");
	// Get the unique ID of the the dewState object, and allocate a new medium
	// object and set the dewState uniqueID if necessary
	int bubbleUniqueID = MediumMap::medium(uniqueID)->getBubbleUniqueID(phase);
	// Call the original medium object's setBubbleState function
	// which will compute the properties of the dew state and store them
	// in the medium with the dew state unique ID
    MediumMap::medium(uniqueID)->setBubbleState(phase);
    // Return values
	if (state_uniqueID != NULL)
  	  *state_uniqueID = bubbleUniqueID;
	if (state_phase != NULL)
	  *state_phase = phase;
}

//! Return density of specified medium
double density_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function density_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->d();
}

//! Return derivative of density wrt pressure at constant specific enthalpy of specified medium
double density_derp_h_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function density_derp_h_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->dd_dp_h();
}

//! Return derivative of density wrt specific enthalpy at constant pressure of specified medium
double density_derh_p_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function density_derh_p_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->dd_dh_p();
}

//! Return derivative of density wrt pressure and specific enthalpy of specified medium
double density_ph_der_(int uniqueID, double p_der, double h_der){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function density_ph_der_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->dd_dp_h()*p_der +
		   MediumMap::medium(uniqueID)->dd_dh_p()*h_der;
}

//! Return pressure of specified medium
double pressure_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function pressure_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->p();
}

//! Return specific enthalpy of specified medium
double specificEnthalpy_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function specificEnthalpy_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->h();
}

//! Return specific entropy of specified medium
double specificEntropy_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function specificEntropy_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->s();
}

//! Return temperature of specified medium
double temperature_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function temperature_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->T();
}

//! Return derivative of temperature wrt pressure and specific enthalpy of specified medium
double temperature_ph_der_(int uniqueID, double p_der, double h_der){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function temperature_ph_der_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->dT_dp_h()*p_der +
		   MediumMap::medium(uniqueID)->dT_dh_p()*h_der;
}

//! Return the enthalpy at pressure p after an isentropic transformation form the specified medium state
double isentropicEnthalpy_(double p, int uniqueID)
{
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function isentropicEnthalpy_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->h_iso(p);
}

//! Return derivative of saturation temperature of specified medium from saturation properties
double saturationTemperature_derp_sat_(double psat, double Tsat, int uniqueID,
								       const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_Ts_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_Ts_dp();
}

//! Return bubble density of specified medium from saturation properties
double bubbleDensity_(double psat, double Tsat, int uniqueID,
					  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->dl();
	}
	else
		return MediumMap::medium(uniqueID)->dl();
}

//! Return dew density of specified medium from saturation properties
double dewDensity_(double psat, double Tsat, int uniqueID,
				   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->dv();
	}
	else
		return MediumMap::medium(uniqueID)->dv();
}

//! Return bubble specific enthalpy of specified medium from saturation properties
double bubbleEnthalpy_(double psat, double Tsat, int uniqueID,
					   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->hl();
	}
	else
		return MediumMap::medium(uniqueID)->hl();
}

//! Return dew specific enthalpy of specified medium from saturation properties
double dewEnthalpy_(double psat, double Tsat, int uniqueID,
					const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->hv();
	}
	else
		return MediumMap::medium(uniqueID)->hv();
}

//! Return bubble specific entropy of specified medium from saturation properties
double bubbleEntropy_(double psat, double Tsat, int uniqueID,
					  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->sl();
	}
	else
		return MediumMap::medium(uniqueID)->sl();
}

//! Return dew specific entropy of specified medium from saturation properties
double dewEntropy_(double psat, double Tsat, int uniqueID,
				   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->sv();
	}
	else
		return MediumMap::medium(uniqueID)->sv();
}

//! Return derivative of bubble density wrt pressure of specified medium from saturation properties
double dBubbleDensity_dPressure_(double psat, double Tsat, int uniqueID,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_dl_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_dl_dp();
}

//! Return derivative of dew density wrt pressure of specified medium from saturation properties
double dDewDensity_dPressure_(double psat, double Tsat, int uniqueID,
							  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_dv_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_dv_dp();
}

//! Return derivative of bubble specific enthalpy wrt pressure of specified medium from saturation properties
double dBubbleEnthalpy_dPressure_(double psat, double Tsat, int uniqueID,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_hl_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_hl_dp();
}

//! Return derivative of dew specific enthalpy wrt pressure of specified medium from saturation properties
double dDewEnthalpy_dPressure_(double psat, double Tsat, int uniqueID,
							   const char *mediumName, const char *libraryName, const char *substanceName){
	// Use solver medium object if no unique ID is supplied
	if (uniqueID == 0)
	{
		BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
		medium->setSat_p(psat);
		return medium->d_hv_dp();
	}
	else
		return MediumMap::medium(uniqueID)->d_hv_dp();
}

//! Return isobaric expansion coefficient of specified medium
double isobaricExpansionCoefficient_(int uniqueID){
	return MediumMap::medium(uniqueID)->beta();
}

//! Return isothermal compressibility of specified medium
double isothermalCompressibility_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function isothermalCompressibility_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->kappa();
}

//! Return specific heat capacity cp of specified medium
double specificHeatCapacityCp_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function specificHeatCapacityCp_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->cp();
}

//! Return specific heat capacity cv of specified medium
double specificHeatCapacityCv_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function specificHeatCapacityCv_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->cv();
}

//! Return dynamic viscosity of specified medium
double dynamicViscosity_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function dynamicViscosity_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->eta();
}

//! Return thermal conductivity of specified medium
double thermalConductivity_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function thermalConductivity_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->lambda();
}

//! Return Prandtl number of specified medium
double prandtlNumber_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function prandtlNumber_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->Pr();
}

//! Return surface tension of specified medium
double surfaceTension_(double psat, double Tsat, int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function surfaceTension_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->sigma();
}

//! Return derivative of density wrt pressure at constant specific enthalpy of specified medium
double dDensity_dPressure_h_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function dDensity_dPressure_h_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->dd_dp_h();
}

//! Return derivative of density wrt specific enthalpy at constant pressure of specified medium
double dDensity_dEnthalpy_p_(int uniqueID){
	// Check for the validity of the uniqueID - this function should never be 
	// called with a zero unique ID
	if (uniqueID == 0)
		errorMessage("Function dDensity_dEnthalpy_p_ called without a valid uniqueID");
	return MediumMap::medium(uniqueID)->dd_dh_p();
}

//! Compute saturation pressure for specified medium and temperature
double saturationPressure_(double T, const char *mediumName,
						   const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
	// Compute saturation pressure
	medium->setSat_T(T);
	// Return saturation pressure
	return medium->ps();
}

//! Compute saturation temperature for specified medium and pressure
double saturationTemperature_(double p, const char *mediumName,
							  const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
	// Compute saturation pressure
	medium->setSat_p(p);
	return medium->Ts();
}

//! Compute derivative of saturation temperature for specified medium and pressure
double saturationTemperature_derp_(double p, const char *mediumName,
								   const char *libraryName, const char *substanceName){
	// Get medium object
	BaseTwoPhaseMedium *medium = MediumMap::solverMedium(mediumName, libraryName, substanceName);
	// Compute saturation pressure
	medium->setSat_p(p);
	return medium->d_Ts_dp();
}
