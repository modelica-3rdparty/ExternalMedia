#include "twophasemedium.h"

#include <math.h>

//! Constructor
/*!
  This constructor is passing the argument to the constructor of the base class
  and is creating a the property container.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
  @param solver Solver
  @param uniqueID Unique ID number
*/
TwoPhaseMedium::TwoPhaseMedium(const string &mediumName, const string &libraryName, 
							   const string &substanceName, BaseSolver *const solver,
							   const int &uniqueID)
	: BaseTwoPhaseMedium(mediumName, libraryName, substanceName, solver, uniqueID){
	// Create new medium property struct
	_properties = new TwoPhaseMediumProperties();
}

//! Destructor
/*!
  The destructor is deleting the property container.
*/
TwoPhaseMedium::~TwoPhaseMedium(){
	// Delete medium property struct
	delete _properties; _properties = 0;
}
