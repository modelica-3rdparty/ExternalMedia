#include "coolpropsolver.h"
#include "CoolPropTools.h"
#include "CoolPropLib.h"
#include "CoolProp.h"
#include "AbstractState.h"
#include <iostream>
#include <string>
#include <stdlib.h>

//double _p_eps   ; // relative tolerance margin for subcritical pressure conditions
//double _T_eps   ; // relative tolerance margin for supercritical temperature conditions
//double _delta_h ; // delta_h for one-phase/two-phase discrimination
//ExternalSaturationProperties *_satPropsClose2Crit; // saturation properties close to  critical conditions

CoolPropSolver::CoolPropSolver(const std::string &mediumName, const std::string &libraryName, const std::string &substanceName)
	: BaseSolver(mediumName, libraryName, substanceName){

	// Fluid name can be used to pass in other parameters.
	// The string can be composed like "Propane|enable_TTSE=1|calc_transport=0"
	std::vector<std::string> name_options = strsplit(substanceName,'|');
	std::vector<std::string> library_options = strsplit(libraryName,'|');

	// Set the defaults
	enable_TTSE     = false;
	enable_BICUBIC  = false;
	debug_level     = 0;
	calc_transport  = true;
	extend_twophase = true;
	twophase_derivsmoothing_xend = 0;
	rho_smoothing_xend = 0;

	//Check if a backend has been added to the fluid name (ex: REFPROP::Propane)
    std::string backend;
    CoolProp::extract_backend(name_options[0], backend, this->substanceName);
    
    // Set the default composition
    std::vector<double> fractions(1, 1.0);
    const std::vector<double> *fractions_ptr = NULL;
    this->substanceName = CoolProp::extract_fractions(this->substanceName, fractions);
    fractions_ptr = &fractions;
    
	if (backend == "?") // If no backend found in the fluid name
	{
		if (library_options.size() > 1)	//Check if an option has been added to libraryName (should be the case for all incompressible)
		{
			backend = library_options[1]; // [0] is always "CoolProp", [1] should be the option if any
		}
		else
		{
			//No backend found, default to HEOS
			backend = "HEOS";
		}
	}

	// Initialise the saturation and near-critical variables
	_p_eps   = 1e-3; // relative tolerance margin for subcritical pressure conditions
	_delta_h = 1e-1; // delta_h for one-phase/two-phase discrimination

	if (name_options.size()>1)
	{
		for (unsigned int i = 1; i<name_options.size(); i++)
		{
			// Split around the equals sign
			std::vector<std::string> param_val = strsplit(name_options[i],'=');
			if (param_val.size() != 2)
			{
				errorMessage((char*)format("Could not parse the option [%s], must be in the form param=value",name_options[i].c_str()).c_str());
			}

			// Check each of the options in turn
			if (!param_val[0].compare("enable_TTSE"))
			{
				if (!param_val[1].compare("1") || !param_val[1].compare("true"))
				{
					std::cout << "TTSE is on\n";
					enable_TTSE = true;
					backend = "TTSE&" + backend;
				}
				else if (!param_val[1].compare("0") || !param_val[1].compare("false"))
				{
					std::cout << "TTSE is off\n";
					enable_TTSE = false;
				}
				else
					errorMessage((char*)format("I don't know how to handle this option [%s]",name_options[i].c_str()).c_str());
					//throw NotImplementedError((char*)format("I don't know how to handle this option [%s]",name_options[i].c_str()).c_str());
			}
			else if (!param_val[0].compare("enable_BICUBIC"))
			{
				if (!param_val[1].compare("1") || !param_val[1].compare("true"))
				{
					std::cout << "BICUBIC is on\n";
					enable_BICUBIC = true;
					backend = "BICUBIC&" + backend;
				}
				else if (!param_val[1].compare("0") || !param_val[1].compare("false"))
				{
					std::cout << "BICUBIC is off\n";
					enable_BICUBIC = false;
				}
				else
					errorMessage((char*)format("I don't know how to handle this option [%s]",name_options[i].c_str()).c_str());
					//throw NotImplementedError((char*)format("I don't know how to handle this option [%s]",name_options[i].c_str()).c_str());
			}
			else if (!param_val[0].compare("calc_transport"))
			{
				if (!param_val[1].compare("1") || !param_val[1].compare("true"))
					calc_transport = true;
				else if (!param_val[1].compare("0") || !param_val[1].compare("false"))
					calc_transport = false;
				else
					errorMessage((char*)format("I don't know how to handle this option [%s]",name_options[i].c_str()).c_str());
			}
			else if (!param_val[0].compare("enable_EXTTP"))
			{
				if (!param_val[1].compare("1") || !param_val[1].compare("true"))
					extend_twophase = true;
				else if (!param_val[1].compare("0") || !param_val[1].compare("false"))
					extend_twophase = false;
				else
					errorMessage((char*)format("I don't know how to handle this option [%s]",name_options[i].c_str()).c_str());
			}
			else if (!param_val[0].compare("twophase_derivsmoothing_xend"))
			{
				twophase_derivsmoothing_xend = strtod(param_val[1].c_str(),NULL);
				if (twophase_derivsmoothing_xend<0 || twophase_derivsmoothing_xend > 1)
					errorMessage((char*)format("I don't know how to handle this twophase_derivsmoothing_xend value [%d]",param_val[0].c_str()).c_str());
			}
			else if (!param_val[0].compare("rho_smoothing_xend"))
			{
				rho_smoothing_xend = strtod(param_val[1].c_str(),NULL);
				if (rho_smoothing_xend<0 || rho_smoothing_xend > 1)
					errorMessage((char*)format("I don't know how to handle this rho_smoothing_xend value [%d]",param_val[0].c_str()).c_str());
			}
			else if (!param_val[0].compare("debug"))
			{
				debug_level = (int)strtol(param_val[1].c_str(),NULL,0);
				if (debug_level<0 || debug_level > 1000) {
					errorMessage((char*)format("I don't know how to handle this debug level [%s]",param_val[0].c_str()).c_str());
				} else {
					// TODO: Fix this segmentation fault!
					//set_debug_level(debug_level);
				}
			}
			else
			{
				errorMessage((char*)format("This option [%s] was not understood",name_options[i].c_str()).c_str());
			}

			// Some options were passed in, lets see what we have
			std::cout << param_val[0] << " has the value of " << param_val[1] << std::endl;
		}
	}

	// Handle the name
	if (debug_level > 5) std::cout << "Check passed, reducing " << substanceName << " to fluid " << this->substanceName << ", with " << backend << " backend."<< std::endl;

	// Check if incompressible
	isCompressible = (backend.find("INCOMP") == std::string::npos);

	// Create the state class
	this->state = CoolProp::AbstractState::factory(backend, this->substanceName);
    if (this->state->using_mole_fractions()){
        // Skip predefined mixtures and pure fluids
        if (this->state->get_mole_fractions().empty()){
            this->state->set_mole_fractions(*fractions_ptr);
        }
    } else if (this->state->using_mass_fractions()){
        this->state->set_mass_fractions(*fractions_ptr);
    } else if (this->state->using_volu_fractions()){
        this->state->set_volu_fractions(*fractions_ptr);
    } else {
        if (debug_level > 5) std::cout << format("%s:%d: CoolPropSolver could not set composition, defaulting to mole fractions.\n",__FILE__,__LINE__);
        this->state->set_mole_fractions(*fractions_ptr);
    }

    // ... all is set, start using the state class.
	this->setFluidConstants();
}


