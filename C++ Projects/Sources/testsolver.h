/* *****************************************************************
 * Interface of class TestSolver
 *
 * This class defines a dummy solver object, computing properties of
 * a fluid roughly resembling warm water at low pressure, without
 * the need of any further external code. This is useful for debugging
 * purposes, to test if the C compiler and Modelica tools are set correctly, 
 * before tackling problems with the actual external code
 *
 * To instatiate a specific FluidProp fluid, it is necessary to set 
 * the libraryName package constants as follows:
 *
 * libraryName = "TestMedium";
 *
 * See also the solvermap.cpp code
 *
 * Francesco Casella, Christoph Richter, Oct 2006
 ********************************************************************/

#ifndef TESTSOLVER_H_
#define TESTSOLVER_H_

#include "basesolver.h"

class TestSolver : public BaseSolver{
public:
	TestSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	~TestSolver();
	virtual void setFluidConstants();

	virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties);
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties);
};

#endif /*TESTSOLVER_H_*/
