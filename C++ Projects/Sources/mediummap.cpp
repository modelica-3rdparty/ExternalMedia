/* *****************************************************************
 * Implementation of class MediumMap
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "mediummap.h"

#include "twophasemedium.h"

int MediumMap::addMedium(const std::string &mediumName, const std::string &libraryName, const std::string &substanceName){
	// Increase unique ID number
	++_uniqueID;
	// Get a pointer to the solver (and create it if it doesn't exist)
	// based on the libraryName, substanceName and possibly mediumName strings
	BaseSolver *baseSolver = SolverMap::addSolver(mediumName, libraryName, substanceName);

	// Create new medium

	/* ***************************************************/
	/* This is the place where one could specify a more  */
	/* advanced medium extending from BaseTwoPhaseMedium */
	/* ***************************************************/

	_mediums[_uniqueID] = new TwoPhaseMedium(mediumName, libraryName, substanceName, baseSolver, _uniqueID);
	// Return unique ID number
	return _uniqueID;
}

void MediumMap::addSolverMedium(const string &solverKey, BaseSolver *const baseSolver){
	// Create new medium
	_solverMediums[solverKey] = new TwoPhaseMedium(baseSolver->mediumName, baseSolver->libraryName, baseSolver->substanceName, baseSolver, -1);
}

BaseTwoPhaseMedium *MediumMap::medium(const int &uniqueID){
	// Check whether unique ID number is valid
	// This check is not complete and will basically make a couple of functions
	// fail is a 0 is returned. There is some room for improvements.
	if (uniqueID > _uniqueID)
		return 0;
	return _mediums[uniqueID];
}

BaseTwoPhaseMedium *MediumMap::solverMedium(const string &solverKey){
	return _solverMediums[solverKey];
}

map<int, BaseTwoPhaseMedium*> MediumMap::_mediums;

map<string, BaseTwoPhaseMedium*> MediumMap::_solverMediums;

int MediumMap::_uniqueID(0);