CoolPropSolver::~CoolPropSolver(){
	delete state;
};


void CoolPropSolver::setFluidConstants(){
	if (isCompressible){
		if (debug_level > 5) std::cout << format("Setting constants for fluid %s \n",substanceName.c_str());
		// Theses values are part of the `trivial_keyed_output` and do not require a state update.
		_fluidConstants.pc = state->p_critical();
		_fluidConstants.Tc = state->T_critical();
		_fluidConstants.MM = state->molar_mass();
		_fluidConstants.dc = state->rhomass_critical();
		// Now we fill the close to crit record
		if (debug_level > 5) std::cout << format("Setting near-critical saturation conditions for fluid %s \n",substanceName.c_str());
		_satPropsClose2Crit.psat = _fluidConstants.pc*(1.0-_p_eps); // Needs update, setSat_p relies on it
		setSat_p(_satPropsClose2Crit.psat, &_satPropsClose2Crit);

	}
	else { // incompressible
		if (debug_level > 5) std::cout << format("Setting constants for incompressible fluid %s \n",substanceName.c_str());
		_fluidConstants.pc = NAN;
		_fluidConstants.Tc = NAN;
		_fluidConstants.MM = NAN; //state->molar_mass(); //NAN
		_fluidConstants.dc = NAN;
	}
}


