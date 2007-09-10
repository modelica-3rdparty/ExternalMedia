#ifndef SOLVERMAP_H_
#define SOLVERMAP_H_

#include "include.h"

class BaseSolver;

//! Solver map
/*!
  This class manages the map of all solvers. A solver is a class that inherits
  from BaseSolver and that interfaces the external fluid property computation
  code. Only one instance is created for each external library.

  Christoph Richter, Francesco Casella, Sep 2006
*/
class SolverMap{
public:
	static BaseSolver *getSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	static string solverKey(const string &libraryName, const string &substanceName);

protected:
	//! Map for all solver instances identified by the SolverKey
	static map<string, BaseSolver*> _solvers;
};

#endif // SOLVERMAP_H_
