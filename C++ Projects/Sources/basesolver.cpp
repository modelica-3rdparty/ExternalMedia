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
   
double BaseSolver::getMolarMass(){
	return fluidConstants.MM;
}

double BaseSolver::getCriticalTemperature(){
	return fluidConstants.Tc;
}

double BaseSolver::getCriticalPressure(){
	return fluidConstants.pc;
}

void BaseSolver::setFluidConstants(){
}

void BaseSolver::setSat_p(const double &p, TwoPhaseMediumProperties *const properties){
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