void CoolPropSolver::postStateChange(ExternalThermodynamicState *const properties) {
	/// Some common code to avoid pitfalls from incompressibles
	if (isCompressible)
	{
		try{
			// Set the values in the output structure
			properties->p = state->p();
			properties->T = state->T();
			properties->d = state->rhomass();
			properties->h = state->hmass();
			properties->s = state->smass();
			if (state->phase() == CoolProp::iphase_twophase){
				properties->phase = 2;
			}
			else{
				properties->phase = 1;
			}
			properties->cv = state->cvmass();
			properties->a = state->speed_sound();
			if ((state->phase() == CoolProp::iphase_twophase) && state->Q() >= 0 && state->Q() <= twophase_derivsmoothing_xend && twophase_derivsmoothing_xend > 0.0)
			{
				// Use the smoothed derivatives between a quality of 0 and twophase_derivsmoothing_xend
				properties->ddhp = state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP, twophase_derivsmoothing_xend); // [1/kPa -- > 1/Pa]
				properties->ddph = state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass, twophase_derivsmoothing_xend); // [1/(kJ/kg) -- > 1/(J/kg)]
			}
			else if ((state->phase() == CoolProp::iphase_twophase) && state->Q() >= 0 && state->Q() <= rho_smoothing_xend && rho_smoothing_xend > 0.0)
			{
				// Use the smoothed density between a quality of 0 and rho_smoothing_xend
				properties->ddhp =  state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP, rho_smoothing_xend);
				properties->ddph = state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass, rho_smoothing_xend);
				properties->d = state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iDmass, CoolProp::iDmass, rho_smoothing_xend);
			}
			else
			{
				properties->ddhp = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
				properties->ddph = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass);
			}

			// When two phases and EXTTP activated, interpolate some values from the saturated ones.
			// Theses values have generally no physical meaning in this area.
			if ((extend_twophase) && (properties->phase ==2))
			{
				/* Old way creating two more states.
				// Temporary varriables
				double cp_L, kappa_L, beta_L, eta_L, lambda_L, cp_V, kappa_V, beta_V, eta_V, lambda_V;

				// Liquid saturation values: Q=0
				state->update(CoolProp::PQ_INPUTS,properties->p,0);

				cp_L = state->cpmass();
				kappa_L = state->isothermal_compressibility();
				beta_L = state->isobaric_expansion_coefficient();
				if (calc_transport)
				{
					eta_L = state->viscosity();
					lambda_L = state->conductivity();
				}
	
				// Vapour saturation values: Q=1
				state->update(CoolProp::PQ_INPUTS,properties->p,1);

				cp_V = state->cpmass();
				kappa_V = state->isothermal_compressibility();
				beta_V = state->isobaric_expansion_coefficient();
				if (calc_transport)
				{
					eta_V = state->viscosity();
					lambda_V = state->conductivity();
				}*/
	
				// Interpolation
				properties->cp = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iCpmass), state->saturated_vapor_keyed_output(CoolProp::iCpmass));
				properties->kappa = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iisothermal_compressibility), state->saturated_vapor_keyed_output(CoolProp::iisothermal_compressibility));
				properties->beta = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iisobaric_expansion_coefficient), state->saturated_vapor_keyed_output(CoolProp::iisobaric_expansion_coefficient));

				if (calc_transport)
				{
					properties->eta = interp_recip(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iviscosity), state->saturated_vapor_keyed_output(CoolProp::iviscosity));
					properties->lambda = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iconductivity), state->saturated_vapor_keyed_output(CoolProp::iconductivity));
				} else {
					properties->eta    = NAN;
					properties->lambda = NAN;
				}

				// Reset the state (to be sure not using the Q=1 state later): // was from the old way
				//state->clear();
			}
			else{
				properties->cp = state->cpmass();
				properties->kappa = state->isothermal_compressibility();
				properties->beta = state->isobaric_expansion_coefficient();

				if (calc_transport)
				{
					properties->eta = state->viscosity();
					properties->lambda = state->conductivity(); //[kW/m/K --> W/m/K]
				} else {
					properties->eta    = NAN;
					properties->lambda = NAN;
				}
			}
		}
		catch(std::exception &e)
		{
			errorMessage((char*)e.what());
		}
	}
	else // incompressible
	{
			try{
				// Set the values in the output structure
				properties->p = state->p();
				properties->T = state->T();
				properties->d = state->rhomass();
				properties->h = state->hmass();
				properties->s = state->smass();
				properties->phase = 1;
				properties->cp = state->cpmass();
				properties->cv = state->cvmass();
				properties->a     = NAN;
				properties->ddhp = NAN; //state->first_partial_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
				properties->ddph = NAN; //state->first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass);
				properties->kappa = NAN;
				properties->beta  = NAN;
				if (calc_transport)
				{
					properties->eta = state->viscosity();
					properties->lambda = state->conductivity(); //[kW/m/K --> W/m/K]
				} else {
					properties->eta    = NAN;
					properties->lambda = NAN;
				}
			}
			catch(std::exception &e)
			{
				errorMessage((char*)e.what());
			}
	}
	if (debug_level > 50)
	{
		std::cout << format("At the end of %s \n","postStateChange");
		std::cout << format("Setting pressure to %f \n",properties->p);
		std::cout << format("Setting temperature to %f \n",properties->T);
		std::cout << format("Setting density to %f \n",properties->d);
		std::cout << format("Setting enthalpy to %f \n",properties->h);
		std::cout << format("Setting entropy to %f \n",properties->s);
	}
}


