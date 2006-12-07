/* *****************************************************************
 * Implementation of class TestSolver
 *
 * Francesco Casella, Christoph Richter, Oct 2006
 ********************************************************************/

#include "testsolver.h"
#include "twophasemediumproperties.h"
#include "mediummap.h"
#include <math.h>

TestSolver::TestSolver(const string &mediumName, const string &libraryName, const string &substanceName)
	: BaseSolver(mediumName, libraryName, substanceName){
  setFluidConstants();
}

TestSolver::~TestSolver(){
}

void TestSolver::setFluidConstants(){
  _fluidConstants.pc = 220.0e5;
  _fluidConstants.Tc = 650.0;
  _fluidConstants.MM = 0.018;
  _fluidConstants.dc = 322;
}

void TestSolver::setSat_p(double &p, TwoPhaseMediumProperties *const properties){
	properties->ps = p;
	properties->Ts = 372.0 + (393.0-373.0)*(p - 1.0e5)/1.0e5;
	properties->dl = 958.0 + (940.0 - 958.0)*(p - 1.0e5)/1.0e5;
	properties->dv = 0.59 + (1.13 - 0.59)*(p - 1.0e5)/1.0e5;
	properties->hl = 417.5e3 + (504.7e3 - 417.5e3)*(p - 1.0e5)/1.0e5;
	properties->hv = 2.67e6 + (2.71e6 - 2.67e6)*(p - 1.0e5)/1.0e5;
    properties->d_Ts_dp = (393.0-373.0)/1.0e5;
	properties->d_dl_dp = (940.0 - 958.0)/1.0e5;
	properties->d_dv_dp = (1.13 - 0.59)/1.0e5;
	properties->d_hl_dp = (504.7e3 - 417.5e3)/1.0e5;
	properties->d_hv_dp = (2.71e6 - 2.67e6)/1.0e5;
}

void TestSolver::setSat_T(double &T, TwoPhaseMediumProperties *const properties){
	properties->Ts = T;
	double p = 1e5 + 1e5*(T-372)/(393-373);
	properties->ps = p;
	properties->dl = 958.0 + (940.0 - 958.0)*(p - 1.0e5)/1.0e5;
	properties->dv = 0.59 + (1.13 - 0.59)*(p - 1.0e5)/1.0e5;
	properties->hl = 417.5e3 + (504.7e3 - 417.5e3)*(p - 1.0e5)/1.0e5;
	properties->hv = 2.67e6 + (2.71e6 - 2.67e6)*(p - 1.0e5)/1.0e5;
    properties->d_Ts_dp = (393.0-373.0)/1.0e5;
	properties->d_dl_dp = (940.0 - 958.0)/1.0e5;
	properties->d_dv_dp = (1.13 - 0.59)/1.0e5;
	properties->d_hl_dp = (504.7e3 - 417.5e3)/1.0e5;
	properties->d_hv_dp = (2.71e6 - 2.67e6)/1.0e5;
}

void TestSolver::setSat_p_state(TwoPhaseMediumProperties *const properties){
  setSat_p(properties->p, properties);
};

void TestSolver::setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties){
	properties->p = p;
	properties->h = h;
	properties->T = h/4200.0 + 273.15;
	properties->d = (1000.0 - h/4200.0)*(1.0 + p/21000e5);
	properties->s = 4200.0 * log(properties->T/273.15);
	properties->cp = 4200;
	properties->cv = 4150;
	properties->beta = 2.4e-4;
	properties->kappa = 4.5e-10;
	properties->dd_dp_h = (1000.0 - h/4200.0)/21000e5;
	properties->dd_dh_p = -(1.0 + p/21000e5)/4200.0;
	properties->dT_dp_h = 0;
	properties->dT_dh_p = 1.0/4200.0;
}

void TestSolver::setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties){
	properties->p = p;
	properties->T = T;
	properties->h = (T - 273.15)*4200.0;
	properties->d = (1000.0 - properties->h/4200.0)*(1 + p/21000e5);
	properties->s = 4200.0 * log(properties->T/273.15);
	properties->cp = 4200;
	properties->cv = 4150;
	properties->beta = 2.4e-4;
	properties->kappa = 4.5e-10;
	properties->dd_dp_h = (1000.0 - properties->h/4200.0)/21000e5;
	properties->dd_dh_p = -(1.0 + p/21000e5)/4200.0;
	properties->dT_dp_h = 0;
	properties->dT_dh_p = 1.0/4200.0;
}

void TestSolver::setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties){
	properties->d = d;
	properties->T = T;
	properties->h = (T - 273.15)*4200;
	properties->p = 1e5;
	properties->s = 4200.0 * log(properties->T/273.15);
	properties->cp = 4200;
	properties->cv = 4150;
	properties->beta = 2.4e-4;
	properties->kappa = 4.5e-10;
	properties->dd_dp_h = (1000.0 - properties->h/4200.0)/21000e5;
	properties->dd_dh_p = -(1.0 + properties->p/21000e5)/4200.0;
	properties->dT_dp_h = 0;
	properties->dT_dh_p = 1.0/4200.0;
}

void TestSolver::setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties){
	properties->p = p;
	properties->s = s;
	properties->T = 273.15*exp(s/4200);
	properties->h = (properties->T - 273.15)*4200;
	properties->d = (1000.0 - properties->h/4200.0)*(1.0 + p/21000e5);
	properties->cp = 4200;
	properties->cv = 4150;
	properties->beta = 2.4e-4;
	properties->kappa = 4.5e-10;
	properties->dd_dp_h = (1000.0 - properties->h/4200.0)/21000e5;
	properties->dd_dh_p = -(1.0+p/21000e5)/4200.0;
	properties->dT_dp_h = 0;
	properties->dT_dh_p = 1.0/4200.0;
}

