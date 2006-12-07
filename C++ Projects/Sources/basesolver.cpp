/* *****************************************************************
 * Implementation of class BaseSolver
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "basesolver.h"

#include <math.h>
#include "twophasemediumproperties.h"
#include "mediummap.h"

BaseSolver::BaseSolver(const string &mediumName, const string &libraryName, const string &substanceName)
	: mediumName(mediumName), libraryName(libraryName), substanceName(substanceName){
}

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

double BaseSolver::criticalDensity() const{
	return _fluidConstants.dc;
}

double BaseSolver::criticalMolarVolume() const{
	return _fluidConstants.MM/_fluidConstants.dc;
}

double BaseSolver::criticalEnthalpy() const{
	return _fluidConstants.hc;
}

double BaseSolver::criticalEntropy() const{
	return _fluidConstants.sc;
}

void BaseSolver::setFluidConstants(){
}

void BaseSolver::setSat_p(double &p, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setSat_T(double &T, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setSat_p_state(TwoPhaseMediumProperties *const properties){
};

void BaseSolver::setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties){
}

// Default implementation of the setBubbleState function, relying on the correct 
// behaviour of setState_ph with respect to the state input. Can be overridden 
// in the specific solver code to get more efficient handling of this situation
void BaseSolver::setBubbleState(int phase, TwoPhaseMediumProperties *const properties,
		                                   TwoPhaseMediumProperties *const bubbleProperties){
	// Set the bubble state property record based on the original medium 
	// saturation state
	printf("BaseSolver::setBubbleState called\n"); // XXX
	setState_ph(properties->ps, properties->hl, phase, bubbleProperties);
	printf("BaseSolver::setBubbleState finished\n"); // XXX
}

// Default implementation of the setDewState function, relying on the correct 
// behaviour of setState_ph with respect to the state input. Can be overridden 
// in the specific solver code to get more efficient handling of this situation
void BaseSolver::setDewState(int phase, TwoPhaseMediumProperties *const properties,
		                                TwoPhaseMediumProperties *const dewProperties){
	// Set the dew state property record based on the original medium 
	// saturation state
	setState_ph(properties->ps, properties->hv, phase, dewProperties);
}

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
