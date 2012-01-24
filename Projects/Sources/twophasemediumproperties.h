#ifndef TWOPHASEMEDIUMPROPERTIES_H_
#define TWOPHASEMEDIUMPROPERTIES_H_

#include "include.h"

//! Two phase medium property class
/*!
  The two phase medium propery struct defines all the properties that
  are computed by external Modelica medium models extending from
  PartialExternalTwoPhaseMedium. It also contains the saturation properties
  for the dew and the bubble line which makes it a real two phase medium
  property struct.

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/
class ExternalMediaThermodynamicState{
public:
	TwoPhaseMediumProperties();
	void initializeFields();

	//! Density
	double d;
	//! Specific enthalpy
	double h;
	//! Pressure
	double p;
	//! Specific entropy
	double s;
	//! Temperature
	double T;
	//! Phase 
	/*!
	  This phase flag is defined according to the phase flag in Modelica.Media:
	  2 for two-phase, 1 for one-phase.
	*/
	int phase;
	//! Isobaric expansion coefficient
	double beta;
	//! Specific heat capacity cp
	double cp;
	//! Specific heat capacity cv
	double cv;
	//! Derivative of density wrt pressure at constant enthalpy
	double ddph;
	//! Derivative of density wrt enthalpy at constant pressure
	double ddhp;
	//! Compressibility
	double kappa;
	//! Derivative of temperature wrt pressure at constant enthalpy
	double dT_dp_h;
	//! Derivative of temperature wrt enthalpy at constant pressure
	double dT_dh_p;
	//! Dynamic viscosity
	double eta;
	//! Thermal conductivity
	double lambda;
	//! Prandtl number
	double Pr;
	//! Velocity of sound
	double a;
};

class ExternalMediaSaturationProperties{
public:
	TwoPhaseMediumProperties();
	void initializeFields();
	//! Saturation pressure
	double psat;
	//! Saturation temperature
	double Tsat;
	//! Density at bubble line (for pressure ps)
	double dl;
	//! Density at dew line (for pressure ps)
	double dv;
	//! Specific enthalpy at bubble line (for pressure ps)
	double hl;
	//! Specific enthalpy at dew line (for pressure ps)
	double hv;
	//! Specific entropy at bubble line (for pressure ps)
	double sl;
	//! Specific entropy at dew line (for pressure ps)
	double sv;
	//! Surface tension
	double sigma;
	//! Derivative of Ts wrt pressure
    double dTp;
	//! Derivative of dls wrt pressure
	double ddldp;
	//! Derivative of dvs wrt pressure
	double ddvdp;
	//! Derivative of hls wrt pressure
    double dhldp;
	//! Derivative of hvs wrt pressure
	double dhvdp;
};

#endif // TWOPHASEMEDIUMPROPERTIES_H_
