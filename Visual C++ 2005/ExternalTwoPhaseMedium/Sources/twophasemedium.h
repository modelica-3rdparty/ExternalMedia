/* *****************************************************************
 * Interface of class TwoPhaseMedium
 *
 * TwoPhaseMedium is the default object embedding the fluid property
 * computations at a given point of the plant. 
 *
 * TwoPhaseMedium extends BaseTwoPhaseMedium.
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#ifndef TWOPHASEMEDIUM_H_
#define TWOPHASEMEDIUM_H_

# include "basetwophasemedium.h"  // Base class definition

class TwoPhaseMedium : public BaseTwoPhaseMedium{
public:
	TwoPhaseMedium(const string &mediumName);
	~TwoPhaseMedium();

	void setSat_p(const double &p);
	void setSat_T(const double &T);

	double saturationPressure(const double &T, const string &mediumName);
	double saturationTemperature(const double &p, const string &mediumName);

	void setState_dT(const double &d, const double &T, const int &phase);
	void setState_ph(const double &p, const double &h, const int &phase);
	void setState_ps(const double &p, const double &s, const int &phase);
	void setState_pT(const double &p, const double &T);
};

#endif /*TWOPHASEMEDIUM_H_*/