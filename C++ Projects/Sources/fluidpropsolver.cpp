/* *****************************************************************
 * Implementation of class FluidProp solver
 *
 * Francesco Casella, Christoph Richter, Oct 2006 - Feb 2007
 ********************************************************************/

#include "fluidpropsolver.h"
#include "twophasemediumproperties.h"

#if (FLUIDPROP == 1)
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
	if (isError(ErrorMsg))  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg);
		errorMessage(error);
	}

	// Set SI units 
	FluidProp.SetUnits("SI", " ", " ", " ", &ErrorMsg);
	if (isError(ErrorMsg))  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg);
		errorMessage(error);
	}

	// Set fluid constants
	setFluidConstants();
}

FluidPropSolver::~FluidPropSolver(){
}

void FluidPropSolver::setFluidConstants(){
  string ErrorMsg;

  _fluidConstants.MM = FluidProp.Mmol(&ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute molar mass\n %s\n", ErrorMsg);
	errorMessage(error);
	}

  _fluidConstants.Tc = FluidProp.Tcrit(&ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical temperature\n %s\n", ErrorMsg);
	errorMessage(error);
	}

  _fluidConstants.pc = FluidProp.Pcrit(&ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical pressure\n %s\n", ErrorMsg);
	errorMessage(error);
	}

  _fluidConstants.dc = FluidProp.Density("PT", _fluidConstants.pc, _fluidConstants.Tc, &ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical density\n %s\n", ErrorMsg);
	errorMessage(error);
	}
}

void FluidPropSolver::setSat_p(double &p, TwoPhaseMediumProperties *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables at pressure p and steam quality 0
	FluidProp.AllPropsSat("Pq", p , 0.0, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setSat_p\n %s\n", ErrorMsg);
		errorMessage(error);
	}

    // Fill in the TwoPhaseMedium variables (in SI units)
    properties->ps = p;             // saturation pressure
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
}

void FluidPropSolver::setSat_T(double &T, TwoPhaseMediumProperties *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables at temperature T and steam quality 0
	FluidProp.AllPropsSat("Tq", T , 0.0, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setSat_T\n %s\n", ErrorMsg);
		errorMessage(error);
	}

    // Fill in the TwoPhaseMedium variables (in SI units)
    properties->ps = P_;        // saturation pressure
	properties->Ts = T;			// saturation temperature

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
}

void FluidPropSolver::setSat_p_state(TwoPhaseMediumProperties *const properties){
  // Do nothing: the saturation properties have already been computed by
  // setState and stored in the properties struct
};

// Computes the properties of the state vector *and* the saturation properties at the medium pressure
// for later use by setState_p_state
// Note: the phase input is currently not supported
void FluidPropSolver::setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables
	FluidProp.AllPropsSat("Ph", p , h, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_ph\n %s\n", ErrorMsg);
		errorMessage(error);
	}

    // Fill in the TwoPhaseMedium variables (in SI units)
	// properties->beta = 0;				// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h;					// specific enthalpy
	// properties->kappa = 0;				// compressibility
	properties->p = p;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->T = T_;         		// temperature

	properties->ps = p;				// saturation pressure
	properties->Ts = T_sat_;		// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity

    // set the phase output
	if (phase == 0) {
		properties->phase = (properties->h > properties->hl && 
			                 properties->h < properties->hv &&
							 properties->p < _fluidConstants.pc)  ?  2 : 1;
	} else
		properties->phase = phase;
}

// Computes the properties of the state vector *and* the saturation properties at the medium pressure
// for later use by setState_p_state
// Note: the phase input is currently not supported
void FluidPropSolver::setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables
	FluidProp.AllPropsSat("PT", p , T, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_pT\n %s\n", ErrorMsg);
		errorMessage(error);
	}

    // Fill in the TwoPhaseMedium variables (in SI units)
	// properties->beta = 0;				// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h_;					// specific enthalpy
	// properties->kappa = 0;				// compressibility
	properties->p = p;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->T = T_;         		// temperature

	properties->ps = p;				// saturation pressure
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

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity

	properties->phase = 1;  // Always one-phase with pT inputs
}

