/* *****************************************************************
 * Implementation of class TwoPhaseMedium
 *
 * TwoPhaseMedium is the default object embedding the fluid property
 * computations at a given point of the plant. 
 * 
 * TwoPhaseMedium extends BaseTwoPhaseMedium.
 *
 * See the header file for further documentation
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "twophasemedium.h"

// General purpose includes
#include <math.h>

TwoPhaseMedium::TwoPhaseMedium(const string &mediumName, const string &libraryName, const string &substanceName, BaseSolver *const solver)
	: mediumName(mediumName), libraryName(libraryName), substanceName(substanceName), solver(solver){
	// Set medium constants
	solver->setMediumConstants(this);
}

TwoPhaseMedium::~TwoPhaseMedium(){
}

void TwoPhaseMedium::setSat_p(const double &p){
	solver->setSat_p(p, this);
}

void TwoPhaseMedium::setSat_T(const double &T){
	solver->setSat_T(T, this);
}

void TwoPhaseMedium::setState_dT(const double &d, const double &T, const int &phase){
	solver->setState_dT(d, T, phase, this);
}

void TwoPhaseMedium::setState_ph(const double &p, const double &h, const int &phase){
	solver->setState_ph(p, h, phase, this);
}

void TwoPhaseMedium::setState_ps(const double &p, const double &s, const int &phase){
	solver->setState_ps(p, s, phase, this);
}

void TwoPhaseMedium::setState_pT(const double &p, const double &T){
	solver->setState_pT(p, T, this);
}

double TwoPhaseMedium::saturationPressure(const double &T, const string &mediumName){
	return 0.0;
}

double TwoPhaseMedium::saturationTemperature(const double &p, const string &mediumName){
	return 0.0;
}
