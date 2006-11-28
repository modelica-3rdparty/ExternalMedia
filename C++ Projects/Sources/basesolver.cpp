/* *****************************************************************
 * Implementation of class BaseSolver
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "basesolver.h"

#include "twophasemediumproperties.h"

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

void BaseSolver::setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties){
}