void CoolPropSolver::setSat_p(double &p, ExternalSaturationProperties *const properties){

	if (debug_level > 5)
		std::cout << format("setSat_p(%0.16e)\n",p);

	if (p > _satPropsClose2Crit.psat) { // supercritical conditions
		properties->Tsat  = _satPropsClose2Crit.Tsat;  // saturation temperature
		properties->dTp   = _satPropsClose2Crit.dTp;   // derivative of Ts by pressure
		properties->ddldp = _satPropsClose2Crit.ddldp; // derivative of dls by pressure
		properties->ddvdp = _satPropsClose2Crit.ddvdp; // derivative of dvs by pressure
		properties->dhldp = _satPropsClose2Crit.dhldp; // derivative of hls by pressure
		properties->dhvdp = _satPropsClose2Crit.dhvdp; // derivative of hvs by pressure
		properties->dl    = _satPropsClose2Crit.dl;    // bubble density
		properties->dv    = _satPropsClose2Crit.dv;    // dew density
		properties->hl    = _satPropsClose2Crit.hl;    // bubble specific enthalpy
		properties->hv    = _satPropsClose2Crit.hv;    // dew specific enthalpy
		properties->psat  = _satPropsClose2Crit.psat;  // saturation pressure
		properties->sigma = _satPropsClose2Crit.sigma; // Surface tension
		properties->sl    = _satPropsClose2Crit.sl;    // Specific entropy at bubble line (for pressure ps)
		properties->sv    = _satPropsClose2Crit.sv;    // Specific entropy at dew line (for pressure ps)
	} else {
	  //this->preStateChange();
	  try {
		  /* Use of ancillary equations possible - fast but not 100% consistent with the rest                      **
		  ** properties->dl = state->saturation_ancillary(CoolProp::iDmass,0,CoolProp::iT,T);                      **
		  ** properties->dl = state->saturation_ancillary(CoolProp::iDmolar,0,CoolProp::iT,T)/state->molar_mass(); */

		  /* Use of                                                      **
		  ** state->saturated_liquid_keyed_output(CoolProp::iDmass)      **
		  ** state->saturated_vapor_keyed_output(CoolProp::iDmass)       **
		  ** would be interesting but this doesn't solve the derivatives */

		  /* TODO:                                                                                                  **
		  ** Uncoment the `specify_phase` when issue 656 is solved: https://github.com/CoolProp/CoolProp/issues/656 **
		  ** This will allow for a full state update and a speed enhancement                                        */

		  // At bubble line:
		  state->specify_phase(CoolProp::iphase_liquid);
		  state->update(CoolProp::PQ_INPUTS,p,0);
		  //! Saturation temperature
		  properties->Tsat = state->T(); // At bubble line! (mather for pseudo-pure fluids)
		  //! Derivative of Ts wrt pressure
		  properties->dTp = state->first_saturation_deriv(CoolProp::iT, CoolProp::iP);
		  //! Derivative of dls wrt pressure
		  properties->ddldp = state->first_saturation_deriv(CoolProp::iDmass, CoolProp::iP);
		  //! Derivative of hls wrt pressure
		  properties->dhldp = state->first_saturation_deriv(CoolProp::iHmass, CoolProp::iP);
		  //! Density at bubble line (for pressure ps)
		  properties->dl = state->rhomass();
		  //! Specific enthalpy at bubble line (for pressure ps)
		  properties->hl = state->hmass();
		  //! Saturation pressure
		  properties->psat = p;
		  //! Surface tension
		  properties->sigma = state->surface_tension();
		  //! Specific entropy at bubble line (for pressure ps)
		  properties->sl = state->smass();

		  // At dew line:
		  state->specify_phase(CoolProp::iphase_gas);
		  state->update(CoolProp::PQ_INPUTS,p,1);
		  //! Derivative of dvs wrt pressure
		  properties->ddvdp = state->first_saturation_deriv(CoolProp::iDmass, CoolProp::iP);
		  //! Derivative of hvs wrt pressure
		  properties->dhvdp = state->first_saturation_deriv(CoolProp::iHmass, CoolProp::iP);
		  //! Density at dew line (for pressure ps)
		  properties->dv = state->rhomass();
		  //! Specific enthalpy at dew line (for pressure ps)
		  properties->hv = state->hmass();
		  //! Specific entropy at dew line (for pressure ps)
		  properties->sv = state->smass();
		  // state->specify_phase(CoolProp::iphase_not_imposed);

		  // Reset the state (to be sure a new one is created before computing new values):
		  state->clear();

	  } catch(std::exception &e) {
		errorMessage((char*)e.what());
	  }
    }
}

