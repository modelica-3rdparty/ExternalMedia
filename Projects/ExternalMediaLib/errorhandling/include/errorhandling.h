/*!
  \file errorhandling.h
  \brief Error handling for external library

  Errors in the external fluid property library have to be reported
  to the Modelica layer. This class defines the required interface
  functions.

  Francesco Casella, Christoph Richter, Nov 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/

#ifndef ERRORHANDLING_H_
#define ERRORHANDLING_H_

#ifdef WIN32
extern void (*ModelicaErrorPtr)(const char *);
extern void (*ModelicaWarningPtr)(const char *);
#endif

/*! Function to display error message */
/*!
  Calling this function will display the specified error message and will
  terminate the simulation.
  @param errorMessage Error message to be displayed
*/
void errorMessage(char const*errorMsg);
/*! Function to display warning message */
/*!
  Calling this function will display the specified warning message.
  @param warningMessage Warning message to be displayed
*/
void warningMessage(char const *warningMsg);

#endif /* ERRORHANDLING_H_ */
