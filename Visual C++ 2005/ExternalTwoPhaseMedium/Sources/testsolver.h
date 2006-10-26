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
	virtual void setMediumConstants(TwoPhaseMediumProperties *const properties);

	virtual void setSat_p(const double &p, TwoPhaseMediumProperties *const properties);
	virtual void setSat_T(const double &T, TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(const double &d, const double &T, const int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ph(const double &p, const double &h, const int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ps(const double &p, const double &s, const int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_pT(const double &p, const double &T, TwoPhaseMediumProperties *const properties);
};

#endif /*TESTSOLVER_H_*/
