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