void CoolPropSolver::setSat_T(double &T, ExternalSaturationProperties *const properties){

	if (debug_level > 5)
		std::cout << format("setSat_T(%0.16e)\n",T);

	if (T > _satPropsClose2Crit.Tsat) { // supercritical conditions
		properties->Tsat  = _satPropsClose2Crit.Tsat;  // saturation temperature
		properties->dTp   = _satPropsClose2Crit.dTp;   // derivative of Ts by pressure
		properties->ddldp = _satPropsClose2Crit.ddldp; // derivative of dls by pressure
		properties->ddvdp = _satPropsClose2Crit.ddvdp; // derivative of dvs by pressure
		properties->dhldp = _satPropsClose2Crit.dhldp; // derivative of hls by pressure
		properties->dhvdp = _satPropsClose2Crit.dhvdp; // derivative of hvs by pressure
		properties->dl    = _satPropsClose2Crit.dl;    // bubble density
		properties->dv    = _satPropsClose2Crit.dv;    // dew density
		properties->hl    = _satPropsClose2Crit.hl;    // bubble specific enthalpy
		properties->hv    = _satPropsClose2Crit.hv;    // dew specific enthalpy
		properties->psat  = _satPropsClose2Crit.psat;  // saturation pressure
		properties->sigma = _satPropsClose2Crit.sigma; // Surface tension
		properties->sl    = _satPropsClose2Crit.sl;    // Specific entropy at bubble line (for pressure ps)
		properties->sv    = _satPropsClose2Crit.sv;    // Specific entropy at dew line (for pressure ps)
	} else {
	  //this->preStateChange();
	  try
	  {
		  // Ancillary equations - fast but not 100% consistent with the rest
		  //properties->dl = state->saturation_ancillary(CoolProp::iDmass,0,CoolProp::iT,T);
		  //properties->dl = state->saturation_ancillary(CoolProp::iDmolar,0,CoolProp::iT,T)/state->molar_mass();

		  /* TODO:                                                                                                  **
		  ** Uncoment the `specify_phase` when issue 656 is solved: https://github.com/CoolProp/CoolProp/issues/656 **
		  ** This will allow for a full state update and a speed enhancement                                        */
		  
		  // At bubble line:
		  // state->specify_phase(CoolProp::iphase_liquid);
		  state->update(CoolProp::QT_INPUTS,0,T);
		  properties->Tsat = T;
		  properties->psat = state->p(); // At bubble line! (mather for pseudo-pure fluids)
		  properties->dl = state->rhomass();
		  properties->hl = state->hmass();
		  properties->dTp = state->first_saturation_deriv(CoolProp::iT, CoolProp::iP);
		  properties->ddldp = state->first_saturation_deriv(CoolProp::iDmass, CoolProp::iP);
		  properties->dhldp = state->first_saturation_deriv(CoolProp::iHmass, CoolProp::iP);
		  properties->sigma = state->surface_tension(); // Surface tension
		  properties->sl    = state->smass();    // Specific entropy at bubble line (for pressure ps)

		  // At dew line:
		  // state->specify_phase(CoolProp::iphase_gas);
		  state->update(CoolProp::QT_INPUTS,1,T);
		  properties->dv = state->rhomass();
		  properties->hv = state->hmass();
		  properties->ddvdp = state->first_saturation_deriv(CoolProp::iDmass, CoolProp::iP);
		  properties->dhvdp = state->first_saturation_deriv(CoolProp::iHmass, CoolProp::iP);
		  properties->sv    = state->smass();    // Specific entropy at dew line (for pressure ps)
		  // state->specify_phase(CoolProp::iphase_not_imposed);

	  } catch(std::exception &e) {
		errorMessage((char*)e.what());
	  }
	}
}

