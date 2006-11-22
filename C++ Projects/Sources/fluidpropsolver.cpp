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
		   psi_, zeta_ , gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_, dd_liq_dP_hL_, dd_liq_dP_h2_, dd_vap_dP_h2_, dd_vap_dP_hV_;

	// Compute all FluidProp variables
	FluidProp.AllPropsSat("Ph", p , h, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, dd_liq_dP_hL_, dd_liq_dP_h2_, dd_vap_dP_h2_, dd_vap_dP_hV_, 
						  &ErrorMsg);

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

	properties->Ts = T_sat_;		// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy
	properties->sl = 0;		// bubble specific entropy
	properties->sv = 0;		// dew specific entropy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure

	properties->d_dl_dP_hL = dd_liq_dP_hL_; // derivative of density by pressure at constant enthalpy, bubble point, liquid side
	properties->d_dl_dP_h2 = dd_liq_dP_h2_; // derivative of density by pressure at constant enthalpy, bubble point, 2-phase side
	properties->d_dv_dP_hV = dd_vap_dP_hV_; // derivative of density by pressure at constant enthalpy, bubble point, vapour side
	properties->d_dv_dP_h2 = dd_vap_dP_h2_; // derivative of density by pressure at constant enthalpy, bubble point, 2-phase side

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
