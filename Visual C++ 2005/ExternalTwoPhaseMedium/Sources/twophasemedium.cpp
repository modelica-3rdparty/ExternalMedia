/* *****************************************************************
 * Implementation of the base class TwoPhaseMedium.cpp
 *
 * Francesco Casella, Christoph Richter Sep 2006
 ********************************************************************/

#include "twophasemedium.h"
#include <math.h>

TwoPhaseMedium::TwoPhaseMedium(const string &mediumName){
	_mediumName = mediumName;
}

TwoPhaseMedium::~TwoPhaseMedium(){
}

double TwoPhaseMedium::beta() const{
	return _beta;
}

double TwoPhaseMedium::cp() const{
	return _cp;
}

double TwoPhaseMedium::cv() const{
	return _cv;
}

double TwoPhaseMedium::d() const{
	return _d;
}

double TwoPhaseMedium::h() const{
	return _h;
}

double TwoPhaseMedium::kappa() const{
	return _kappa;
}

double TwoPhaseMedium::p() const{
	return _p;
}

double TwoPhaseMedium::s() const{
	return _s;
}

double TwoPhaseMedium::T() const{
	return _T;
}

double TwoPhaseMedium::ps() const{
	return _ps;
}

double TwoPhaseMedium::Ts() const{
	return _Ts;
}

double TwoPhaseMedium::dl() const{
	return _dl;
}

double TwoPhaseMedium::dv() const{
	return _dv;
}

double TwoPhaseMedium::hl() const{
	return _hl;
}

double TwoPhaseMedium::hv() const{
	return _hv;
}

double TwoPhaseMedium::sl() const{
	return _sl;
}

double TwoPhaseMedium::sv() const{
	return _sv;
}

double TwoPhaseMedium::dc() const{
	return _dc;
}

double TwoPhaseMedium::pc() const{
	return _pc;
}

double TwoPhaseMedium::Tc() const{
	return _Tc;
}

double TwoPhaseMedium::MM() const{
	return _MM;
}

double TwoPhaseMedium::eta() const{
	return _eta;
}

double TwoPhaseMedium::lambda() const{
	return _lambda;
}

double TwoPhaseMedium::Pr() const{
	return _Pr;
}

double TwoPhaseMedium::sigma() const{
	return _sigma;
}
	
void TwoPhaseMedium::initializeLibrary() const{
	return;  // do nothing by default
}

bool TwoPhaseMedium::inputIsEqual_p(const double &p){
	return (fabs(_ps - p) < EPSILON);
}

bool TwoPhaseMedium::inputIsEqual_T(const double &T){
	return (fabs(_Ts - T) < EPSILON);
}

bool TwoPhaseMedium::inputsAreEqual_dT(const double &d, const double &T, const int &phase){
	return (fabs(_d - d) < EPSILON && fabs(_T - T) < EPSILON && _phase == phase);
}

bool TwoPhaseMedium::inputsAreEqual_ph(const double &p, const double &h, const int &phase){
	return (fabs(_p - p) < EPSILON && fabs(_h - h) < EPSILON && _phase == phase);
}

bool TwoPhaseMedium::inputsAreEqual_ps(const double &p, const double &s, const int &phase){
	return (fabs(_p - p) < EPSILON && fabs(_s - s) < EPSILON && _phase == phase);
}

bool TwoPhaseMedium::inputsAreEqual_pT(const double &p, const double &T){
	return (fabs(_p - p) < EPSILON && fabs(_T - T) < EPSILON);
}
