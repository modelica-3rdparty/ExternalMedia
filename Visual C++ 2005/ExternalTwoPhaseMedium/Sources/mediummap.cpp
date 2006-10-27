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

BaseTwoPhaseMedium *MediumMap::medium(const int &uniqueID){
	// Check whether unique ID number is valid
	if (uniqueID > _uniqueID){
		return 0;
	}
	return _mediums[uniqueID];
}

map<int, BaseTwoPhaseMedium*> MediumMap::_mediums;

int MediumMap::_uniqueID(0);
