#include "mediummap.h"
#include "twophasemedium.h"

//! Add a new medium object to the medium map
/*!
  This function adds a new medium object to the medium map and
  returns its (positive) uniqueID.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
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

//! Add a new transient medium object to the medium map
/*!
  This function adds a new transient medium object to the medium map 
  and returns its (negative) uniqueID.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
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

//! Add the default medium object for the solver to the default solver media map
/*!
  This function adds the default medium object for the specified solver
  to the default solver media map. The default medium object is used for
  function calls without a uniqueID (i.e. all functions returning fluid
  constants, saturationPressure(), saturationTemperature(), etc.).
  @param solverKey Solver key
  @param solver Pointer to solver
*/
void MediumMap::addSolverMedium(const string &solverKey, BaseSolver *const solver){
	// Create new medium
	_solverMediums[solverKey] = new TwoPhaseMedium(solver->mediumName, solver->libraryName, solver->substanceName, solver, 0);
}

//! Change a medium
/*!
  This function replaces the existing pointer to a solver that is part
  of the medium object with a pointer to another solver.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
  @param uniqueID uniqueID of the medium object for which the solver should be changed
*/
void MediumMap::changeMedium(const string &mediumName, const string &libraryName, const string &substanceName, const int &uniqueID){
	// This function changes an existing medium
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
	// Set solver
	_mediums[uniqueID]->setSolver(solver);
}

//! Delete a medium from the map
/*!
  This function deletes a medium object from the medium map. It can
  be called when the Modelica simulation is terminated.
  @param uniqueID UniqueID of the medium to be deleted
*/
void MediumMap::deleteMedium(const int &uniqueID){
	// Delete medium from map
	delete _mediums[uniqueID]; _mediums[uniqueID] = 0;
}

//! Return a pointer to a medium object 
/*!
  This function returns a pointer to a medium object in the medium
  map. The advantage of using this function instead of using the medium map
  directly is that an error reporting mechanism can be implemented that
  avoids crashed due to calls with an invalid uniqueID.
  @param uniqueID UniqueID of the medium object
*/
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

//! Return a pointer to the default medium for the solver
/*!
  This function returns a pointer to a medium object in the solver 
  medium map.
  @param solver Pointer to solver
*/
BaseTwoPhaseMedium *MediumMap::solverMedium(BaseSolver *const solver){
	// Return solver medium for specified solver
	return _solverMediums[SolverMap::solverKey(solver->libraryName, solver->substanceName)];
}

//! Return a pointer to the default medium for the solver
/*!
  This function returns a pointer to a medium object in the solver 
  medium map. It is a convenience function if the pointer to the solver
  is not available. It creates a new solver medium if it does not exist.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
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
