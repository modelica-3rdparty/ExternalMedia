/* *****************************************************************
 * Class that contains the medium map
 *
 * This class is used to hold a static medium map. One could
 * implement save value return functions within this class.
 * It is currently not checked whether the requested access key
 * uniqueID exists in the map.
 *
 * Christoph Richter, Francesco Casella, Sep 2006
 ********************************************************************/

#ifndef MEDIUMMAP_H_
#define MEDIUMMAP_H_

#include <map>
using std::map;

class TwoPhaseMedium;

class MediumMap{
public:
	static map<int, TwoPhaseMedium*> mediums;
};

#endif /*MEDIUMMAP_H_*/