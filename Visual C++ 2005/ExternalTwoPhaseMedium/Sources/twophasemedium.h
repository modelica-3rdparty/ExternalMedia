/* *****************************************************************
 * Interface of class TwoPhaseMedium
 *
 * TwoPhaseMedium is the default object embedding the fluid property
 * computations at a given point of the plant. 
 *
 * To test the compiler setup, uncomment the directive
 * #define COMPILER_TEST; the TwoPhaseMedium object will compute
 * dummy properties, without needing to be interfaced with any
 * actual external fluid property computation code.
 *
 * To compile the FluidProp interface, uncomment the directive
 * #define FLUIDPROP.
 *
 * To implement the interface to your own external fluid property
 * computation software, comment the directive
 * #define COMPILER_TEST, and fill in the blanks in the code with 
 * the appropriate function calls to your external code.
*
 * TwoPhaseMedium extends BaseTwoPhaseMedium.
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#ifndef TWOPHASEMEDIUM_H_
#define TWOPHASEMEDIUM_H_

# include "basetwophasemedium.h"  // Base class definition

// Uncomment the directives as needed
// #define COMPILER_TEST
#define FLUIDPROP

#ifdef FLUIDPROP
#include "FluidProp_IF.h"
#endif

class TwoPhaseMedium : public BaseTwoPhaseMedium{
public:
	TwoPhaseMedium(const string &mediumName, 
		           const string &libraryName,
		           const string &substanceName);
	~TwoPhaseMedium();

	void setSat_p(const double &p);
	void setSat_T(const double &T);

	double saturationPressure(const double &T, const string &mediumName);
	double saturationTemperature(const double &p, const string &mediumName);

	void setState_dT(const double &d, const double &T, const int &phase);
	void setState_ph(const double &p, const double &h, const int &phase);
	void setState_ps(const double &p, const double &s, const int &phase);
	void setState_pT(const double &p, const double &T);
private:
#ifdef FLUIDPROP
	CFluidProp *FluidProp;  // Instance of FluidProp wrapper object
#endif
};

#endif /*TWOPHASEMEDIUM_H_*/
