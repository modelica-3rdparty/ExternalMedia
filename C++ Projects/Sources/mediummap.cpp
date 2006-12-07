/* *****************************************************************
 * Implementation of class MediumMap
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "mediummap.h"

#include "twophasemedium.h"

int MediumMap::addMedium(const string &mediumName, const string &libraryName, const string &substanceName){
	// Increase unique ID number
	++_uniqueID;
	// Get a pointer to the solver (and create it if it doesn't exist)
	// based on the libraryName, substanceName and possibly mediumName strings
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
	// Create new medium
	/* ***************************************************/
	/* This is the place where one could specify a more  */
	/* advanced medium extending from BaseTwoPhaseMedium */
	/* ***************************************************/
	_mediums[_uniqueID] = new TwoPhaseMedium(mediumName, libraryName, substanceName, solver, _uniqueID);
	// Return unique ID number
	return _uniqueID;
}

int MediumMap::addTransientMedium(const string &mediumName, const string &libraryName, const string &substanceName){
    // transientUniqueID wrapped to stay within the finite number of
	// available objects in the map.
	// -1 >= transientUniqueID >= -MAX_TRANSIENT_MEDIUM
	int transientUniqueIDWrapped;

	// Get a new transient unique ID number
	++_transientUniqueID;
    transientUniqueIDWrapped = -(((_transientUniqueID - 1) % MAX_TRANSIENT_MEDIUM) + 1);
	// Get a pointer to the solver (and create it if it doesn't exist)
	// based on the libraryName, substanceName and possibly mediumName
	// strings
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);

	// Create new medium
	/* ***************************************************/
	/* This is the place where one could specify a more  */
	/* advanced medium extending from BaseTwoPhaseMedium */
	/* ***************************************************/
	if (_transientUniqueID <= MAX_TRANSIENT_MEDIUM)
  	  // For the first MAX_TRANSIENT_MEDIUM calls, create a new object
	  // and assign it to a negative entry in the medium map
	  _mediums[transientUniqueIDWrapped] = new TwoPhaseMedium(
	     mediumName, libraryName, substanceName, solver, transientUniqueIDWrapped);
	else
 	  // For the subsequent ones, re-initialize the properties of
	  // an already existing object in the medium map, using the wrapped uniqueID
	  _mediums[transientUniqueIDWrapped]->reinitMedium(
	     mediumName, libraryName, substanceName, solver, transientUniqueIDWrapped);

	// Return the unique ID number of the new object
	return transientUniqueIDWrapped;
}
void MediumMap::addSolverMedium(const string &solverKey, BaseSolver *const solver){
	// Create new medium
	_solverMediums[solverKey] = new TwoPhaseMedium(solver->mediumName, solver->libraryName, solver->substanceName, solver, 0);
}

void MediumMap::changeMedium(const string &mediumName, const string &libraryName, const string &substanceName, const int &uniqueID){
	// This function changes an existing medium
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
	// Set solver
	_mediums[uniqueID]->setSolver(solver);
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
