#include "basesolver.h"
#include <math.h>
#include "twophasemediumproperties.h"
#include "mediummap.h"

//! Constructor.
/*!
  The constructor is copying the medium name, library name and substance name
  to the locally defined variables.
  @param mediumName Arbitrary medium name
  @param libraryName Name of the external fluid property library
  @param substanceName Substance name
*/
BaseSolver::BaseSolver(const string &mediumName, const string &libraryName, const string &substanceName)
	: mediumName(mediumName), libraryName(libraryName), substanceName(substanceName){
}

//! Destructor
/*!
  The destructor for the base solver if currently not doing anything.
*/
BaseSolver::~BaseSolver(){
}

//! Return molar mass (Default implementation provided)
double BaseSolver::molarMass() const{
	return _fluidConstants.MM;
}

//! Return temperature at critical point (Default implementation provided)
double BaseSolver::criticalTemperature() const{
	return _fluidConstants.Tc;
}

//! Return pressure at critical point (Default implementation provided)
double BaseSolver::criticalPressure() const{
	return _fluidConstants.pc;
}

//! Return density at critical point (Default implementation provided)
double BaseSolver::criticalDensity() const{
	return _fluidConstants.dc;
}

//! Return molar volume at critical point (Default implementation provided)
double BaseSolver::criticalMolarVolume() const{
	return _fluidConstants.MM/_fluidConstants.dc;
}

//! Return specific enthalpy at critical point (Default implementation provided)
double BaseSolver::criticalEnthalpy() const{
	return _fluidConstants.hc;
}

//! Return specific entropy at critical point (Default implementation provided)
double BaseSolver::criticalEntropy() const{
	return _fluidConstants.sc;
}

//! Set fluid constants
/*!
  This function sets the fluid constants which are defined in the
  FluidConstants record in Modelica. It should be called when a new
  solver is created.
  
  Must be re-implemented in the specific solver
*/
void BaseSolver::setFluidConstants(){
}

//! Set saturation properties from p
/*!
  This function sets the saturation properties for the given pressure p.
  The computed values are written to the two phase medium propery struct.

  Must be re-implemented in the specific solver
  @param p Pressure
  @param properties Two phase medium property record
*/
void BaseSolver::setSat_p(double &p, TwoPhaseMediumProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage("Internal error: setSat_p() not implemented in the Solver object");
}

//! Set saturation properties from T
/*!
  This function sets the saturation properties for the given temperature T.
  The computed values are written to the two phase medium propery struct.

  Must be re-implemented in the specific solver
  @param T Temperature
  @param properties Two phase medium property record
*/
void BaseSolver::setSat_T(double &T, TwoPhaseMediumProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage("Internal error: setSat_T() not implemented in the Solver object");
}

//! Set saturation properties from within BaseProperties model
/*!
  This function sets the saturation properties for the given pressure p
  and is desined to be used from within the BaseProperties model in
  Modelica. The computed values are written to the two phase medium propery 
  struct.

  Must be re-implemented in the specific solver
  @param properties Two phase medium property record
*/
void BaseSolver::setSat_p_state(TwoPhaseMediumProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage("Internal error: setSat_p_state() not implemented in the Solver object");
};

//! Set state from d, T, and phase
/*!
  This function sets the thermodynamic state record for the given density
  d, the temperature T and the specified phase. The computed values are
  written to the two phase medium property struct.

  Must be re-implemented in the specific solver
  @param d Density
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param properties Two phase medium property record
*/
void BaseSolver::setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage("Internal error: setState_dT() not implemented in the Solver object");
}

//! Set state from p, h, and phase
/*!
  This function sets the thermodynamic state record for the given pressure
  p, the specific enthalpy h and the specified phase. The computed values are
  written to the two phase medium property struct.

  Must be re-implemented in the specific solver
  @param p Pressure
  @param h Specific enthalpy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param properties Two phase medium property record
*/
void BaseSolver::setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage("Internal error: setState_ph() not implemented in the Solver object");
}

