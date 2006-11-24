/* *****************************************************************
 * Implementation of the base class BaseTwoPhaseMedium
 *
 * The BaseTwoPhaseMedium class defines all the variables and member
 * functions which are needed to use external Modelica medium models
 * extending from PartialExternalTwoPhaseMedium.
 * 
 * The functions defined here are not fluid-specific, thus need not
 * be adapted to your own specific fluid property computation code.
 *
 * Francesco Casella, Christoph Richter Sep 2006
 ********************************************************************/

#include "basetwophasemedium.h"

#include <math.h>

BaseTwoPhaseMedium::BaseTwoPhaseMedium(const string &mediumName, const string &libraryName, 
									   const string &substanceName, BaseSolver *const solver, 
									   const int &uniqueID)
	: _solver(solver){
}

BaseTwoPhaseMedium::~BaseTwoPhaseMedium(){
}

int BaseTwoPhaseMedium::uniqueID() const{
	return _properties->uniqueID;
}

string BaseTwoPhaseMedium::mediumName() const{
	return _properties->mediumName;
}

string BaseTwoPhaseMedium::libraryName() const{
	return _properties->libraryName;
}

string BaseTwoPhaseMedium::substanceName() const{
	return _properties->substanceName;
}

TwoPhaseMediumProperties *BaseTwoPhaseMedium::properties() const{
	return _properties;
}

BaseSolver *BaseTwoPhaseMedium::solver() const{
	return _solver;
}

void BaseTwoPhaseMedium::setSolver(BaseSolver *const solver){
	_solver = solver;
}

double BaseTwoPhaseMedium::beta() const{
	return _properties->beta;
}

double BaseTwoPhaseMedium::cp() const{
	return _properties->cp;
}

double BaseTwoPhaseMedium::cv() const{
	return _properties->cv;
}

double BaseTwoPhaseMedium::d() const{
	return _properties->d;
}

double BaseTwoPhaseMedium::dd_dp_h() const{
	return _properties->dd_dp_h;
}

double BaseTwoPhaseMedium::dd_dh_p() const{
	return _properties->dd_dh_p;
}

double BaseTwoPhaseMedium::h() const{
	return _properties->h;
}

double BaseTwoPhaseMedium::kappa() const{
	return _properties->kappa;
}

double BaseTwoPhaseMedium::p() const{
	return _properties->p;
}

double BaseTwoPhaseMedium::s() const{
	return _properties->s;
}

double BaseTwoPhaseMedium::T() const{
	return _properties->T;
}

double BaseTwoPhaseMedium::Ts() const{
	return _properties->Ts;
}

double BaseTwoPhaseMedium::dl() const{
	return _properties->dl;
}

double BaseTwoPhaseMedium::dv() const{
	return _properties->dv;
}

double BaseTwoPhaseMedium::hl() const{
	return _properties->hl;
}

double BaseTwoPhaseMedium::hv() const{
	return _properties->hv;
}

double BaseTwoPhaseMedium::sl() const{
	return _properties->sl;
}

double BaseTwoPhaseMedium::sv() const{
	return _properties->sv;
}

double BaseTwoPhaseMedium::d_Ts_dp() const{
	return _properties->d_Ts_dp;
}

double BaseTwoPhaseMedium::d_dl_dp() const{
	return _properties->d_dl_dp;
}

double BaseTwoPhaseMedium::d_dv_dp() const{
	return _properties->d_dv_dp;
}

double BaseTwoPhaseMedium::d_hl_dp() const{
	return _properties->d_hl_dp;
}

double BaseTwoPhaseMedium::d_hv_dp() const{
	return _properties->d_hv_dp;
}

double BaseTwoPhaseMedium::dc() const{
	return _properties->dc;
}

double BaseTwoPhaseMedium::pc() const{
	return _properties->pc;
}

double BaseTwoPhaseMedium::Tc() const{
	return _properties->Tc;
}

double BaseTwoPhaseMedium::MM() const{
	return _properties->MM;
}

double BaseTwoPhaseMedium::eta() const{
	return _properties->eta;
}

double BaseTwoPhaseMedium::lambda() const{
	return _properties->lambda;
}

double BaseTwoPhaseMedium::Pr() const{
	return _properties->Pr;
}

double BaseTwoPhaseMedium::sigma() const{
	return _properties->sigma;
}

void BaseTwoPhaseMedium::setSat_p(const double &p){
	_solver->setSat_p(p, _properties);
}

void BaseTwoPhaseMedium::setSat_T(const double &T){
	_solver->setSat_T(T, _properties);
}

void BaseTwoPhaseMedium::setState_dT(const double &d, const double &T, const int &phase){
	_solver->setState_dT(d, T, phase, _properties);
}

void BaseTwoPhaseMedium::setState_ph(const double &p, const double &h, const int &phase){
	_solver->setState_ph(p, h, phase, _properties);
}

void BaseTwoPhaseMedium::setState_ps(const double &p, const double &s, const int &phase){
	_solver->setState_ps(p, s, phase, _properties);
}

void BaseTwoPhaseMedium::setState_pT(const double &p, const double &T){
	_solver->setState_pT(p, T, _properties);
}
