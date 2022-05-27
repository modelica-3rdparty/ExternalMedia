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

// Provides the IMPORT macro to make Modelica utility functions
// available with a uniform interface for Windows and Linux
#include "importer.h"

void errorMessage(char *errorMessage){
	IMPORT(void (*)(const char *), ModelicaError);
	ModelicaError(errorMessage);
}

void warningMessage(char *warningMessage){
	IMPORT(void (*)(const char *), ModelicaMessage);
	ModelicaMessage(warningMessage);
}
