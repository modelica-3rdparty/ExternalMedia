/* *****************************************************************
 * Implementation of the base class BaseTwoPhaseMedium.cpp
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

BaseTwoPhaseMedium::BaseTwoPhaseMedium(const string &mediumName, 
		                               const string &libraryName,
		                               const string &substanceName){
	_mediumName = mediumName;
	_libraryName = libraryName;
	_substanceName = substanceName;
}

BaseTwoPhaseMedium::~BaseTwoPhaseMedium(){
}

double BaseTwoPhaseMedium::beta() const{
	return _beta;
}

double BaseTwoPhaseMedium::cp() const{
	return _cp;
}

double BaseTwoPhaseMedium::cv() const{
	return _cv;
}

double BaseTwoPhaseMedium::d() const{
	return _d;
}

double BaseTwoPhaseMedium::dd_dp_h() const{
	return _dd_dp_h;
}

double BaseTwoPhaseMedium::dd_dh_p() const{
	return _dd_dh_p;
}

double BaseTwoPhaseMedium::h() const{
	return _h;
}

double BaseTwoPhaseMedium::kappa() const{
	return _kappa;
}

double BaseTwoPhaseMedium::p() const{
	return _p;
}

double BaseTwoPhaseMedium::s() const{
	return _s;
}

double BaseTwoPhaseMedium::T() const{
	return _T;
}

double BaseTwoPhaseMedium::ps() const{
	return _ps;
}

double BaseTwoPhaseMedium::Ts() const{
	return _Ts;
}

double BaseTwoPhaseMedium::dl() const{
	return _dl;
}

double BaseTwoPhaseMedium::dv() const{
	return _dv;
}

double BaseTwoPhaseMedium::hl() const{
	return _hl;
}

double BaseTwoPhaseMedium::hv() const{
	return _hv;
}

double BaseTwoPhaseMedium::sl() const{
	return _sl;
}

double BaseTwoPhaseMedium::sv() const{
	return _sv;
}

double BaseTwoPhaseMedium::dc() const{
	return _dc;
}

double BaseTwoPhaseMedium::pc() const{
	return _pc;
}

double BaseTwoPhaseMedium::Tc() const{
	return _Tc;
}

double BaseTwoPhaseMedium::MM() const{
	return _MM;
}

double BaseTwoPhaseMedium::eta() const{
	return _eta;
}

double BaseTwoPhaseMedium::lambda() const{
	return _lambda;
}

double BaseTwoPhaseMedium::Pr() const{
	return _Pr;
}

double BaseTwoPhaseMedium::sigma() const{
	return _sigma;
}
	
void BaseTwoPhaseMedium::initializeLibrary() const{
	return;  // do nothing by default
}

bool BaseTwoPhaseMedium::inputIsEqual_p(const double &p){
	return (fabs(_ps - p) < EPSILON);
}

bool BaseTwoPhaseMedium::inputIsEqual_T(const double &T){
	return (fabs(_Ts - T) < EPSILON);
}

bool BaseTwoPhaseMedium::inputsAreEqual_dT(const double &d, const double &T, const int &phase){
	return (fabs(_d - d)/std::max(_d,1e-12) < EPSILON && fabs(_T - T)/std::max(_T,1e-12) < EPSILON && _phase == phase);
}

bool BaseTwoPhaseMedium::inputsAreEqual_ph(const double &p, const double &h, const int &phase){
	return (fabs(_p - p)/std::max(_p,1e-12) < EPSILON && fabs(_h - h)/std::max(_h,1e-12) < EPSILON && _phase == phase);
}

bool BaseTwoPhaseMedium::inputsAreEqual_ps(const double &p, const double &s, const int &phase){
	return (fabs(_p - p)/std::max(_p,1e-12) < EPSILON && fabs(_s - s)/std::max(_s,1e-12) < EPSILON && _phase == phase);
}

bool BaseTwoPhaseMedium::inputsAreEqual_pT(const double &p, const double &T){
	return (fabs(_p - p)/std::max(_p,1e-12) < EPSILON && fabs(_T - T)/std::max(_T,1e-12) < EPSILON);
}
