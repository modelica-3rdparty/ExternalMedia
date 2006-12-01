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
	BaseSolver *baseSolver = SolverMap::getSolver(mediumName, libraryName, substanceName);

	// Create new medium

	/* ***************************************************/
	/* This is the place where one could specify a more  */
	/* advanced medium extending from BaseTwoPhaseMedium */
	/* ***************************************************/

	_mediums[_uniqueID] = new TwoPhaseMedium(mediumName, libraryName, substanceName, baseSolver, _uniqueID);
	// Return unique ID number
	return _uniqueID;
}

int MediumMap::addTransientMedium(const std::string &mediumName,
      const std::string &libraryName, const std::string &substanceName){
  // Get a new transient unique ID number
  ++_transientUniqueID;
  // Get a pointer to the solver (and create it if it doesn't exist)
  // based on the libraryName, substanceName and possibly mediumName
  // strings
  BaseSolver *baseSolver = SolverMap::getSolver(
                           mediumName, libraryName, substanceName);
  // Create new medium

    /* ***************************************************/
    /* This is the place where one could specify a more  */
    /* advanced medium extending from BaseTwoPhaseMedium */
    /* ***************************************************/
   
   // Only create a new object for the first MAX_TRANSIENT_MEDIUM calls
   // and assign it to a negative entry in the medium map
   if (_transientUniqueID <= MAX_TRANSIENT_MEDIUM)
     _mediums[-(_transientUniqueID)] = new TwoPhaseMedium(mediumName,
                 libraryName, substanceName, baseSolver, _uniqueID);
   // Return the unique ID number of the new object (for the first 
   // MAX_TRANSIENT_MEDIUM instances), or of an old object using a 
   // the map as a circular buffer (for the subsequent ones)
   return -(((_transientUniqueID - 1) % MAX_TRANSIENT_MEDIUM) + 1);
}
void MediumMap::addSolverMedium(const string &solverKey, BaseSolver *const baseSolver){
	// Create new medium
	_solverMediums[solverKey] = new TwoPhaseMedium(baseSolver->mediumName, baseSolver->libraryName, baseSolver->substanceName, baseSolver, -1);
}

void MediumMap::changeMedium(const string &mediumName, const string &libraryName, const string &substanceName, const int &uniqueID){
	// This function changes an existing medium
	BaseSolver *baseSolver = SolverMap::getSolver(mediumName, libraryName, substanceName);

	_mediums[uniqueID]->setSolver(baseSolver);
}

void MediumMap::deleteMedium(const int &uniqueID){
	// Delete medium from map
	delete _mediums[uniqueID]; _mediums[uniqueID] = 0;
}

BaseTwoPhaseMedium *MediumMap::medium(const int &uniqueID){
	// Check whether unique ID number is valid
	// There is some room for improvement
	if (uniqueID > _uniqueID){
		char error[100];
		sprintf(error, "Error: a medium with the specific unique ID %i does not exist!", uniqueID);
		errorMessage(error);
	}
	return _mediums[uniqueID];
}

BaseTwoPhaseMedium *MediumMap::solverMedium(BaseSolver *const solver){
	// Return solver medium for specified solver
	return _solverMediums[SolverMap::solverKey(solver->libraryName, solver->substanceName)];
}

BaseTwoPhaseMedium *MediumMap::solverMedium(const string &mediumName, const string &libraryName, const string &substanceName){
	// Make sure, that solver currently exists
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
	// Return pointer to current solver medium
	return solverMedium(solver);
}

map<int, BaseTwoPhaseMedium*> MediumMap::_mediums;

map<string, BaseTwoPhaseMedium*> MediumMap::_solverMediums;

int MediumMap::_uniqueID(0);
int MediumMap::_transientUniqueID(0);
