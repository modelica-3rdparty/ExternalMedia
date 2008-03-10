/*!
  \file include.h
  \brief generic include file

  This is a generic include for for the entire ExternalMediaPackage
  project. It defines some important preprocessor variables that might
  have to be changed by the user.

  Uncomment the define directives as appropriate

  Francesco Casella, Christoph Richter, Oct 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/
#ifndef INCLUDE_H_
#define INCLUDE_H_

/********************************************************************
 *                Start of user option selection
 ********************************************************************/

// Selection of Modelica compiler
//! Modelica compiler is Dymola
/*!
  Set this preprocessor variable to 1 if Dymola is the Modelica
  compiler that is going to be used with the compiled library.
  \sa OPEN_MODELICA
*/
#define DYMOLA 1
//! Modelica compiler is OpenModelica
/*!
  Set this preprocessor variable to 1 if OpanModelica is the Modelica
  compiler that is going to be used with the compiled library.
  \sa DYMOLA
*/
#define OPEN_MODELICA 0

// Selection of used external fluid property computation packages.
//! FluidProp solver	
/*!
  Set this preprocessor variable to 1 to include the interface to the
  FluidProp solver developed and maintained by Francesco Casella.
*/
#define FLUIDPROP 0

// Selection of build type for this project
//! Build project into a DLL
/*!
  Set this preprocessor variable to 1 if the project is built into a 
  dynamic link library. This setting influences the error reporting
  mechanism as well as the export statement.
*/
#define BUILD_DLL 1

//! Maximum number of non-overlapping transient medium objects
/*!
  Increase this number if you ran a large model with more than 
  a thousand instances of  ThermodynamicState and SaturationState 
  outside BaseProperties records, and without explicit uniqueID handling
*/
#define MAX_TRANSIENT_MEDIUM 1000

//! Not a number
/*!
  This value is used as not a number value. It can be changed by
  the user if there is a more appropriate value.
*/
#define NAN 0xffffffff
#define ISNAN(x) (x == NAN)

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
