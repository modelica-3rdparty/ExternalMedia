#ifndef FLUIDPROPSSOLVER_H_
#define FLUIDPROPSSOLVER_H_

#include "basesolver.h"

class FluidPropsSolver : public BaseSolver{
public:
	FluidPropsSolver(const string &libraryName, const string &substanceName);
	~FluidPropsSolver();
};

#endif /*FLUIDPROPSSOLVER_H_*/