/// Set bubble state
void CoolPropSolver::setBubbleState(ExternalSaturationProperties *const properties, int phase, ExternalThermodynamicState *const bubbleProperties){
	double hl;
	if (phase == 0)
		hl = properties->hl;
	else if (phase == 1) // liquid phase
		hl = properties->hl-_delta_h;
	else                 // two-phase mixture
		hl = properties->hl+_delta_h;

	setState_ph(properties->psat, hl, phase, bubbleProperties);
}

/// Set dew state
void CoolPropSolver::setDewState(ExternalSaturationProperties *const properties, int phase, ExternalThermodynamicState *const dewProperties){
	double hv;
	if (phase == 0)
		hv = properties->hv;
	else if (phase == 1) // gaseous phase
		hv = properties->hv+_delta_h;
	else                 // two-phase mixture
		hv = properties->hv-_delta_h;

	setState_ph(properties->psat, hv, phase, dewProperties);
}

// Note: the phase input is currently not supported
void CoolPropSolver::setState_ph(double &p, double &h, int &phase, ExternalThermodynamicState *const properties){

	if (debug_level > 5)
		std::cout << format("setState_ph(p=%0.16e,h=%0.16e)\n",p,h);

	//this->preStateChange();

	try{
		// Update the internal variables in the state instance
		state->update(CoolProp::HmassP_INPUTS,h,p);

		if (!ValidNumber(state->rhomass()) || !ValidNumber(state->T()))
		{
			throw CoolProp::ValueError(format("p-h [%g, %g] failed for update",p,h));
		}

		// Set the values in the output structure
		this->postStateChange(properties);
	}
	catch(std::exception &e)
	{
		errorMessage((char*)e.what());
	}
}

