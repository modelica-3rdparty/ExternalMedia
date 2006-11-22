/* *****************************************************************
 * Definition of the TwoPhaseMediumProperties struct
 *
 * The TwoPhaseMediumProperties struct defines all the properties that are
 * computed by external Modelica medium models
 * extending from PartialExternalTwoPhaseMedium.
 * 
 * Francesco Casella, Christoph Richter Sep 2006
 ********************************************************************/

#ifndef TWOPHASEMEDIUMPROPERTIES_H_
#define TWOPHASEMEDIUMPROPERTIES_H_

#include "include.h"

struct TwoPhaseMediumProperties{
	string mediumName;			// medium name
	string libraryName;			// external library name
	string substanceName;		// substance name
	int uniqueID;	// unique ID number

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

    // Saturation properties at the fluid pressure

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

    double d_Ts_dp;  // derivative of Ts by pressure
	double d_dl_dp; // derivative of dls by pressure
	double d_dv_dp; // derivative of dvs by pressure
    double d_hl_dp; // derivative of hls by pressure
	double d_hv_dp; // derivative of hvs by pressure

	double d_dl_dP_hL; // derivative of density by pressure at constant enthalpy, bubble point, liquid side
	double d_dl_dP_h2; // derivative of density by pressure at constant enthalpy, bubble point, 2-phase side
	double d_dv_dP_hV; // derivative of density by pressure at constant enthalpy, bubble point, vapour side
	double d_dv_dP_h2; // derivative of density by pressure at constant enthalpy, bubble point, 2-phase side
};

#endif /*TWOPHASEMEDIUMPROPERTIES_H_*/
