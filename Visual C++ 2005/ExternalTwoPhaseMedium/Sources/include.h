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

/********************************************************************
 *                 End of user option selection
 *            Do not change anything below this line                                    
 ********************************************************************/

// General purpose includes
#include <map>
using std::map;

#include <string>
using std::string;

// Error message generation macro
// Prints an error messages on the console and waits for the user to press 
// the return key - in the future, better logging and graceful simulation
// termination will be supported
#define ERROR_MSG(error_string) {printf("\a%s\nPress return to end the simulation\n", error_string); getchar(); exit(1);}

#endif /*INCLUDE_H_*/
