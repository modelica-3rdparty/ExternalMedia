#ifndef BASESOLVER_H_
#define BASESOLVER_H_

#include "include.h"

class TwoPhaseMedium;

class BaseSolver{
public:
	BaseSolver(const string &libraryName, const string &substanceName);
	virtual ~BaseSolver();

	virtual void setMediumConstants(TwoPhaseMedium *const medium);

	virtual void setSat_p(const double &p, TwoPhaseMedium *const medium);
	virtual void setSat_T(const double &T, TwoPhaseMedium *const medium);

	virtual void setState_dT(const double &d, const double &T, const int &phase, TwoPhaseMedium *const medium);
	virtual void setState_ph(const double &p, const double &h, const int &phase, TwoPhaseMedium *const medium);
	virtual void setState_ps(const double &p, const double &s, const int &phase, TwoPhaseMedium *const medium);
	virtual void setState_pT(const double &p, const double &T, TwoPhaseMedium *const medium);

	// Solver properties
	string libraryName;		// library name
	string substanceName;	// substance name
};

#endif /*BASESOLVER_H_*/
