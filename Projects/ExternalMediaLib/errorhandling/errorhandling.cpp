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
#include <string>

// Provides the IMPORT macro to make Modelica utility functions
// available with a uniform interface for Windows and Linux
#include "importer.h"

//Temporary workaround because some Modelica tools on Windows just don't export functions
#ifdef WIN32
void (*ModelicaErrorPtr)(const char *) = nullptr;
void (*ModelicaWarningPtr)(const char *) = nullptr;
#endif

void errorMessage(char *errorMsg){
    //Add prefix to help users understand this message comes from externalmedia
    std::string msg="ExternalMedia error: ";
    msg+=errorMsg;
    #ifndef WIN32
    IMPORT(void (*)(const char *), ModelicaError);
    ModelicaError(msg.c_str());
	#else
	// Workaround code for Windows, will be removed when a proper solution to
	// call ModelicaUtilities functions from DLLs is finally delivered
	if(ModelicaErrorPtr)
    // The pointer to ModelicaError has already been initialized by a previous
    // call to setState_xx
	  ModelicaErrorPtr(msg.c_str());
	else
	{
	  // The pointer to ModelicaError is not yet initialized, so all we can do
	  // is to print an error message on the console, and hope that later 
	  // function calls to setState_xx actually triger the proper ModelicaError call
	  printf("%s\nSimulation is not aborted due to current limitations in the implementation\n", msg.c_str());
	}
	#endif
}

void warningMessage(char *warningMsg){
    //Add prefix to help users understand this message comes from externalmedia
    std::string msg="ExternalMedia warning: ";
    msg+=warningMsg;
    #ifndef WIN32
    IMPORT(void (*)(const char *), ModelicaWarning);
    ModelicaWarning(msg.c_str());
	#else
	// Workaround code for Windows, will be removed when a proper solution to
	// call ModelicaUtilities functions from DLLs is finally delivered
	if(ModelicaWarningPtr)
    // The pointer to ModelicaError has already been initialized by a previous
    // call to setState_xx
	  ModelicaWarningPtr(msg.c_str());
	else
	{
	  // The pointer to ModelicaError is not yet initialized, so all we can do
	  // is to print an error message on the console, and hope that later 
	  // function calls to setState_xx actually triger the proper ModelicaError call
	  printf("%s\n", msg.c_str());
	}
	#endif
}
