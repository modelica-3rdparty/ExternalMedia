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

#if (EXTERNALMEDIA_MODELICA_ERRORS == 1)
#include "importer.h"
// This implementation uses the native Modelica tool log and error window to report errors
void errorMessage(char *errorMessage){
	IMPORT(void (*)(const char *), ModelicaError);
	ModelicaError(errorMessage);
}

void warningMessage(char *warningMessage){
	IMPORT(void (*)(const char *), ModelicaMessage);
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
	printf("%s\n",warningMessage);
}
#endif
