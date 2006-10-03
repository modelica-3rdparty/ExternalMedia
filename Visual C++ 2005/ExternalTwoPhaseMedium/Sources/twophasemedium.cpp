/* *****************************************************************
 * Implementation of class TwoPhaseMedium
 *
 * TwoPhaseMedium is the default object embedding the fluid property
 * computations at a given point of the plant. 
 * 
 * To test the compiler setup, uncomment the directive
 * #define COMPILER_TEST; the TwoPhaseMedium object will compute
 * dummy properties, without needing to be interfaced with any
 * actual external fluid property computation code.
 *
 * To implement the interface to your own external fluid property
 * computation software, comment the directive
 * #define COMPILER_TEST, and fill in the blanks in the code with 
 * the appropriate function calls to your external code.
 * 
 * TwoPhaseMedium extends BaseTwoPhaseMedium.
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "twophasemedium.h"
#include <math.h>

#define COMPILER_TEST

TwoPhaseMedium::TwoPhaseMedium(const string &mediumName) : BaseTwoPhaseMedium(mediumName){
}

TwoPhaseMedium::~TwoPhaseMedium(){
}

void TwoPhaseMedium::setState_ph(const double &p, const double &h, const int &phase){
#ifdef COMPILER_TEST
	_p = p;
	_h = h;
	_T = h/4200 + 273.15;
	_d = (1000 - h/4200)*(1+_p/21000e5);
	_s = 4200 * log(_T/273);
#else
// Place your code here
#endif
}

void TwoPhaseMedium::setState_pT(const double &p, const double &T){
	_p = p;
	_T = T;
	_h = (T - 273.15)*4200;
	_d = (1000 - _h/4200)*(1+_p/21000e5);
	_s = 4200 * log(_T/273);
}

void TwoPhaseMedium::setState_dT(const double &d, const double &T, const int &phase){
#ifdef COMPILER_TEST
	_d = d;
	_T = _T;
	_h = (T - 273.15)*4200;
	_p = 1e5;
	_s = 4200 * log(_T/273);
#else
// Place your code here
#endif
}

void TwoPhaseMedium::setState_ps(const double &p, const double &s, const int &phase){
#ifdef COMPILER_TEST
	_p = p;
	_s = s;
	_T = 273.15*exp(s/4200);
	_h = (_T - 273.15)*4200;
	_d = (1000 - _h/4200)*(1+_p/21000e5);
#else
// Place your code here
#endif
}

void TwoPhaseMedium::setSat_p(const double &p){
#ifdef COMPILER_TEST
#else
// Place your code here
#endif
}

void TwoPhaseMedium::setSat_T(const double &T){
}

double TwoPhaseMedium::saturationPressure(const double &T, const string &mediumName){
	return 0.0;
}

double TwoPhaseMedium::saturationTemperature(const double &p, const string &mediumName){
	return 0.0;
}

