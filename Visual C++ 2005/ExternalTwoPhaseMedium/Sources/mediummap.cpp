/* *****************************************************************
 * Implementation of class MediumMap
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "mediummap.h"

#include "twophasemedium.h"

int MediumMap::addMedium(const std::string &mediumName, const std::string &libraryName, const std::string &substanceName){
	// Increase unique ID number
	++uniqueID;
	// Check whether solver for specified medium already exists
	BaseSolver *baseSolver = SolverMap::addSolver(libraryName, substanceName);
	// Create new medium data
	mediums[uniqueID] = new TwoPhaseMedium(mediumName, libraryName, substanceName, baseSolver);
	// Return unique ID number
	return uniqueID;
}

TwoPhaseMedium *MediumMap::medium(const int &uniqueID){
	// Check whether unique ID number is valid
	if (uniqueID > MediumMap::uniqueID){
		return new TwoPhaseMedium("", "", "", 0);
	}
	return mediums[uniqueID];
}

map<int, TwoPhaseMedium*> MediumMap::mediums;

int MediumMap::uniqueID(0);
