/* *****************************************************************
 * Interface of class TwoPhaseMedium
 *
 * TwoPhaseMedium is the default object embedding the fluid property
 * computations at a given point of the plant. 
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#ifndef TWOPHASEMEDIUM_H_
#define TWOPHASEMEDIUM_H_

#include "include.h"

#include "basesolver.h"

class TwoPhaseMedium{
public:
	TwoPhaseMedium(const string &mediumName, const string &libraryName, 
				   const string &substanceName, BaseSolver *const solver);
	~TwoPhaseMedium();

	void setSat_p(const double &p);
	void setSat_T(const double &T);

	double saturationPressure(const double &T, const string &mediumName);
	double saturationTemperature(const double &p, const string &mediumName);

	void setState_dT(const double &d, const double &T, const int &phase);
	void setState_ph(const double &p, const double &h, const int &phase);
	void setState_ps(const double &p, const double &s, const int &phase);
	void setState_pT(const double &p, const double &T);

	// Fluid properties
	string mediumName;		// medium name
	string libraryName;		// library name
	string substanceName;	// substance name

	BaseSolver *solver;		// pointer to solver

	int phase;			// 2 for two-phase, 1 for one-phase, 0 if not known

	double beta;		// isothermal expansion coefficient
	double cp;			// specific heat capacity cp
	double cv;			// specific heat capacity cv
	double d;			// density
	double dd_dp_h;		// derivative of density by pressure at constant enthalpy
	double dd_dh_p;		// derivative of density by enthalpy at constant pressure
	double h;			// specific enthalpy
	double kappa;		// compressibility
	double p;			// pressure
	double s;			// specific entropy
	double T;			// temperature

	double ps;			// saturation pressure
	double Ts;			// saturation temperature

	double dl;			// bubble density
	double dv;			// dew density
	double hl;			// bubble specific enthalpy
	double hv;			// dew specific enthalpy
	double sl;			// bubble specific entropy
	double sv;			// dew specific entropy

	double dc;			// critical density
	double pc;			// critical pressure
	double Tc;			// critical temperature

	double MM;			// molar mass

	double eta;			// dynamic viscosity
	double lambda;		// thermal conductivity
	double Pr;			// Prandtl number
	double sigma;		// surface tension
};

#endif /*TWOPHASEMEDIUM_H_*/
