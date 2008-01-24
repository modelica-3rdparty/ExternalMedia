#ifndef TWOPHASEMEDIUM_H_
#define TWOPHASEMEDIUM_H_

#include "include.h"
#include "basetwophasemedium.h"

//! Two phase medium
/*!
  This class is the default object embedding the fluid property
  computations at a given point of the plant. 
  
  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/
class TwoPhaseMedium : public BaseTwoPhaseMedium{
public:
	TwoPhaseMedium(const string &mediumName, const string &libraryName, 
				   const string &substanceName, BaseSolver *const solver,
				   const int &uniqueID);
	~TwoPhaseMedium();

};

#endif // TWOPHASEMEDIUM_H_
