#ifndef TWOPHASEMEDIUMPROPERTIES_H_
#define TWOPHASEMEDIUMPROPERTIES_H_

#include "include.h"

struct TwoPhaseMediumProperties{
	string mediumName;			// medium name
	string libraryName;        // external library name
	string substanceName;      // substance name

	int phase;		// 2 for two-phase, 1 for one-phase, 0 if not known

	double beta;	// isothermal expansion coefficient
	double cp;		// specific heat capacity cp
	double cv;		// specific heat capacity cv
	double d;		// density
	double dd_dp_h; // derivative of density by pressure at constant enthalpy
	double dd_dh_p; // derivative of density by enthalpy at constant pressure
	double h;		// specific enthalpy
	double kappa;	// compressibility
	double p;		// pressure
	double s;		// specific entropy
	double T;		// temperature

	double ps;		// saturation pressure
	double Ts;		// saturation temperature

	double dl;		// bubble density
	double dv;		// dew density
	double hl;		// bubble specific enthalpy
	double hv;		// dew specific enthalpy
	double sl;		// bubble specific entropy
	double sv;		// dew specific entropy

	double dc;		// critical density
	double pc;		// critical pressure
	double Tc;		// critical temperature

	double MM;		// molar mass

	double eta;		// dynamic viscosity
	double lambda;	// thermal conductivity
	double Pr;		// Prandtl number
	double sigma;	// surface tension
};

#endif /*TWOPHASEMEDIUMPROPERTIES_H_*/
