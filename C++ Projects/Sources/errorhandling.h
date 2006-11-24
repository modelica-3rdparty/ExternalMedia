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
