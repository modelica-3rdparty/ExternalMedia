/* *****************************************************************
 * Implementation of class FluidProp solver
 *
 * Francesco Casella, Christoph Richter, Roberto Bonifetto
 * 2006 - 2012
 ********************************************************************/

#include "fluidpropsolver.h"

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
		sprintf(error, "FluidProp error: %s\n", ErrorMsg.c_str());
		errorMessage(error);
	}

	// Set SI units 
	FluidProp.SetUnits("SI", " ", " ", " ", &ErrorMsg);
	if (isError(ErrorMsg))  // An error occurred
	{
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error: %s\n", ErrorMsg.c_str());
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
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute molar mass\n %s\n", ErrorMsg.c_str());
	errorMessage(error);
	}

  _fluidConstants.Tc = FluidProp.Tcrit(&ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical temperature\n %s\n", ErrorMsg.c_str());
	errorMessage(error);
	}

  _fluidConstants.pc = FluidProp.Pcrit(&ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical pressure\n %s\n", ErrorMsg.c_str());
	errorMessage(error);
	}

  _fluidConstants.dc = FluidProp.Density("PT", _fluidConstants.pc, _fluidConstants.Tc, &ErrorMsg);
  if (isError(ErrorMsg))  // An error occurred
	{
	// Build error message and pass it to the Modelica environment
	char error[300];
	sprintf(error, "FluidProp error in FluidPropSolver::setFluidConstants: can't compute critical density\n %s\n", ErrorMsg.c_str());
	errorMessage(error);
	}
}

void FluidPropSolver::setSat_p(double &p, ExternalSaturationProperties *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables at pressure p and steam quality 0
	FluidProp.AllPropsSat("Pq", p , 0.0, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setSat_p(%f)\n %s\n", p, ErrorMsg.c_str());
		errorMessage(error);
	}

    // Fill in the ExternalSaturationProperties variables (in SI units)
	properties->Tsat = T_sat_;		// saturation temperature
	properties->dTp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->ddldp = dd_liq_dP_; // derivative of dls by pressure
	properties->ddvdp = dd_vap_dP_; // derivative of dvs by pressure
    properties->dhldp = dh_liq_dP_; // derivative of hls by pressure
	properties->dhvdp = dh_vap_dP_; // derivative of hvs by pressure
	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy
    properties->psat = p;             // saturation pressure

}

void FluidPropSolver::setSat_T(double &T, ExternalSaturationProperties *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_,
		   d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
		   dh_vap_dP_, dT_sat_dP_;

	// Compute all FluidProp variables at temperature T and steam quality 0
	FluidProp.AllPropsSat("Tq", T , 0.0, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                  alpha_, beta_, chi_, fi_, ksi_, psi_, zeta_, theta_, kappa_, gamma_, eta_, lambda_,  
	    			      d_liq_, d_vap_, h_liq_, h_vap_, T_sat_, dd_liq_dP_, dd_vap_dP_, dh_liq_dP_, 
						  dh_vap_dP_, dT_sat_dP_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setSat_T(%f)\n %s\n", T, ErrorMsg.c_str());
		errorMessage(error);
	}

    // Fill in the ExternalSaturationProperties variables (in SI units)
	properties->Tsat = T;			// saturation temperature
	properties->dTp = dT_sat_dP_;  // derivative of Ts by pressure
	properties->ddldp = dd_liq_dP_; // derivative of dls by pressure
	properties->ddvdp = dd_vap_dP_; // derivative of dvs by pressure
    properties->dhldp = dh_liq_dP_; // derivative of hls by pressure
	properties->dhvdp = dh_vap_dP_; // derivative of hvs by pressure
	properties->dl = d_liq_;	// bubble density
	properties->dv = d_vap_;	// dew density
	properties->hl = h_liq_;	// bubble specific enthalpy
	properties->hv = h_vap_;	// dew specific enthalpy
    properties->psat = P_;        // saturation pressure

}

//! Computes the properties of the state vector from p and h
/*! Note: the phase input is currently not supported according to the standard, 
    the phase input is returned in the state record
*/
void FluidPropSolver::setState_ph(double &p, double &h, int &phase, ExternalThermodynamicState *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_;

	// Compute all FluidProp variables
	FluidProp.AllProps("Ph", p , h, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                 alpha_, beta_, chi_, fi_, ksi_, psi_, 
						 zeta_, theta_, kappa_, gamma_, eta_, lambda_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_ph(%f, %f)\n %s\n", p, h, ErrorMsg.c_str());
		errorMessage(error);
	}

    // Fill in the ExternalThermodynamicState variables (in SI units)
	properties->T = T_;         		// temperature
    properties->a = c_;					// speed of sound
	properties->beta = theta_;			// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->ddhp = ksi_;            // derivative of density by enthalpy at constant p
	properties->ddph = psi_;            // derivative of density by pressure at constant h
	properties->eta = eta_;				// dynamic viscosity
	properties->h = h;					// specific enthalpy
	properties->kappa = kappa_;			// compressibility
	properties->lambda = lambda_;		// thermal conductivity
	properties->p = p;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->phase = phase;          // phase

}

