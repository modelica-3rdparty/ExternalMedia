/* *****************************************************************
 * Implementation of class DummyTwoPhaseMedium
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "dummytwophasemedium.h"

DummyTwoPhaseMedium::DummyTwoPhaseMedium(const string &mediumName) : TwoPhaseMedium(mediumName){
}

DummyTwoPhaseMedium::~DummyTwoPhaseMedium(){
}

void DummyTwoPhaseMedium::setSat_p(const double &p){
}

void DummyTwoPhaseMedium::setSat_T(const double &T){
}

double DummyTwoPhaseMedium::saturationPressure(const double &T, const string &mediumName){
	return 0.0;
}

double DummyTwoPhaseMedium::saturationTemperature(const double &p, const string &mediumName){
	return 0.0;
}

void DummyTwoPhaseMedium::setState_dT(const double &d, const double &T, const int &phase){
	_d = 1.0;
	_h = 2.0;
	_p = 3.0;
	_s = 4.0;
	_T = 5.0;
}

void DummyTwoPhaseMedium::setState_ph(const double &p, const double &h, const int &phase){
	_d = 10.0;
	_h = 20.0;
	_p = 30.0;
	_s = 40.0;
	_T = 50.0;
}

void DummyTwoPhaseMedium::setState_ps(const double &p, const double &s, const int &phase){
}

void DummyTwoPhaseMedium::setState_pT(const double &p, const double &T){
}
