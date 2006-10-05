/* *****************************************************************
 * Implementation of class TwoPhaseMedium
 *
 * TwoPhaseMedium is the default object embedding the fluid property
 * computations at a given point of the plant. 
 * 
 * TwoPhaseMedium extends BaseTwoPhaseMedium.
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

// General purpose includes
#include "twophasemedium.h"
#include <math.h>

TwoPhaseMedium::TwoPhaseMedium(const string &mediumName) : BaseTwoPhaseMedium(mediumName){
#if defined (COMPILER_TEST)
	 // The (constant) molar mass should be defined when the object is created
	_MM = 0.18; 
#elif defined (FLUIDPROP)
	char* ErrorMsg;
    char* Comp[20];
    double Conc[20];

	FluidProp = new CFluidProp();
	Comp[0] = "";
    FluidProp->SetFluid("IF97", 1, Comp, Conc, ErrorMsg);
	_MM = 0.018015268f;
/*
    if ( strcmp( ErrorMsg, "No errors"))
    {
        cout << endl;
        cout << ErrorMsg << endl;
		return;
    }
	*/
#else
    // Place your code here
#endif

}

TwoPhaseMedium::~TwoPhaseMedium(){
#if defined (COMPILER_TEST)
	// Do nothing
#elif defined (FLUIDPROP)
	// Destroy object
	delete FluidProp;
#else
    // Place your code here
#endif
}

void TwoPhaseMedium::setState_ph(const double &p, const double &h, const int &phase){
#if defined (COMPILER_TEST)
	_p = p;
	_h = h;
	_T = h/4200 + 273.15;
	_d = (1000 - h/4200)*(1+_p/21000e5);
	_s = 4200 * log(_T/273);
#elif defined (FLUIDPROP)
	// FluidProp variables (with their default units)
    char* ErrorMsg;
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, gamma_, eta_, lambda_;
	// Compute all FluidProp variables
	FluidProp->AllProps( "Ph", p*1e-5, h*1e-3, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
                         alpha_, beta_, chi_, fi_, ksi_, psi_, gamma_, eta_, lambda_, ErrorMsg);
    // Fill in the TwoPhaseMedium variables (in SI units)
	_beta = 0;				// isothermal expansion coefficient
	_cp = cp_*1000.0;		// specific heat capacity cp
	_cv = cv_*1000.0;		// specific heat capacity cv
	_d = d_;				// density
	_h = h;					// specific enthalpy
	_kappa = 0;				// compressibility
	_p = p;					// pressure
	_s = s_*1000.0;		// specific entropy
	_T = T_ + 273.15;		// temperature

	_ps = 0;		// saturation pressure
	_Ts = 0;		// saturation temperature

	_dl = 0;		// bubble density
	_dv = 0;		// dew density
	_hl = 0;		// bubble specific enthalpy
	_hv = 0;		// dew specific enthalpy
	_sl = 0;		// bubble specific entropy
	_sv = 0;		// dew specific entropy

	_dc = 0;		// critical density
	_pc = 0;		// critical pressure
	_Tc = 0;		// critical temperature

	_eta = eta_;	    // dynamic viscosity
	_lambda = lambda_;	// thermal conductivity
	_Pr = 0;			// Prandtl number
	_sigma = 0;			// surface tension
#else
// Place your code here
#endif
}

void TwoPhaseMedium::setState_pT(const double &p, const double &T){
#if defined (COMPILER_TEST)
	_p = p;
	_T = T;
	_h = (T - 273.15)*4200;
	_d = (1000 - _h/4200)*(1+_p/21000e5);
	_s = 4200 * log(_T/273);
#elif defined (FLUIDPROP)
#else
// Place your code here
#endif
}

void TwoPhaseMedium::setState_dT(const double &d, const double &T, const int &phase){
#if defined (COMPILER_TEST)
	_d = d;
	_T = _T;
	_h = (T - 273.15)*4200;
	_p = 1e5;
	_s = 4200 * log(_T/273);
#elif defined (FLUIDPROP)

#else
// Place your code here
#endif
}

void TwoPhaseMedium::setState_ps(const double &p, const double &s, const int &phase){
#if defined (COMPILER_TEST)
	_p = p;
	_s = s;
	_T = 273.15*exp(s/4200);
	_h = (_T - 273.15)*4200;
	_d = (1000 - _h/4200)*(1+_p/21000e5);
#elif defined (FLUIDPROP)

#else
// Place your code here
#endif
}

void TwoPhaseMedium::setSat_p(const double &p){
#if defined (COMPILER_TEST)
#elif defined (FLUIDPROP)
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
