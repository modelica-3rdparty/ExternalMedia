#include "testsolver.h"
#include "errorhandling.h"
#include <math.h>

TestSolver::TestSolver(const std::string &mediumName, const std::string &libraryName, const std::string &substanceName)
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

void TestSolver::setSat_p(double &p, ExternalSaturationProperties *const properties){
	properties->Tsat = 372.0 + (393.0-373.0)*(p - 1.0e5)/1.0e5;
    properties->dTp = (393.0-373.0)/1.0e5;
	properties->ddldp = (940.0 - 958.0)/1.0e5;
	properties->ddvdp = (1.13 - 0.59)/1.0e5;
	properties->dhldp = (504.7e3 - 417.5e3)/1.0e5;
	properties->dhvdp = (2.71e6 - 2.67e6)/1.0e5;
	properties->dl = 958.0 + (940.0 - 958.0)*(p - 1.0e5)/1.0e5;
	properties->dv = 0.59 + (1.13 - 0.59)*(p - 1.0e5)/1.0e5;
	properties->hl = 417.5e3 + (504.7e3 - 417.5e3)*(p - 1.0e5)/1.0e5;
	properties->hv = 2.67e6 + (2.71e6 - 2.67e6)*(p - 1.0e5)/1.0e5;
	properties->psat = p;
}

void TestSolver::setSat_T(double &T, ExternalSaturationProperties *const properties){
	properties->Tsat = T;
	double p = 1e5 + 1e5*(T-372)/(393-373);
	properties->psat = p;
    properties->dTp = (393.0-373.0)/1.0e5;
	properties->ddldp = (940.0 - 958.0)/1.0e5;
	properties->ddvdp = (1.13 - 0.59)/1.0e5;
	properties->dhldp = (504.7e3 - 417.5e3)/1.0e5;
	properties->dhvdp = (2.71e6 - 2.67e6)/1.0e5;
	properties->dl = 958.0 + (940.0 - 958.0)*(p - 1.0e5)/1.0e5;
	properties->dv = 0.59 + (1.13 - 0.59)*(p - 1.0e5)/1.0e5;
	properties->hl = 417.5e3 + (504.7e3 - 417.5e3)*(p - 1.0e5)/1.0e5;
	properties->hv = 2.67e6 + (2.71e6 - 2.67e6)*(p - 1.0e5)/1.0e5;
}

// Note: the phase input is currently not supported
void TestSolver::setState_ph(double &p, double &h, int &phase, ExternalThermodynamicState *const properties){
	properties->p = p;
	properties->h = h;
	properties->T = h/4200.0 + 273.15;
	properties->d = (1000.0 - h/4200.0)*(1.0 + p/21000e5);
	properties->s = 4200.0 * log(properties->T/273.15);
	if (phase == 0) {
		double hl = 417.5e3 + (504.7e3 - 417.5e3)*(p - 1.0e5)/1.0e5;
		double hv = 2.67e6 + (2.71e6 - 2.67e6)*(p - 1.0e5)/1.0e5;
		properties->phase = (h > hl && h < hv) ? 2 : 1;
	} else
		properties->phase = phase;
        properties->a = 1500;
	properties->beta = 2.4e-4;
	properties->cp = 4200;
	properties->cv = 4150;
	properties->ddhp = -(1.0 + p/21000e5)/4200.0;
	properties->ddph = (1000.0 - h/4200.0)/21000e5;
	properties->kappa = 4.5e-10;
	properties->eta = 9e-4 - (properties->T-300)*1e-5;
	properties->lambda = 0.60 + (properties->T-300)*1.6e-3;
}

void TestSolver::setState_pT(double &p, double &T, ExternalThermodynamicState *const properties){
	properties->p = p;
	properties->T = T;
	properties->h = (T - 273.15)*4200.0;
	properties->d = (1000.0 - properties->h/4200.0)*(1 + p/21000e5);
	properties->s = 4200.0 * log(properties->T/273.15);
	properties->phase = 1; // with pT input, always one-phase conditions!
        properties->a = 1500;
	properties->beta = 2.4e-4;
	properties->cp = 4200;
	properties->cv = 4150;
	properties->ddph = (1000.0 - properties->h/4200.0)/21000e5;
	properties->ddhp = -(1.0 + p/21000e5)/4200.0;
	properties->kappa = 4.5e-10;
	properties->eta = 9e-4 - (T-300)*1e-5;
	properties->lambda = 0.60 + (T-300)*1.6e-3;
}

// Note: the phase input is currently not supported
void TestSolver::setState_dT(double &d, double &T, int &phase, ExternalThermodynamicState *const properties){
	properties->d = d;
	properties->T = T;
	properties->h = (T - 273.15)*4200;
	properties->p = 1e5;
	properties->s = 4200.0 * log(properties->T/273.15);
	if (phase == 0) {
	    double p = properties->p;
		double h = properties->h;
		double hl = 417.5e3 + (504.7e3 - 417.5e3)*(p - 1.0e5)/1.0e5;
		double hv = 2.67e6 + (2.71e6 - 2.67e6)*(p - 1.0e5)/1.0e5;
		properties->phase = (h > hl && h < hv) ? 2 : 1;
	} else
		properties->phase = phase;
        properties->a = 1500;
	properties->beta = 2.4e-4;
	properties->cp = 4200;
	properties->cv = 4150;
	properties->ddph = (1000.0 - properties->h/4200.0)/21000e5;
	properties->ddhp = -(1.0 + properties->p/21000e5)/4200.0;
	properties->kappa = 4.5e-10;
	properties->eta = 9e-4 - (T-300)*1e-5;
	properties->lambda = 0.60 + (T-300)*1.6e-3;
}

