#include "basesolver.h"

#include "twophasemediumproperties.h"

BaseSolver::BaseSolver(const string &libraryName, const string &substanceName)
	: libraryName(libraryName), substanceName(substanceName){
}

BaseSolver::~BaseSolver(){
}

void BaseSolver::setMediumConstants(TwoPhaseMediumProperties *const properties){
	/* ***************************************************/
	/* Temporary dummy implementation for test in Dymla	 */
	/* ***************************************************/
	properties->MM = 12.3;
}

void BaseSolver::setSat_p(const double &p, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setSat_T(const double &T, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_dT(const double &d, const double &T, const int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_ph(const double &p, const double &h, const int &phase, TwoPhaseMediumProperties *const properties){
	/* ***************************************************/
	/* Temporary dummy implementation for test in Dymla	 */
	/* ***************************************************/
	properties->d = 1.0;
	properties->s = 2.0;
	properties->T = 3.0;
}

void BaseSolver::setState_ps(const double &p, const double &s, const int &phase, TwoPhaseMediumProperties *const properties){
}

void BaseSolver::setState_pT(const double &p, const double &T, TwoPhaseMediumProperties *const properties){
}
