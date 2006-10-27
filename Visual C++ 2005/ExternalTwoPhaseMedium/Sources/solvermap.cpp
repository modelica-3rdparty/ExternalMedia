/* *****************************************************************
 * Implementation of the SolverMap class
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "solvermap.h"

#include "basesolver.h"
#include "testsolver.h"
#include "fluidpropsolver.h"

BaseSolver *SolverMap::addSolver(const string &mediumName, const string &libraryName, const string &substanceName){
	// Check whether solver already exists
	string solverKey(libraryName + "." + substanceName);
	if (_solvers.find(solverKey) != _solvers.end())
		return _solvers[solverKey];

	// Create new solver if it doesn't exist

	// CompilerTest solver
	if (libraryName.find("TestMedium") == 0)
	  _solvers[solverKey] = new TestSolver(mediumName, libraryName, substanceName);
#ifdef FLUIDPROP
	else if (libraryName.find("FluidProp") == 0)
	  _solvers[solverKey] = new FluidPropSolver(mediumName, libraryName, substanceName);
#endif // FLUIDPROP
	else
	{
	  // Generate error message
	  char error[100];
	  sprintf(error, "Error: libraryName = %s is not supported by any external solver\n", libraryName.c_str());
	  ERROR_MSG(error);
	}
	return _solvers[solverKey];  
};

map<string, BaseSolver*> SolverMap::_solvers;