// Note: the phase input is currently not supported
void TestSolver::setState_ps(double &p, double &s, int &phase, ExternalThermodynamicState *const properties){
	properties->p = p;
	properties->s = s;
	properties->T = 273.15*exp(s/4200);
	properties->h = (properties->T - 273.15)*4200;
	properties->d = (1000.0 - properties->h/4200.0)*(1.0 + p/21000e5);
	if (phase == 0) {
		double h = properties->h;
		double hl = 417.5e3 + (504.7e3 - 417.5e3)*(p - 1.0e5)/1.0e5;
		double hv = 2.67e6 + (2.71e6 - 2.67e6)*(p - 1.0e5)/1.0e5;
		properties->phase = (h > hl && h < hv) ? 2 : 1;
	} else
		properties->phase = phase;
        properties->a = 1500;
	properties->beta = 2.4e-4;
	properties->cp = 4200;
	properties->cv = 4150;
	properties->ddph = (1000.0 - properties->h/4200.0)/21000e5;
	properties->ddhp = -(1.0+p/21000e5)/4200.0;
	properties->kappa = 4.5e-10;
	properties->eta = 9e-4 - (properties->T-300)*1e-5;
	properties->lambda = 0.60 + (properties->T-300)*1.6e-3;
}


void TestSolver::setState_hs(double &h, double &s, int &phase, ExternalThermodynamicState *const properties) {
  errorMessage((char *)"Internal error: setState_hs not implemented in the TestSolver object");
}
double TestSolver::Pr(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: Pr not implemented in the TestSolver object");
  return 0;
}
double TestSolver::T(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: T not implemented in the TestSolver object");
  return 0;
}
double TestSolver::a(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: a not implemented in the TestSolver object");
  return 0;
}
double TestSolver::beta(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: beta not implemented in the TestSolver object");
  return 0;
}
double TestSolver::cp(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: cp not implemented in the TestSolver object");
  return 0;
}
double TestSolver::cv(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: cv not implemented in the TestSolver object");
  return 0;
}
double TestSolver::d(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: d not implemented in the TestSolver object");
  return 0;
}
double TestSolver::ddhp(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: ddhp not implemented in the TestSolver object");
  return 0;
}
double TestSolver::ddph(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: ddph not implemented in the TestSolver object");
  return 0;
}
double TestSolver::eta(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: eta not implemented in the TestSolver object");
  return 0;
}
double TestSolver::h(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: h not implemented in the TestSolver object");
  return 0;
}
double TestSolver::kappa(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: kappa not implemented in the TestSolver object");
  return 0;
}
double TestSolver::lambda(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: lambda not implemented in the TestSolver object");
  return 0;
}
double TestSolver::p(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: p not implemented in the TestSolver object");
  return 0;
}
int TestSolver::phase(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: phase not implemented in the TestSolver object");
  return 0;
}
double TestSolver::s(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: s not implemented in the TestSolver object");
  return 0;
}
double TestSolver::d_der(ExternalThermodynamicState *const properties) {
  errorMessage(
      (char *)"Internal error: d_der not implemented in the TestSolver object");
  return 0;
}
double TestSolver::isentropicEnthalpy(double &p, ExternalThermodynamicState *const properties) {
  errorMessage((char *)"Internal error: isentropicEnthalpy not implemented in "
                       "the Solver object");
  return 0;
}

double TestSolver::partialDeriv_state(const std::string &of, const std::string &wrt, const std::string &cst,
                                      ExternalThermodynamicState *const properties) {
  errorMessage((char *)"Internal error: partialDeriv_state not implemented in "
                       "the Solver object");
  return 0;
}

double TestSolver::dTp(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: dTp not implemented in the TestSolver object");
  return 0;
}
double TestSolver::ddldp(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: ddldp not implemented in the TestSolver object");
  return 0;
}
double TestSolver::ddvdp(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: ddvdp not implemented in the TestSolver object");
  return 0;
}
double TestSolver::dhldp(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: dhldp not implemented in the TestSolver object");
  return 0;
}
double TestSolver::dhvdp(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: dhvdp not implemented in the TestSolver object");
  return 0;
}
double TestSolver::dl(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: dl not implemented in the TestSolver object");
  return 0;
}
double TestSolver::dv(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: dv not implemented in the TestSolver object");
  return 0;
}
double TestSolver::hl(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: hl not implemented in the TestSolver object");
  return 0;
}
double TestSolver::hv(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: hv not implemented in the TestSolver object");
  return 0;
}
double TestSolver::sigma(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: sigma not implemented in the TestSolver object");
  return 0;
}
double TestSolver::sl(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: sl not implemented in the TestSolver object");
  return 0;
}
double TestSolver::sv(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: sv not implemented in the TestSolver object");
  return 0;
}

double TestSolver::psat(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: psat not implemented in the TestSolver object");
  return 0;
}
double TestSolver::Tsat(ExternalSaturationProperties *const properties) {
  errorMessage(
      (char *)"Internal error: Tsat not implemented in the TestSolver object");
  return 0;
}
