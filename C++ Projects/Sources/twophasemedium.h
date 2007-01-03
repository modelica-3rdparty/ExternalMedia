#ifndef TWOPHASEMEDIUM_H_
#define TWOPHASEMEDIUM_H_

#include "include.h"
#include "basetwophasemedium.h"

//! Two phase medium
/*!
  This class is the default object embedding the fluid property
  computations at a given point of the plant. 
  
  Christoph Richter, Francesco Casella, Sep 2006
*/
class TwoPhaseMedium : public BaseTwoPhaseMedium{
public:
	TwoPhaseMedium(const string &mediumName, const string &libraryName, 
				   const string &substanceName, BaseSolver *const solver,
				   const int &uniqueID);
	~TwoPhaseMedium();

};

#endif // TWOPHASEMEDIUM_H_