//! Set state from p, s, and phase
/*!
  This function sets the thermodynamic state record for the given pressure
  p, the specific entropy s and the specified phase. The computed values are
  written to the two phase medium property struct.

  Must be re-implemented in the specific solver
  @param p Pressure
  @param s Specific entropy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param properties Two phase medium property record
*/
void BaseSolver::setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage("Internal error: setState_ps() not implemented in the Solver object");
}

//! Set state from p and T
/*!
  This function sets the thermodynamic state record for the given pressure
  p and the temperature T. The computed values are
  written to the two phase medium property struct.

  Must be re-implemented in the specific solver
  @param p Pressure
  @param T Temperature
  @param properties Two phase medium property record
*/
void BaseSolver::setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage("Internal error: setState_pT() not implemented in the Solver object");
}

//! Set bubble state
/*!
  This function sets the bubble state record bubbleProperties corresponding to the 
  saturation data contained in the properties record.

  The default implementation of the setBubbleState function is relying on the correct 
  behaviour of setState_ph with respect to the state input. Can be overridden 
  in the specific solver code to get more efficient or correct handling of this situation.
  @param phase Phase (1: one-phase, 2: two-phase)
  @param properties Two phase medium property record with saturation properties data
  @param bubbleProperties Two phase medium property record where to write the bubble point properties
*/
void BaseSolver::setBubbleState(int phase, TwoPhaseMediumProperties *const properties,
		                                   TwoPhaseMediumProperties *const bubbleProperties){
	// Set the bubble state property record based on the original medium saturation state
	setState_ph(properties->ps, properties->hl, phase, bubbleProperties);
}

//! Set dew state
/*!
  This function sets the dew state record dewProperties corresponding to the 
  saturation data contained in the properties record.

  The default implementation of the setDewState function is relying on the correct 
  behaviour of setState_ph with respect to the state input. Can be overridden 
  in the specific solver code to get more efficient or correct handling of this situation.
  @param phase Phase (1: one-phase, 2: two-phase)
  @param properties Two phase medium property record with saturation properties data
  @param dewProperties Two phase medium property record where to write the dew point properties
*/
void BaseSolver::setDewState(int phase, TwoPhaseMediumProperties *const properties,
		                                TwoPhaseMediumProperties *const dewProperties){
	// Set the dew state property record based on the original medium saturation state
	setState_ph(properties->ps, properties->hv, phase, dewProperties);
}

//! Compute derivatives
/*!
  This function computes the derivatives according to the Bridgman's table.
  The computed values are written to the two phase medium property struct.
  This function can be called from within the setState_XX routines 
  when implementing a new solver. Please be aware that cp, beta and
  kappa have to be provided to allow the computation of the derivatives. It
  returns false if the computation failed.

  Default implementation provided.
  @param properties Two phase medium property record
*/
bool BaseSolver::computeDerivatives(TwoPhaseMediumProperties *const properties){
	// Check whether cp is equal to zero
	if (properties->cp == 0.0)
		return false;
	// Check whether density is equal to zero
	if (properties->d == 0.0)
		return false;
	// Compute dd_dp_h
	properties->dd_dp_h = -(properties->T*properties->beta*properties->beta -
						  properties->beta -
						  properties->kappa*properties->d*properties->cp)/
						  properties->cp;
	// Compute dd_dh_p
	properties->dd_dh_p = -properties->beta*properties->d/properties->cp;
	return true;
}

//! Compute isentropic enthalpy
/*!
  This function returns the enthalpy at pressure p after an isentropic
  transformation from the state specified by the properties input

  Must be re-implemented in the specific solver
  @param p New pressure
  @param properties Two phase medium property record corresponding to current state
*/
double BaseSolver::isentropicEnthalpy(double &p, TwoPhaseMediumProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage("Internal error: isentropicEnthalpy() not implemented in the Solver object");
	return 0;
}
