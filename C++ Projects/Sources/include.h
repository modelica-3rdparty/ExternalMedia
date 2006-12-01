/* *****************************************************************
 * Generic include file for the whole ExternalMediaPackage project
 *
 * Uncomment the #define directives as appropriate
 *
 * Christoph Richter, Francesco Casella, Oct 2006
 ********************************************************************/

#ifndef INCLUDE_H_
#define INCLUDE_H_

/********************************************************************
 *                Start of user option selection
 ********************************************************************/

// Selection of Modelica compiler 
#define DYMOLA
// #define OPEN_MODELICA

// Selection of used external fluid property computation packages
#define FLUIDPROP

// Select the build type of this project
#define BUILD_DLL
// #define BUILD_LIB

// Maximum number of non-overlapping transient medium objects
// Increase this number if you ran a large model with more than 
// a thousand instances of  ThermodynamicState and SaturationState 
// outside BaseProperties records, and without explicit uniqueID handling
#define MAX_TRANSIENT_MEDIUM 1000

/********************************************************************
 *                 End of user option selection
 *            Do not change anything below this line                                    
 ********************************************************************/

// General purpose includes
#include <map>
using std::map;

#include <string>
using std::string;

// Include error handling
#include "errorhandling.h"

#endif /*INCLUDE_H_*/
