/*!
  \file include.h
  \brief Main include file

  This is a main include file for the entire ExternalMediaPackage
  project. It defines some important preprocessor variables that might
  have to be changed by the user.

  Uncomment the define directives as appropriate

  Francesco Casella, Christoph Richter, Roberto Bonifetto
  2006-2012
  Copyright Politecnico di Milano, TU Braunschweig, Politecnico di Torino
*/
#ifndef INCLUDE_H_
#define INCLUDE_H_

/********************************************************************
 *                Start of user option selection
 ********************************************************************/

// Selection of used external fluid property computation packages.
//! FluidProp solver
/*!
  Set this preprocessor variable to 1 to include the interface to the
  FluidProp solver developed and maintained by Francesco Casella.
*/
#ifndef EXTERNALMEDIA_FLUIDPROP
#define EXTERNALMEDIA_FLUIDPROP 0
#endif

// Selection of used external fluid property computation packages.
//! CoolProp solver
/*!
  Set this preprocessor variable to 1 to include the interface to the
  CoolProp solver developed and maintained by Jorrit Wronski et al.
*/
#ifndef EXTERNALMEDIA_COOLPROP
#define EXTERNALMEDIA_COOLPROP 1
#endif

//! Not a number
/*!
  This value is used as not a number value. It can be changed by
  the user if there is a more appropriate value.
*/
#include <math.h>
#ifndef NAN
#define NAN 0xffffffff
#endif
#ifndef ISNAN
#define ISNAN(x) (x == NAN)
#endif

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
