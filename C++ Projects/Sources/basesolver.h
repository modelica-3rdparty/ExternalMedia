#ifndef BASESOLVER_H_
#define BASESOLVER_H_

#include "include.h"
#include "fluidconstants.h"


struct TwoPhaseMediumProperties;
struct FluidConstants;

//! Base solver class.
/*!
  This is the base class for all external solver objects
  (e.g. TestSolver, FluidPropSolver)

  Christoph Richter, Francesco Casella, Sep 2006
*/
class BaseSolver{
public:
	//! Constructor.
	/*!
	  The constructor is copying the medium name, library name and substance name
	  to the locally defined variables.
	*/
	BaseSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	//! Destructor
	/*!
	  The destructor for the base solver if currently not doing anything.
	*/
	virtual ~BaseSolver();

	//! Member function
	/*!
	  This function sets the fluid constants which are defined in the
	  FluidConstants record in Modelica. It should be called when a new
	  solver is created.
	*/
    virtual void setFluidConstants();

	//! Set saturation property function
	/*!
	  This function sets the saturation properties for the given pressure p.
	  The computed values are written to the two phase medium propery struct.
	*/
	virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties);
	//! Set saturation property function
	/*!
	  This function sets the saturation properties for the given temperature T.
	  The computed values are written to the two phase medium propery struct.
	*/
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties);
	//! Set saturation property function
	/*!
	  This function sets the saturation properties for the given pressure p
	  and is desined to be used from within the BaseProperties model in
	  Modelica. The computed values are written to the two phase medium propery 
	  struct.
	*/
	virtual void setSat_p_state(TwoPhaseMediumProperties *const properties);

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
