/* *****************************************************************
 * Interface of the base class BaseTwoPhaseMedium.cpp
 *
 * The BaseTwoPhaseMedium class defines all the variables and member
 * functions which are needed to use external Modelica medium models
 * extending from PartialExternalTwoPhaseMedium.
 * 
 * Francesco Casella, Christoph Richter Sep 2006
 ********************************************************************/

#ifndef BASETWOPHASEMEDIUM_H_
#define BASETWOPHASEMEDIUM_H_

#include "include.h"

#include "basesolver.h"
#include "twophasemediumproperties.h"

class BaseTwoPhaseMedium{
public:
	BaseTwoPhaseMedium(const string &mediumName, const string &libraryName, 
		const string &substanceName, BaseSolver *const solver);
	virtual ~BaseTwoPhaseMedium();

	virtual double beta() const;
	virtual double cp() const;
	virtual double cv() const;
	virtual double d() const;
	virtual double dd_dp_h() const;
	virtual double dd_dh_p() const;
	virtual double h() const;
	virtual double kappa() const;
	virtual double p() const;
	virtual double s() const;
	virtual double T() const;

	virtual double ps() const;
	virtual double Ts() const;

	virtual double dl() const;
	virtual double dv() const;
	virtual double hl() const;
	virtual double hv() const;
	virtual double sl() const;
	virtual double sv() const;

	virtual double dc() const;
	virtual double pc() const;
	virtual double Tc() const;

	virtual double MM() const;

	virtual double eta() const;
	virtual double lambda() const;
	virtual double Pr() const;
	virtual double sigma() const;

	virtual void setSat_p(const double &p) = 0;
	virtual void setSat_T(const double &T) = 0;

	virtual double saturationPressure(const double &T, const string &mediumName) = 0;
	virtual double saturationTemperature(const double &p, const string &mediumName) = 0;

	virtual void setState_dT(const double &d, const double &T, const int &phase) = 0;
	virtual void setState_ph(const double &p, const double &h, const int &phase) = 0;
	virtual void setState_ps(const double &p, const double &s, const int &phase) = 0;
	virtual void setState_pT(const double &p, const double &T) = 0;

protected:	
	// Pointer to medium property record
	TwoPhaseMediumProperties *_properties;

	// Pointer to solver
	BaseSolver *_solver;
};

#endif /*BASETWOPHASEMEDIUM_H_*/