//! Computes the properties of the state vector from p and T
void FluidPropSolver::setState_pT(double &p, double &T, ExternalThermodynamicState *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_;
	
	// Compute all FluidProp variables
	FluidProp.AllProps("PT", p , T, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                 alpha_, beta_, chi_, fi_, ksi_, psi_,
						 zeta_, theta_, kappa_, gamma_, eta_, lambda_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_pT(%f, %f)\n %s\n", p, T, ErrorMsg.c_str());
		errorMessage(error);
	}

	// Fill in the ExternalThermodynamicState variables (in SI units)
	properties->T = T_;         		// temperature
    properties->a = c_;					// speed of sound
	properties->beta = theta_;			// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->ddhp = ksi_;			// derivative of density by enthalpy at constant p
	properties->ddph = psi_;			// derivative of density by pressure at constant h
	properties->eta = eta_;				// dynamic viscosity
	properties->h = h_;					// specific enthalpy
	properties->kappa = kappa_;			// compressibility
	properties->lambda = lambda_;		// thermal conductivity
	properties->p = p;					// pressure
	properties->s = s_;     		    // specific entropy
	properties->phase = 1;		        // Always one-phase with pT inputs
}

// Computes the properties of the state vector from d and T
/*! Note: the phase input is currently not supported according to the standard, 
    the phase input is returned in the state record
*/
void FluidPropSolver::setState_dT(double &d, double &T, int &phase, ExternalThermodynamicState *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_;

	// Compute all FluidProp variables
	FluidProp.AllProps("Td", T , d, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                 alpha_, beta_, chi_, fi_, ksi_, psi_,
						 zeta_, theta_, kappa_, gamma_, eta_, lambda_, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_dT(%f, %f)\n %s\n", d, T, ErrorMsg.c_str());
		errorMessage(error);
	}

	// Fill in the ExternalThermodynamicState variables (in SI units)
	properties->T = T;         			// temperature
    properties->a = c_;					// speed of sound
	properties->beta = theta_;			// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d;				    // density
	properties->ddhp = ksi_;			// derivative of density by enthalpy at constant p
	properties->ddph = psi_;			// derivative of density by pressure at constant h
	properties->eta = eta_;				// dynamic viscosity
	properties->h = h_;					// specific enthalpy
	properties->kappa = kappa_;			// compressibility
	properties->lambda = lambda_;		// thermal conductivity
	properties->p = P_;					// pressure
	properties->s = s_;     		    // specific entropy
 	properties->phase = phase;			// phase
}

//! Computes the properties of the state vector from p and s
/*! Note: the phase input is currently not supported according to the standard, 
    the phase input is returned in the state record
*/
void FluidPropSolver::setState_ps(double &p, double &s, int &phase, ExternalThermodynamicState *const properties){
	string ErrorMsg;
	// FluidProp variables (in SI units)
    double P_, T_, v_, d_, h_, s_, u_, q_, x_[20], y_[20], 
		   cv_, cp_, c_, alpha_, beta_, chi_, fi_, ksi_,
		   psi_, zeta_ , theta_, kappa_, gamma_, eta_, lambda_;
	
	// Compute all FluidProp variables
	FluidProp.AllProps("Ps", p , s, P_, T_, v_, d_, h_, s_, u_, q_, x_, y_, cv_, cp_, c_,
		                 alpha_, beta_, chi_, fi_, ksi_, psi_,
						 zeta_, theta_, kappa_, gamma_, eta_, lambda_, &ErrorMsg);
	if (isError(ErrorMsg)) {  
		// An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::setState_ps(%f, %f)\n %s\n", p, s, ErrorMsg.c_str());
		errorMessage(error);
	}

	// Fill in the ExternalThermodynamicState variables (in SI units)
	properties->T = T_;         		// temperature
    properties->a = c_;					// speed of sound
	properties->beta = theta_;			// isothermal expansion coefficient
	properties->cp = cp_; 		        // specific heat capacity cp
	properties->cv = cv_;		        // specific heat capacity cv
	properties->d = d_;				    // density
	properties->ddhp = ksi_;			// derivative of density by enthalpy at constant p
	properties->ddph = psi_;			// derivative of density by pressure at constant h
	properties->eta = eta_;				// dynamic viscosity
	properties->h = h_;					// specific enthalpy
	properties->kappa = kappa_;			// compressibility
	properties->lambda = lambda_;		// thermal conductivity
	properties->p = p;					// pressure
	properties->s = s;     			    // specific entropy
	properties->phase = phase;			// phase
}

//! Compute isentropic enthalpy
/*!
  This function returns the enthalpy at pressure p after an isentropic
  transformation from the state specified by the properties input

  @param p New pressure
  @param properties ExternalThermodynamicState property record corresponding to current state
*/
double FluidPropSolver::isentropicEnthalpy(double &p, ExternalThermodynamicState *const properties){
	string ErrorMsg;
    double h;

	h = FluidProp.Enthalpy("Ps", p , properties->s, &ErrorMsg);
	if (isError(ErrorMsg)) {  // An error occurred
		// Build error message and pass it to the Modelica environment
		char error[300];
		sprintf(error, "FluidProp error in FluidPropSolver::isentropicEnthalpy(%f, %f)\n %s\n", p, properties->s, ErrorMsg.c_str());
		errorMessage(error);
	}
	return h;
}

//! Check if FluidProp returned an error
bool FluidPropSolver::isError(string ErrorMsg)
{
  if(ErrorMsg == "No errors")
	  return false;
  else
      return true;
}


#endif // FLUIDPROP == 1
