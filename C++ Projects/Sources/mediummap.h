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

//! Medium map
/*!
*/
class MediumMap{
public:
	//! Add a new medium object to the medium map
	/*!
	  This function adds a new medium object to the medium map and
	  returns its (positive) uniqueID.
	  @param mediumName Medium name
	  @param libraryName Library name
	  @param substanceName Substance name
	*/
	static int addMedium(const string &mediumName, const string &libraryName, const string &substanceName);
	
	//! Add a new transient medium object to the medium map
	/*!
	  This function adds a new transient medium object to the medium map 
	  and returns its (negative) uniqueID.
	  @param mediumName Medium name
	  @param libraryName Library name
	  @param substanceName Substance name
	*/
	static int addTransientMedium(const string &mediumName, const string &libraryName, const string &substanceName);
	
	//! Add the default medium object for the solver to the default solver media map
	/*!
	  This function adds the default medium object for the specified solver
	  to the default solver media map. The default medium object is used for
	  function calls without a uniqueID (i.e. all functions returning fluid
	  constants, saturationPressure(), saturationTemperature(), etc.).
	  @param solverKey Solver key
	  @param solver Pointer to solver
	*/
	static void addSolverMedium(const string &solverKey, BaseSolver *const solver);

	//! Change a medium
	/*!
	  This function replaces the existing pointer to a solver that is part
	  of the medium object with a pointer to another solver.
	  @param mediumName Medium name
	  @param libraryName Library name
	  @param substanceName Substance name
	  @param uniqueID uniqueID of the medium object for which the solver should be changed
	*/
	static void changeMedium(const string &mediumName, const string &libraryName, const string &substanceName, const int &uniqueID);

	//! Delete a medium from the map
	/*!
	  This function deletes a medium object from the medium map. It can
	  be called when the Modelica simulation is terminated.
	  @param uniqueID UniqueID of the medium to be deleted
	*/
	static void deleteMedium(const int &uniqueID);
	
	//! Return a pointer to a medium object 
	/*!
	  This function returns a pointer to a medium object in the medium
	  map. The advantage of using this function instead of using the medium map
	  directly is that an error reporting mechanism can be implemented that
	  avoids crashed due to calls with an invalid uniqueID.
	  @param uniqueID UniqueID of the medium object
	*/
	static BaseTwoPhaseMedium *medium(const int &uniqueID);

	//! Return a pointer to the default medium for the solver
	/*!
	  This function returns a pointer to a medium object in the solver 
	  medium map.
	  @param solver Pointer to solver
	*/
	static BaseTwoPhaseMedium *solverMedium(BaseSolver *const solver);
	//! Return a pointer to the default medium for the solver
	/*!
	  This function returns a pointer to a medium object in the solver 
	  medium map. It is a convenience function if the pointer to the solver
	  is not available. It creates a new solver medium if it does not exist.
	  @param mediumName Medium name
	  @param libraryName Library name
	  @param substanceName Substance name
	*/
	static BaseTwoPhaseMedium *solverMedium(const string &mediumName, const string &libraryName, const string &substanceName);

protected:
	//! Static integer for the positive unique ID number used by permanent medium objects
	static int _uniqueID;

	//! Static integer for the negative unique ID number used by transient medium objects
	static int _transientUniqueID;

	//! Map for mediums with unique ID as identifier
	static map<int, BaseTwoPhaseMedium*> _mediums;

	//! Map for mediums that are used for function calls without specified unique ID such as saturationTemperature_() and saturationPressure_()
	static map<string, BaseTwoPhaseMedium*> _solverMediums;
};

#endif /*MEDIUMMAP_H_*/
