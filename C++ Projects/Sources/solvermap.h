/* *****************************************************************
 * Interface of the SolverMap class
 *
 * This class manages the map of all solvers, i.e. wrapper
 * classes derived from BaseSolver and incapsulating the external
 * fluid property computation library. Only one instance is created
 * for each external library.
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#ifndef SOLVERMAP_H_
#define SOLVERMAP_H_

#include "include.h"

class BaseSolver;

class SolverMap{
public:
	// Returns a pointer to the solver corresponding to mediumName, libraryName, substanceName
	// Creates the solver if it doesn't exist yet in the solver map
	static BaseSolver *getSolver(const string &mediumName, const string &libraryName, const string &substanceName);

	// Compute a unique map key from the libraryName and substanceName
	static string solverKey(const string &libraryName, const string &substanceName);

protected:
	static map<string, BaseSolver*> _solvers;
};

#endif /*SOLVERMAP_H_*/
