#include "basesolver.h"
#include <math.h>
#include "externalmedialib.h"

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


double BaseSolver::molarMass() const{
	return _fluidConstants.MM;
}

double BaseSolver::criticalTemperature() const{
	return _fluidConstants.Tc;
}


double BaseSolver::criticalPressure() const{
	return _fluidConstants.pc;
}


double BaseSolver::criticalMolarVolume() const{
	return _fluidConstants.MM/_fluidConstants.dc;
}

double BaseSolver::criticalDensity() const{
	return _fluidConstants.dc;
}

double BaseSolver::criticalEnthalpy() const{
	return _fluidConstants.hc;
}

double BaseSolver::criticalEntropy() const{
	return _fluidConstants.sc;
}




void BaseSolver::setBubbleState(ExternalSaturationProperties *const properties, int phase,
		                        ExternalThermodynamicState *const bubbleProperties){

  // The default implementation of the setBubbleState function is relying on the
  // correct behaviour of setState_ph with respect to the state input.Can be
  // overridden in the specific solver code to get more efficient or correct
  // handling of this situation.

  // Set the bubble state property record based on the saturation properties
  // record

  setState_ph(properties->psat, properties->hl, phase, bubbleProperties);
}

void BaseSolver::setDewState(ExternalSaturationProperties *const properties, int phase,
		                     ExternalThermodynamicState *const dewProperties){

  // The default implementation of the setDewState function is relying on the
  // correct behaviour of setState_ph with respect to the state input.Can be
  // overridden in the specific solver code to get more efficient or correct
  // handling of this situation.

      // Set the dew state property record based on the saturation properties
      // record
      setState_ph(properties->psat, properties->hv, phase, dewProperties);
}


bool BaseSolver::computeDerivatives(ExternalThermodynamicState *const properties){
	// Check whether cp is equal to zero
	if (properties->cp == 0.0)
		return false;
	// Check whether density is equal to zero
	if (properties->d == 0.0)
		return false;
	// Compute ddph
	properties->ddph = -(properties->T*properties->beta*properties->beta -
						 properties->beta -
						 properties->kappa*properties->d*properties->cp)/
						 properties->cp;
	// Compute ddhp
	properties->ddhp = -properties->beta*properties->d/properties->cp;
	return true;
}
