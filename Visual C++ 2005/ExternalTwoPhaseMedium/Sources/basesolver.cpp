#include "basesolver.h"

#include "twophasemedium.h"

BaseSolver::BaseSolver(const string &libraryName, const string &substanceName)
	: libraryName(libraryName), substanceName(substanceName){
}

BaseSolver::~BaseSolver(){
}

void BaseSolver::setMediumConstants(TwoPhaseMedium *const medium){
	/* ***************************************************/
	/* Temporary dummy implementation for test in Dymla	 */
	/* ***************************************************/
	medium->MM = 12.3;
}

void BaseSolver::setSat_p(const double &p, TwoPhaseMedium *const medium){
}

void BaseSolver::setSat_T(const double &T, TwoPhaseMedium *const medium){
}

void BaseSolver::setState_dT(const double &d, const double &T, const int &phase, TwoPhaseMedium *const medium){
}

void BaseSolver::setState_ph(const double &p, const double &h, const int &phase, TwoPhaseMedium *const medium){
	/* ***************************************************/
	/* Temporary dummy implementation for test in Dymla	 */
	/* ***************************************************/
	medium->d = 1.0;
	medium->s = 2.0;
	medium->T = 3.0;
}

void BaseSolver::setState_ps(const double &p, const double &s, const int &phase, TwoPhaseMedium *const medium){
}

void BaseSolver::setState_pT(const double &p, const double &T, TwoPhaseMedium *const medium){
}
