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

void BaseSolver::setMediumConstants(TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setSat_p(const double &p, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setSat_T(const double &T, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_dT(const double &d, const double &T, const int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_ph(const double &p, const double &h, const int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_ps(const double &p, const double &s, const int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_pT(const double &p, const double &T, TwoPhaseMediumProperties *const properties){
}
