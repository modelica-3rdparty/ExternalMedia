/* *****************************************************************
 * Implementation of class TwoPhaseMedium
 *
 * TwoPhaseMedium is the default object embedding the fluid property
 * computations at a given point of the plant. 
 * 
 * TwoPhaseMedium extends BaseTwoPhaseMedium.
 *
 * See the header file for further documentation
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#include "twophasemedium.h"

// General purpose includes
#include <math.h>

TwoPhaseMedium::TwoPhaseMedium(const string &mediumName, const string &libraryName, 
							   const string &substanceName, BaseSolver *const solver,
							   const int &uniqueID)
	: BaseTwoPhaseMedium(mediumName, libraryName, substanceName, solver, uniqueID){
	// Create new medium property struct
	_properties = new TwoPhaseMediumProperties();
	_properties->mediumName = mediumName;
	_properties->libraryName = libraryName;
	_properties->substanceName = substanceName;
	_properties->uniqueID = uniqueID;
	// Set medium constants
	_solver->setMediumConstants(_properties);
}

TwoPhaseMedium::~TwoPhaseMedium(){
	// Delete medium property struct
	delete _properties; _properties = 0;
}

void TwoPhaseMedium::setSolver(BaseSolver *const solver){
	BaseTwoPhaseMedium::setSolver(solver);
	// Set medium constants
	_solver->setMediumConstants(_properties);
}