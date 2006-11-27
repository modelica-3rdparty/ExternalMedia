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
//#define FLUIDPROP

// Select the build type of this project
#define BUILD_DLL
// #define BUILD_LIB

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

// Define export
#if defined BUILD_DLL
#define EXPORT __declspec(dllexport)
#elif defined BUILD_LIB
#define EXPORT __declspec(dllexport)
#else
#define EXPORT __declspec(dllimport)
#endif

#endif /*INCLUDE_H_*/
