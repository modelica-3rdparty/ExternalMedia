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
	: BaseTwoPhaseMedium(mediumName, libraryName, substanceName, solver){
	// Set medium constants
	_solver->setMediumConstants(_properties);
}

TwoPhaseMedium::~TwoPhaseMedium(){
}

void TwoPhaseMedium::setSat_p(const double &p){
	_solver->setSat_p(p, _properties);
}

void TwoPhaseMedium::setSat_T(const double &T){
	_solver->setSat_T(T, _properties);
}

void TwoPhaseMedium::setState_dT(const double &d, const double &T, const int &phase){
	_solver->setState_dT(d, T, phase, _properties);
}

void TwoPhaseMedium::setState_ph(const double &p, const double &h, const int &phase){
	_solver->setState_ph(p, h, phase, _properties);
}

void TwoPhaseMedium::setState_ps(const double &p, const double &s, const int &phase){
	_solver->setState_ps(p, s, phase, _properties);
}

void TwoPhaseMedium::setState_pT(const double &p, const double &T){
	_solver->setState_pT(p, T, _properties);
}

double TwoPhaseMedium::saturationPressure(const double &T, const string &mediumName){
	return 0.0;
}

double TwoPhaseMedium::saturationTemperature(const double &p, const string &mediumName){
	return 0.0;
}
