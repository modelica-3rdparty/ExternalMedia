#ifndef BASESOLVER_H_
#define BASESOLVER_H_

#include "include.h"

struct TwoPhaseMediumProperties;

class BaseSolver{
public:
	BaseSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	virtual ~BaseSolver();

	virtual void setMediumConstants(TwoPhaseMediumProperties *const properties);

	virtual void setSat_p(const double &p, TwoPhaseMediumProperties *const properties);
	virtual void setSat_T(const double &T, TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(const double &d, const double &T, const int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ph(const double &p, const double &h, const int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ps(const double &p, const double &s, const int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_pT(const double &p, const double &T, TwoPhaseMediumProperties *const properties);

	// Solver properties
	string mediumName;		// medium name
	string libraryName;		// library name
	string substanceName;	// substance name
};

#endif /*BASESOLVER_H_*/
