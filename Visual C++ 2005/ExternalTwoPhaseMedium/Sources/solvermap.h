#ifndef SOLVERMAP_H_
#define SOLVERMAP_H_

#include "include.h"

class BaseSolver;

class SolverMap{
public:
	static map<string, BaseSolver*> solvers;

	static BaseSolver *addSolver(const string &libraryName, const string &substanceName);
};

#endif /*SOLVERMAP_H_*/
