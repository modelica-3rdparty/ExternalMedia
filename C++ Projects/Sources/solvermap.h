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
	static BaseSolver *addSolver(const string &mediumName, const string &libraryName, const string &substanceName);

	static string solverKey(const string &libraryName, const string &substanceName);

protected:
	static map<string, BaseSolver*> _solvers;
};

#endif /*SOLVERMAP_H_*/
