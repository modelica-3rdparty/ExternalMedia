/* *****************************************************************
 * Implementation of the error reporting functions
 *
 * The actual implementation depends on the selected preprocessor
 * variable defined in include.h
 * 
 * Francesco Casella, Christoph Richter, Nov 2006
 ********************************************************************/

#include "errorhandling.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if (DYMOLA == 1) || (OPENMODELICA == 1)
#if (BUILD_DLL == 0)
// This implementation uses the native Modelica tool log and error window to report errors
void errorMessage(char *errorMessage){
	ModelicaError(errorMessage);
}

void warningMessage(char *warningMessage){
	ModelicaMessage(warningMessage);
}
#else
// The Dymola specific implementation does currently not work for dynmic link libraries
void errorMessage(char *errorMessage){
	printf("\a%s\nPress the Stop button in Dymola to end the simulation!\n", errorMessage);
	getchar();
	exit(1);
}

void warningMessage(char *warningMessage){
	strcat(warningMessage, "\n");
	printf("%s",warningMessage);
}
#endif
#else
// This is the default section
// Error and warnings are sent to the standard output
void errorMessage(char *errorMessage){
	printf("\a%s\nPress the stop button in Dymola to end the simulation!\n", errorMessage);
	getchar();
	exit(1);
}

void warningMessage(char *warningMessage){
	strcat(warningMessage, "\n");
	printf("%s",warningMessage);
}
#endif
