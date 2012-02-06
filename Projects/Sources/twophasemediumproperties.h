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
typedef struct {
//class ExternalThermodynamicState{
public:
	TwoPhaseMediumProperties();
	void initializeFields();

	//! Prandtl number
	double Pr;
	//! Temperature
	double T;
	//! Velocity of sound
	double a;
	//! Isobaric expansion coefficient
	double beta;
	//! Specific heat capacity cp
	double cp;
	//! Specific heat capacity cv
	double cv;
	//! Density
	double d;
	//! Derivative of density wrt enthalpy at constant pressure
	double ddhp;
	//! Derivative of density wrt pressure at constant enthalpy
	double ddph;
	//! Dynamic viscosity
	double eta;
	//! Specific enthalpy
	double h;
	//! Compressibility
	double kappa;
	//! Thermal conductivity
	double lambda;
	//! Pressure
	double p;
	//! Phase 
	/*!
	  This phase flag is defined according to the phase flag in Modelica.Media:
	  2 for two-phase, 1 for one-phase.
	*/
	int phase;
	//! Specific entropy
	double s;
//};
} ExternalThermodynamicState;

//class ExternalSaturationProperties{
typedef struct {
public:
	TwoPhaseMediumProperties();
	void initializeFields();
	//! Saturation temperature
	double Tsat;
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
	//! Density at bubble line (for pressure ps)
	double dl;
	//! Density at dew line (for pressure ps)
	double dv;
	//! Specific enthalpy at bubble line (for pressure ps)
	double hl;
	//! Specific enthalpy at dew line (for pressure ps)
	double hv;
	//! Saturation pressure
	double psat;
	//! Surface tension
	double sigma;
	//! Specific entropy at bubble line (for pressure ps)
	double sl;
	//! Specific entropy at dew line (for pressure ps)
	double sv;
//};
} ExternalSaturationProperties;

#endif // TWOPHASEMEDIUMPROPERTIES_H_
