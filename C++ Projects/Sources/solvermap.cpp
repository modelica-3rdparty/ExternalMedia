/* *****************************************************************
 * Implementation of the SolverMap class
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "solvermap.h"

#include "basesolver.h"
#include "mediummap.h"
#include "testsolver.h"

#ifdef FLUIDPROP
#include "fluidpropsolver.h"
#endif // FLUIDPROP

BaseSolver *SolverMap::getSolver(const string &mediumName, const string &libraryName, const string &substanceName){
	// Get solver key from library and substance name
	string solverKey(solverKey(libraryName, substanceName));
	// Check whether solver already exists
	if (_solvers.find(solverKey) != _solvers.end())
		return _solvers[solverKey];
	// Create new solver if it doesn't exist
	// CompilerTest solver
	if (libraryName.compare("TestMedium") == 0)
	  _solvers[solverKey] = new TestSolver(mediumName, libraryName, substanceName);
#ifdef FLUIDPROP
	else if (libraryName.compare("FluidProp") == 0)
	  _solvers[solverKey] = new FluidPropSolver(mediumName, libraryName, substanceName);
#endif // FLUIDPROP
	else {
	  // Generate error message
	  char error[100];
	  sprintf(error, "Error: libraryName = %s is not supported by any external solver\n", libraryName.c_str());
	  errorMessage(error);
	}
	// Create new medium object for function calls without specified unique ID
	MediumMap::addSolverMedium(solverKey, _solvers[solverKey]);
	// Return pointer to solver
	return _solvers[solverKey];  
};

string SolverMap::solverKey(const string &libraryName, const string &substanceName){
	// This function returns the solver key and may be changed by advanced users
	return libraryName + "." + substanceName;
}

map<string, BaseSolver*> SolverMap::_solvers;
