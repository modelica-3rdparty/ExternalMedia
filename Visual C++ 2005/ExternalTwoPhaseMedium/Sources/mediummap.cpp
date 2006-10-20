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
	BaseSolver *baseSolver = SolverMap::addSolver(mediumName, libraryName, substanceName);
	// Create new medium

	/* ***************************************************/
	/* This is the place where one could specify a more  */
	/* advanced medium extending from BaseTwoPhaseMedium */
	/* ***************************************************/

	mediums[uniqueID] = new TwoPhaseMedium(mediumName, libraryName, substanceName, baseSolver, uniqueID);
	// Return unique ID number
	return uniqueID;
}

BaseTwoPhaseMedium *MediumMap::medium(const int &uniqueID){
	// Check whether unique ID number is valid
	if (uniqueID > MediumMap::uniqueID){
		return 0;
	}
	return mediums[uniqueID];
}

map<int, BaseTwoPhaseMedium*> MediumMap::mediums;

int MediumMap::uniqueID(0);
