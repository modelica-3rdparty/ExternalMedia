/* *****************************************************************
 * Interface of class BaseSolver
 *
 * This is the base class of all the external solver objects
 * (e.g. TestSolver, FluidPropSolver)
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#ifndef BASESOLVER_H_
#define BASESOLVER_H_

#include "include.h"
#include "fluidconstants.h"


struct TwoPhaseMediumProperties;
struct FluidConstants;

class BaseSolver{
public:
	BaseSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	virtual ~BaseSolver();

	// Sets the solver's fluid constants
    virtual void setFluidConstants();

	virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties);
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties);

    double molarMass() const;
	double criticalTemperature() const;
	double criticalPressure() const;
	double criticalDensity() const;
	double criticalEnthalpy() const;
	double criticalEntropy() const;

	// Solver properties
	string mediumName;		// medium name
	string libraryName;		// library name
	string substanceName;	// substance name

protected:
	// Fluid constants
	FluidConstants _fluidConstants;  // fluid constants
};

#endif /*BASESOLVER_H_*/
