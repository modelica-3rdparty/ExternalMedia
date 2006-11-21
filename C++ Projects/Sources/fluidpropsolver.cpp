/* *****************************************************************
 * Implementation of class FluidProp solver
 *
 * Francesco Casella, Christoph Richter, Oct 2006
 ********************************************************************/

#include "fluidpropsolver.h"
#include "twophasemediumproperties.h"

#ifdef FLUIDPROP
#define _AFXDLL
FluidPropSolver::FluidPropSolver(const string &mediumName,
								 const string &libraryName,
								 const string &substanceName)
	: BaseSolver(mediumName, libraryName, substanceName){
	string ErrorMsg;
	string Comp[20];
    double Conc[20];

    // Build FluidProp object with the libraryName and substanceName info
	Comp[0] = substanceName.c_str();
    FluidProp.SetFluid(libraryName.substr(libraryName.find(".")+1), 1, Comp, Conc, &ErrorMsg);
	if (ErrorMsg != "No errors")  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[100];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg);
		ERROR_MSG(error);
	}

	// Set SI units 
	FluidProp.SetUnits("SI", " ", " ", " ", &ErrorMsg);
	if (ErrorMsg != "No errors")  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[100];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg);
		ERROR_MSG(error);
	}
}

FluidPropSolver::~FluidPropSolver(){
}

void FluidPropSolver::setMediumConstants(TwoPhaseMediumProperties *const properties){
  string ErrorMsg;

  properties->MM = FluidProp.Mmol(&ErrorMsg);
  if (ErrorMsg != "No errors")  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[100];
	sprintf(error, "FluidProp error: %s\n", ErrorMsg);
	ERROR_MSG(error);
	}

  properties->Tc = FluidProp.Tcrit(&ErrorMsg);
  if (ErrorMsg != "No errors")  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[100];
	sprintf(error, "FluidProp error: %s\n", ErrorMsg);
	ERROR_MSG(error);
	}

  properties->pc = FluidProp.Pcrit(&ErrorMsg);
  if (ErrorMsg != "No errors")  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[100];
	sprintf(error, "FluidProp error: %s\n", ErrorMsg);
	ERROR_MSG(error);
	}
}

void FluidPropSolver::setSat_p(const double &p, TwoPhaseMediumProperties *const properties){
}

void FluidPropSolver::setSat_T(const double &T, TwoPhaseMediumProperties *const properties){
}

void FluidPropSolver::setState_ph(const double &p, const double &h, const int &phase, TwoPhaseMediumProperties *const properties){
	// FluidProp variables (with their default units)
	string ErrorMsg;
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, gamma_, eta_, lambda_;
	// Compute all FluidProp variables
	FluidProp.AllProps( "Ph", p, h, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
                         alpha_, beta_, chi_, fi_, ksi_, psi_, gamma_, eta_, lambda_, &ErrorMsg);
    // Fill in the TwoPhaseMedium variables (in SI units)
	properties->beta = 0;				// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h;					// specific enthalpy
	properties->kappa = 0;				// compressibility
	properties->p = p;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->T = T_;         		// temperature

	properties->ps = 0;		// saturation pressure
	properties->Ts = 0;		// saturation temperature

	properties->dl = 0;		// bubble density
	properties->dv = 0;		// dew density
	properties->hl = 0;		// bubble specific enthalpy
	properties->hv = 0;		// dew specific enthalpy
	properties->sl = 0;		// bubble specific entropy
	properties->sv = 0;		// dew specific entropy

	properties->dc = 0;		// critical density
	properties->pc = 0;		// critical pressure
	properties->Tc = 0;		// critical temperature

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity
	properties->Pr = 0;			// Prandtl number
	properties->sigma = 0;			// surface tension
}

void FluidPropSolver::setState_pT(const double &p, const double &T, TwoPhaseMediumProperties *const properties){
   /// XXX to be completed
}

void FluidPropSolver::setState_dT(const double &d, const double &T, const int &phase, TwoPhaseMediumProperties *const properties){
   /// XXX to be completed
}

void FluidPropSolver::setState_ps(const double &p, const double &s, const int &phase, TwoPhaseMediumProperties *const properties){
   /// XXX to be completed
}

#endif // FLUIDPROP
