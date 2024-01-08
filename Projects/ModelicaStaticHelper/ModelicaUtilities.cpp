/* Utility functions which can be called by external Modelica functions.

   These functions are defined in section 12.8.6 of the
   Modelica Specification 3.0 and section 12.9.6 of the
   Modelica Specification 3.1 and later.

   This implements them make it possible to compile tests of ExternalMedia
   without having access to the relevant functions of Modelica.

   Don't use this file for anything other than compiling test targets in ExternalMedia

*/

#include "ModelicaUtilities.h"
#include <stdexcept>


void ModelicaError(const char *string){ throw std::runtime_error(string);}

void ModelicaWarning(const char *string) { throw std::runtime_error(string); }