// Computes the properties of the state vector *and* the saturation properties at the medium pressure
// for later use by setState_p_state
// Note: the phase input is currently not supported
void FluidPropSolver::setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables
	FluidProp.AllPropsSat("dT", d , T, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_pT\n %s\n", ErrorMsg);
		errorMessage(error);
	}

    // Fill in the TwoPhaseMedium variables (in SI units)
	// properties->beta = 0;				// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h_;					// specific enthalpy
	// properties->kappa = 0;				// compressibility
	properties->p = P_;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->T = T;         		// temperature

	properties->ps = P_;				// saturation pressure
	properties->Ts = T_sat_;		// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity

    // set the phase output
	if (phase == 0) {
		properties->phase = (properties->d < properties->dl && 
			                 properties->d > properties->dv &&
							 properties->T < _fluidConstants.Tc)  ?  2 : 1;
	} else
		properties->phase = phase;
}

// Computes the properties of the state vector *and* the saturation properties at the medium pressure
// for later use by setState_p_state
// Note: the phase input is currently not supported
void FluidPropSolver::setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables
	FluidProp.AllPropsSat("Ps", p , s, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  
		// An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_pT\n %s\n", ErrorMsg);
		errorMessage(error);
	}

    // Fill in the TwoPhaseMedium variables (in SI units)
	// properties->beta = 0;				// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->dd_dp_h = psi_;         // derivative of density by pressure at constant h
	properties->dd_dh_p = ksi_;         // derivative of density by enthalpy at constant p
	properties->h = h_;					// specific enthalpy
	// properties->kappa = 0;				// compressibility
	properties->p = p;					// pressure
	properties->s = s;     		    // specific entropy
	properties->T = T_;         		// temperature

	properties->ps = p;				// saturation pressure
	properties->Ts = T_sat_;		// saturation temperature

	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy

	properties->d_Ts_dp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->d_dl_dp = dd_liq_dP_; // derivative of dls by pressure
	properties->d_dv_dp = dd_vap_dP_; // derivative of dvs by pressure
    properties->d_hl_dp = dh_liq_dP_; // derivative of hls by pressure
	properties->d_hv_dp = dh_vap_dP_; // derivative of hvs by pressure

	properties->eta = eta_;	    // dynamic viscosity
	properties->lambda = lambda_;	// thermal conductivity

    // set the phase output
	if (phase == 0) {
		properties->phase = (properties->h > properties->hl && 
			                 properties->h < properties->hv &&
							 properties->p < _fluidConstants.pc)  ?  2 : 1;
	} else
		properties->phase = phase;
}

void FluidPropSolver::setBubbleState(int phase, TwoPhaseMediumProperties *const properties,
		                             TwoPhaseMediumProperties *const bubbleProperties){
	// Set the bubble state property record based on the original medium 
	// saturation state
    // Change hl a little to guarantee the correct phase, since the phase
    // input is currently not supported
    double hl;
	if (phase == 1)
		// liquid
		hl = properties->hl*(1-1e-6);
	else
		// two-phase
		hl = properties->hl*(1+1e-6);
    // Call setState function
	setState_ph(properties->ps, hl, phase, bubbleProperties);
}

void FluidPropSolver::setDewState(int phase, TwoPhaseMediumProperties *const properties,
		                          TwoPhaseMediumProperties *const dewProperties){
	// Set the dew state property record based on the original medium 
	// saturation state
    // Change hv a little to guarantee the correct phase, since the phase
    // input is currently not supported
    double hv;
	if (phase == 1)
		// vapour
		hv = properties->hv*(1+1e-6);
	else
		// two-phase
		hv = properties->hv*(1-1e-6);
    // Call setState function
	setState_ph(properties->ps, hv, phase, dewProperties);
}

bool FluidPropSolver::isError(string ErrorMsg)
{
  if(ErrorMsg == "No errors")
	  return false;
  else
      return true;
}


#endif // FLUIDPROP == 1
