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
	// Add a new medium object to the medium map, and return its (positive) uniqueID
	static int addMedium(const string &mediumName, const string &libraryName, const string &substanceName);
	
	// Add a new transient medium object to the medium map, and return its (negative) uniqueID
	static int addTransientMedium(const string &mediumName, const string &libraryName, const string &substanceName);
	
	// Add the default medium object for the solver to the default solver media map
	static void addSolverMedium(const string &solverKey, BaseSolver *const baseSolver);

	// Change a medium
	static void changeMedium(const string &mediumName, const string &libraryName, const string &substanceName, const int &uniqueID);

	// Delete a medium from the map
	static void deleteMedium(const int &uniqueID);
	
	// Return a pointer to the medium object identified by the uniqueID
	static BaseTwoPhaseMedium *medium(const int &uniqueID);

	// Return a pointer to the default medium for the solver
	static BaseTwoPhaseMedium *solverMedium(BaseSolver *const solver);
	static BaseTwoPhaseMedium *solverMedium(const string &mediumName, const string &libraryName, const string &substanceName);

protected:
	// Static integer for the positive unique ID number used by permanent
	// medium objects
	static int _uniqueID;

	// Static integer for the negative unique ID number used by transient
	// medium objects
	static int _transientUniqueID;

	// Map for mediums with unique ID as identifier
	static map<int, BaseTwoPhaseMedium*> _mediums;

	// Map for mediums that are used for function calls without specified unique ID
	// such as saturationTemperature_() and saturationPressure_()
	static map<string, BaseTwoPhaseMedium*> _solverMediums;
};

#endif /*MEDIUMMAP_H_*/
