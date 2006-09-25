/* *****************************************************************
 * Interface of class DummyTwoPhaseMedium
 *
 * DummyTwoPhaseMedium implements a TwoPhaseMedium object
 * with dummy calculations (i.e. it does not need any external
 * fluid property computation software)
 *
 * Can be used to test the compiler setup.
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "twophasemedium.h"  // Base class header

class DummyTwoPhaseMedium : public TwoPhaseMedium{
public:
	DummyTwoPhaseMedium(const string &mediumName);
	~DummyTwoPhaseMedium();

	void setSat_p(const double &p);
	void setSat_T(const double &T);

	double saturationPressure(const double &T, const string &mediumName);
	double saturationTemperature(const double &p, const string &mediumName);

	void setState_dT(const double &d, const double &T, const int &phase);
	void setState_ph(const double &p, const double &h, const int &phase);
	void setState_ps(const double &p, const double &s, const int &phase);
	void setState_pT(const double &p, const double &T);
};