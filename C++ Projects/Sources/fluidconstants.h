/* *****************************************************************
 * Definition of the FluidConstants struct
 *
 * The FluidConstants struct contains all the constant fluid properties
 * that are returned by the external solver
 * 
 * Francesco Casella, Christoph Richter Nov 2006
 ********************************************************************/

#ifndef FLUIDCONSTANTS_H_
#define FLUIDCONSTANTS_H_

#include "include.h"

struct FluidConstants{
	double MM;		// molar mass
	double pc;		// critical pressure
	double Tc;		// critical temperature
};

#endif /*FLUIDCONSTANTS_H_*/
