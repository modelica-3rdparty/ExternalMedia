#include "errorhandling.h"

#ifdef DYMOLA
#ifdef BUILD_LIB
// This implementation uses the Dymola log and error window to report errors
void errorMessage(char *errorMessage){
	ModelicaError(errorMessage);
}

void warningMessage(char *warningMessage){
	ModelicaMessage(warningMessage);
}
#else
// The Dymola specific implementation does currently not work for dynmic link libraries
void errorMessage(char *errorMessage){
	printf("\a%s\nPress return to end the simulation!\n", errorMessage);
	getchar();
	exit(1);
}

void warningMessage(char *warningMessage){
	strcat(warningMessage, "\n");
	printf(warningMessage);
}
#endif
#else
// This is the default section
// Error and warnings are sent to the standard output
void errorMessage(char *errorMessage){
	printf("\a%s\nPress return to end the simulation!\n", errorMessage);
	getchar();
	exit(1);
}

void warningMessage(char *warningMessage){
	strcat(warningMessage, "\n");
	printf(warningMessage);
}
#endif
