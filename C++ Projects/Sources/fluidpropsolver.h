/* *****************************************************************
 * Interface of class FluidPropSolver
 *
 * This class defines a solver object encapsulating a FluidProp object
 *
 * The class will work if FluidProp is correctly installed, and if
 * the following files, defining the CFluidProp object, are included 
 * in the C project:
 *   - FluidProp_IF.h
 *   - FluidProp_IF.cpp
 *   - FluidProp_COM.h
 *
 * Currently, only Microsoft Visual Studio .NET (2002) is supported.
 * It should be used both to compile the ExternalTwoPhaseMedium.lib
 * library, and to compile the dymosim.exe Dymola executable.
 *
 * To instatiate a specific FluidProp fluid, it is necessary to set 
 * the libraryName and substanceNames package constants as in the
 * following example:
 *
 * libraryName = "FluidProp.RefProp";
 * substanceNames = {"H2O"};
 *
 * Instead of RefProp, it is possible to indicate TPSI, StanMix, etc.
 * Instead of H2O, it is possible to indicate any supported substance
 *
 * See also the solvermap.cpp code
 *
 * Francesco Casella, Christoph Richter, Oct 2006 - Feb 2007
 ********************************************************************/

#ifndef FLUIDPROPSOLVER_H_
#define FLUIDPROPSOLVER_H_

#include "basesolver.h"

#if (FLUIDPROP == 1)

#include "FluidProp_IF.h"

class FluidPropSolver : public BaseSolver{
public:
	FluidPropSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	~FluidPropSolver();
	virtual void setFluidConstants();

	virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties);
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties);
    virtual void setSat_p_state(TwoPhaseMediumProperties *const properties);

	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties);
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties);
	virtual void setBubbleState(int phase, TwoPhaseMediumProperties *const properties,
		                        TwoPhaseMediumProperties *const bubbleProperties);
	virtual void setDewState(int phase, TwoPhaseMediumProperties *const properties,
		                     TwoPhaseMediumProperties *const bubbleProperties);

protected:
    TFluidProp FluidProp;  // Instance of FluidProp wrapper object
	bool isError(string ErrorMsg);
};

#endif // FLUIDPROP == 1

#endif /*FLUIDPROPSOLVER_H_*/
