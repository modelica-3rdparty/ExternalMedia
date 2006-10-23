#include "solvermap.h"

#include "basesolver.h"
#include "testsolver.h"
#include "fluidpropsolver.h"

BaseSolver *SolverMap::addSolver(const string &mediumName, const string &libraryName, const string &substanceName){
	// Check whether solver already exists
	string solverKey(libraryName + "." + substanceName);
	if (solvers.find(solverKey) != solvers.end()){
		return solvers[solverKey];
	}
	// Create new solver
	// CompilerTest solver
	if (libraryName.find("TestMedium") == 0)
	  solvers[solverKey] = new TestSolver(mediumName, libraryName, substanceName);
	else if (libraryName.find("FluidProp") == 0)
	  solvers[solverKey] = new FluidPropSolver(mediumName, libraryName, substanceName);
	return solvers[solverKey];
};

map<string, BaseSolver*> SolverMap::solvers;
