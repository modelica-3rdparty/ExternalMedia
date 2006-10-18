#include "solvermap.h"

#include "basesolver.h"

BaseSolver *SolverMap::addSolver(const string &libraryName, const string &substanceName){
	// Check whether solver already exists
	string solverKey(libraryName + "." + substanceName);
	if (solvers.find(solverKey) != solvers.end()){
		return solvers[solverKey];
	}
	// Create new solver
	
	/* ***************************************************/
	/* This is the place where one would have to specify */
	/* specific solvers! This is not done yet!			 */
	/* ***************************************************/

	solvers[solverKey] = new BaseSolver(libraryName, substanceName);
	return solvers[solverKey];
};

map<string, BaseSolver*> SolverMap::solvers;
