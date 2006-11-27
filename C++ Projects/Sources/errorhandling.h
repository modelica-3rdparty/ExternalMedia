/* *****************************************************************
 * Error handling for external library
 *
 * Errors in the external fluid property library have to be reported
 * to the Modelica layer. This class defines the required interface
 * functions.
 * 
 * Francesco Casella, Christoph Richter Nov 2006
 ********************************************************************/

#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_

#include "include.h"

#ifdef DYMOLA
#ifdef BUILD_LIB
extern "C" {
#include "ModelicaUtilities.h"
}
#endif // BUILD_LIB
#endif

void errorMessage(char *errorMessage);
void warningMessage(char *warningMessage);

#endif // ERRORHANDLING_H_
