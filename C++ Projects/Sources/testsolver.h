#ifndef TESTSOLVER_H_
#define TESTSOLVER_H_

#include "basesolver.h"

//! Test solver class
/*!
  This class defines a dummy solver object, computing properties of
  a fluid roughly resembling warm water at low pressure, without
  the need of any further external code. The class is useful for debugging
  purposes, to test whether the C compiler and the Modelica tools are set
  up correctly before tackling problems with the actual - usually ways more
  complex - external code.

  To instantiate this solver, it is necessary to set the library name package
  constant in Modelica as follows:

  libraryName = "TestMedium";

  Francesco Casella, Christoph Richter, Oct 2006
*/
class TestSolver : public BaseSolver{
public:
	TestSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	~TestSolver();
	virtual void setFluidConstants();

	virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties);
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties);
    virtual void setSat_p_state(TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties);
};

#endif /*TESTSOLVER_H_*/
