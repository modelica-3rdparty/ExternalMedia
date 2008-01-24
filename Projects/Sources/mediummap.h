#ifndef MEDIUMMAP_H_
#define MEDIUMMAP_H_

#include "include.h"
#include "solvermap.h"

class BaseTwoPhaseMedium;

//! Medium map
/*!
  This class is used to hold a static medium map. It stores instances of TwoPhaseMedium
  to avoid the time-consuming recomputation of fluid properties. It uses positive as well
  as negative unique ID number to uniquely identify the instances. The positive unique
  ID numbers are used for instances of the Modelica model BaseProperties. They have
  to be created from within Modelica when the Modelica models are initialized. The negative
  unique ID numbers are also called transient unique IDs and are used when the BaseProperties
  record is not used in Modelica. The maximum number of transient unique IDs is called
  MAX_TRANSIENT_MEDIUM and can be set in include.h.

  Francesco Casella, Christoph Richter, Sep 2006  
  Copyright Politecnico di Milano and TU Braunschweig
*/
class MediumMap{
public:
	static int addMedium(const string &mediumName, const string &libraryName, const string &substanceName);
	static int addTransientMedium(const string &mediumName, const string &libraryName, const string &substanceName);
	static void addSolverMedium(const string &solverKey, BaseSolver *const solver);

	static void changeMedium(const string &mediumName, const string &libraryName, const string &substanceName, const int &uniqueID);
	static void deleteMedium(const int &uniqueID);
	
	static BaseTwoPhaseMedium *medium(const int &uniqueID);
	static BaseTwoPhaseMedium *solverMedium(BaseSolver *const solver);
	static BaseTwoPhaseMedium *solverMedium(const string &mediumName, const string &libraryName, const string &substanceName);

protected:
	//! Static counter for the positive unique ID number used by permanent medium objects
	static int _uniqueID;
	//! Static counter for the negative unique ID number used by transient medium objects
	static int _transientUniqueID;
	//! Map for mediums with unique ID as identifier
	static map<int, BaseTwoPhaseMedium*> _mediums;
	//! Map for mediums that are used for function calls without specified unique ID such as saturationTemperature_() and saturationPressure_()
	static map<string, BaseTwoPhaseMedium*> _solverMediums;
};

#endif // MEDIUMMAP_H_
