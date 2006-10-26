/* *****************************************************************
 * Implementation of class TestSolver
 *
 * Francesco Casella, Christoph Richter, Oct 2006
 ********************************************************************/

#include "testsolver.h"
#include "twophasemediumproperties.h"
#include <math.h>

TestSolver::TestSolver(const string &mediumName, const string &libraryName, const string &substanceName)
	: BaseSolver(mediumName, libraryName, substanceName){
}

TestSolver::~TestSolver(){
}

void TestSolver::setMediumConstants(TwoPhaseMediumProperties *const properties){
  properties->MM = 0.18;
}

void TestSolver::setSat_p(const double &p, TwoPhaseMediumProperties *const properties){
}

void TestSolver::setSat_T(const double &T, TwoPhaseMediumProperties *const properties){
}

void TestSolver::setState_ph(const double &p, const double &h, const int &phase, TwoPhaseMediumProperties *const properties){
	properties->p = p;
	properties->h = h;
	properties->T = h/4200.0 + 273.15;
	properties->d = (1000.0 - h/4200.0)*(1.0 + p/21000e5);
	properties->dd_dp_h = (1000.0 - h/4200.0)/21000e5;
	properties->dd_dh_p = -(1.0 + p/21000e5)/4200.0;
	properties->s = 4200.0 * log(properties->T/273.15);
}

void TestSolver::setState_pT(const double &p, const double &T, TwoPhaseMediumProperties *const properties){
	properties->p = p;
	properties->T = T;
	properties->h = (T - 273.15)*4200.0;
	properties->d = (1000.0 - properties->h/4200.0)*(1 + p/21000e5);
	properties->dd_dp_h = (1000.0 - properties->h/4200.0)/21000e5;
	properties->dd_dh_p = -(1.0 + p/21000e5)/4200.0;
	properties->s = 4200.0 * log(properties->T/273.15);
}

void TestSolver::setState_dT(const double &d, const double &T, const int &phase, TwoPhaseMediumProperties *const properties){
	properties->d = d;
	properties->T = T;
	properties->h = (T - 273.15)*4200;
	properties->p = 1e5;
	properties->dd_dp_h = (1000.0 - properties->h/4200.0)/21000e5;
	properties->dd_dh_p = -(1.0 + properties->p/21000e5)/4200.0;
	properties->s = 4200.0 * log(properties->T/273.15);
}

void TestSolver::setState_ps(const double &p, const double &s, const int &phase, TwoPhaseMediumProperties *const properties){
	properties->p = p;
	properties->s = s;
	properties->T = 273.15*exp(s/4200);
	properties->h = (properties->T - 273.15)*4200;
	properties->d = (1000.0 - properties->h/4200.0)*(1.0 + p/21000e5);
	properties->dd_dp_h = (1000.0 - properties->h/4200.0)/21000e5;
	properties->dd_dh_p = -(1.0+p/21000e5)/4200.0;
}
