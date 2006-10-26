/* *****************************************************************
 * Interface of the MediumMap class
 *
 * This class is used to hold a static medium map. Each entry
 * corresponds to a different integer uniqueID. One could
 * implement save value return functions within this class.
 * It is currently not checked whether the requested access key
 * uniqueID exists in the map.
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#ifndef MEDIUMMAP_H_
#define MEDIUMMAP_H_

#include "include.h"

#include "solvermap.h"

class BaseTwoPhaseMedium;

class MediumMap{
public:
	static map<int, BaseTwoPhaseMedium*> mediums;

	static int addMedium(const string &mediumName, const string &libraryName, const string &substanceName);
	static BaseTwoPhaseMedium *medium(const int &uniqueID);

protected:
	static int uniqueID;
};

#endif /*MEDIUMMAP_H_*/