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

void BaseSolver::clearTwoPhaseMediumProperties(TwoPhaseMediumProperties *const properties){
	properties->mediumName = "";
	properties->libraryName = "";
	properties->substanceName = "";
	properties->uniqueID = 0;

	properties->phase = 0;

	properties->dewUniqueIDOnePhase = 0;
	properties->dewUniqueIDTwoPhase = 0;
	properties->bubbleUniqueIDOnePhase = 0;
	properties->bubbleUniqueIDTwoPhase = 0;

	properties->beta = 0.0;
	properties->cp = 0.0;
	properties->cv = 0.0;
	properties->d = 0.0;
	properties->dd_dp_h = 0.0;
	properties->dd_dh_p = 0.0;
	properties->h = 0.0;
	properties->kappa = 0.0;
	properties->p = 0.0;
	properties->s = 0.0;
	properties->T = 0.0;

	properties->ps = 0.0;
	properties->Ts = 0.0;

	properties->dl = 0.0;
	properties->dv = 0.0;
	properties->hl = 0.0;
	properties->hv = 0.0;
	properties->sl = 0.0;
	properties->sv = 0.0;

	properties->eta = 0.0;
	properties->lambda = 0.0;
	properties->Pr = 0.0;
	properties->sigma = 0.0;

    properties->d_Ts_dp = 0.0;
	properties->d_dl_dp = 0.0;
	properties->d_dv_dp = 0.0;
    properties->d_hl_dp = 0.0;
	properties->d_hv_dp = 0.0;
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
	setState_ph(properties->p, properties->hl, phase, bubbleProperties);
}

// Default implementation of the setDewState function, relying on the correct 
// behaviour of setState_ph with respect to the state input. Can be overridden 
// in the specific solver code to get more efficient handling of this situation
void BaseSolver::setDewState(int phase, TwoPhaseMediumProperties *const properties,
		                                TwoPhaseMediumProperties *const dewProperties){
	// Set the dew state property record based on the original medium 
	// saturation state
	setState_ph(properties->p, properties->hv, phase, dewProperties);
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