void CoolPropSolver::setState_pT(double &p, double &T, ExternalThermodynamicState *const properties){

	if (debug_level > 5)
		std::cout << format("setState_pT(p=%0.16e,T=%0.16e)\n",p,T);

	//this->preStateChange();

	try{
		// Update the internal variables in the state instance
		state->update(CoolProp::PT_INPUTS,p,T);

		// Set the values in the output structure
		this->postStateChange(properties);
	}
	catch(std::exception &e)
	{
		errorMessage((char*)e.what());
	}
}

// Note: the phase input is currently not supported
void CoolPropSolver::setState_dT(double &d, double &T, int &phase, ExternalThermodynamicState *const properties)
{

	if (debug_level > 5)
		std::cout << format("setState_dT(d=%0.16e,T=%0.16e)\n",d,T);

	//this->preStateChange();

	try{

		// Update the internal variables in the state instance
		state->update(CoolProp::DmassT_INPUTS,d,T);

		// Set the values in the output structure
		this->postStateChange(properties);
	}
	catch(std::exception &e)
	{
		errorMessage((char*)e.what());
	}
}

// Note: the phase input is currently not supported
void CoolPropSolver::setState_ps(double &p, double &s, int &phase, ExternalThermodynamicState *const properties){

	if (debug_level > 5)
		std::cout << format("setState_ps(p=%0.16e,s=%0.16e)\n",p,s);

	//this->preStateChange();

	try{
		// Update the internal variables in the state instance
		state->update(CoolProp::PSmass_INPUTS,p,s);

		// Set the values in the output structure
		this->postStateChange(properties);
	}
	catch(std::exception &e)
	{
		errorMessage((char*)e.what());
	}
}


// Note: the phase input is currently not supported
void CoolPropSolver::setState_hs(double &h, double &s, int &phase, ExternalThermodynamicState *const properties){

	if (debug_level > 5)
		std::cout << format("setState_hs(h=%0.16e,s=%0.16e)\n",h,s);

	//this->preStateChange();

	try{
		// Update the internal variables in the state instance
		state->update(CoolProp::HmassSmass_INPUTS,h,s);

		// Set the values in the output structure
		this->postStateChange(properties);
	}
	catch(std::exception &e)
	{
		errorMessage((char*)e.what());
	}
}

double CoolPropSolver::partialDeriv_state(const string &of, const string &wrt, const string &cst, ExternalThermodynamicState *const properties){
	if (debug_level > 5)
		std::cout << format("partialDeriv_state(of=%s,wrt=%s,cst=%s,state)\n",of.c_str(),wrt.c_str(),cst.c_str());

	long derivTerm = makeDerivString(of,wrt,cst);
	double res = NAN;

	try{
		//res = DerivTerms(derivTerm, properties->d, properties->T, this->substanceName);
		state->update(CoolProp::DmassT_INPUTS,properties->d,properties->T);
		// Get the output value
		res = state->keyed_output(static_cast<CoolProp::parameters>(derivTerm));
	} catch(std::exception &e) {
		errorMessage((char*)e.what());
	}
	return res;
}

