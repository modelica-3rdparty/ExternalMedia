/* *****************************************************************
 * Implementation of the error reporting functions
 *
 * The actual implementation depends on the selected preprocessor
 * variable defined in include.h
 *
 * Francesco Casella, Christoph Richter, Nov 2006
 ********************************************************************/

#include "errorhandling.h"
#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef EXTERNALMEDIA_MODELICA_ERRORS
#define EXTERNALMEDIA_MODELICA_ERRORS 1
#endif
// The Dymola specific implementation does currently not work for dynamic link libraries
#if (EXTERNALMEDIA_MODELICA_ERRORS == 1)
extern "C" {
#include "ModelicaUtilities.h"
}
// This implementation uses the native Modelica tool log and error window to report errors
void errorMessage(char *errorMessage){
	ModelicaError(errorMessage);
}

void warningMessage(char *warningMessage){
	ModelicaMessage(warningMessage);
}
#else
// This is the default section
// Error and warnings are sent to the standard output
void errorMessage(char *errorMessage){
	printf("\a%s\nAn error occurred and the calculation cannot continue, please terminate your simulation manually.\n", errorMessage);
	getchar();
	exit(1);
}

void warningMessage(char *warningMessage){
  //	strcat(warningMessage, "\n");
	printf("%s\n",warningMessage);
}
#endif
