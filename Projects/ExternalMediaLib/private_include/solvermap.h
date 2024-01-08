#ifndef SOLVERMAP_H_
#define SOLVERMAP_H_
#include "include.h"
#include <string>
#include <map>

class BaseSolver;

/*! Solver map */
/*!
  This class manages the map of all solvers. A solver is a class that inherits
  from BaseSolver and that interfaces the external fluid property computation
  code. Only one instance is created for each external library.

  Francesco Casella, Christoph Richter, Roberto Bonifetto
  2006-2012
  Copyright Politecnico di Milano, TU Braunschweig, Politecnico di Torino
*/
class SolverMap{
public:
	static BaseSolver *getSolver(const std::string &mediumName, const std::string &libraryName, const std::string &substanceName);
	static std::string solverKey(const std::string &libraryName, const std::string &substanceName);

protected:
   /*! Map for all solver instances identified by the SolverKey */
	static std::map<std::string, BaseSolver*> _solvers;
};

#endif /* SOLVERMAP_H_ */