long CoolPropSolver::makeDerivString(const string &of, const string &wrt, const string &cst){
	std::string derivTerm;
	     if (!of.compare("d")){ derivTerm = "drho"; }
	else if (!of.compare("p")){ derivTerm = "dp"; }
	else {
		errorMessage((char*) format("Internal error: Derivatives of %s are not defined in the Solver object.",of.c_str()).c_str());
	}
	if      (!wrt.compare("p")){ derivTerm.append("dp"); }
	else if (!wrt.compare("h")){ derivTerm.append("dh"); }
	else if (!wrt.compare("T")){ derivTerm.append("dT"); }
	else {
		errorMessage((char*) format("Internal error: Derivatives with respect to %s are not defined in the Solver object.",wrt.c_str()).c_str());
	}
	if      (!cst.compare("p")){ derivTerm.append("|p"); }
	else if (!cst.compare("h")){ derivTerm.append("|h"); }
	else if (!cst.compare("d")){ derivTerm.append("|rho"); }
	else {
		errorMessage((char*) format("Internal error: Derivatives at constant %s are not defined in the Solver object.",cst.c_str()).c_str());
	}
	long iOutput = CoolProp::get_parameter_index(derivTerm.c_str());
	return iOutput;
}


double CoolPropSolver::Pr(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: Pr() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: Pr() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::T(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: T() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: T() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::a(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: a() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: a() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::beta(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: beta() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: beta() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::cp(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: cpmass() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: cpmass() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::cv(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: cvmass() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: cvmass() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::d(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: d() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: d() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::ddhp(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: ddhp() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: ddhp() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::ddph(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: ddph() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: ddph() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::eta(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: eta() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: eta() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::h(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: hmass() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: hmass() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::kappa(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: kappa() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: kappa() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::lambda(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: lambda() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: lambda() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::p(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: p() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: p() not implemented in the Solver object");
	return NAN;
}

int CoolPropSolver::phase(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: phase() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: phase() not implemented in the Solver object");
	return -1;
}

double CoolPropSolver::s(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: smass() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: smass() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::d_der(ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: d_der() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: d_der() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::isentropicEnthalpy(double &p, ExternalThermodynamicState *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: isentropicEnthalpy() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: isentropicEnthalpy() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::dTp(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: dTp() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: dTp() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::ddldp(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: ddldp() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: ddldp() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::ddvdp(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: ddvdp() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: ddvdp() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::dhldp(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: dhldp() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: dhldp() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::dhvdp(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: dhvdp() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: dhvdp() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::dl(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: dl() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: dl() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::dv(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: dv() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: dv() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::hl(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: hl() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: hl() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::hv(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: hv() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: hv() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::sigma(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: sigma() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: sigma() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::sl(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: sl() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: sl() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::sv(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: sv() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: sv() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::psat(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: psat() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: psat() not implemented in the Solver object");
	return NAN;
}

double CoolPropSolver::Tsat(ExternalSaturationProperties *const properties){
    // Base function returns an error if called - should be redeclared by the solver object
	errorMessage((char*)"Internal error: Tsat() not implemented in the Solver object");
	//throw NotImplementedError((char*)"Internal error: Tsat() not implemented in the Solver object");
	return NAN;
}

/// Interpolation routines
double CoolPropSolver::interp_linear(double Q, double valueL, double valueV) {
	return valueL+Q*(valueV-valueL);
}
double CoolPropSolver::interp_recip(double Q, double valueL, double valueV){
	return 1.0 / interp_linear(Q, 1.0/valueL, 1.0/valueV);
}