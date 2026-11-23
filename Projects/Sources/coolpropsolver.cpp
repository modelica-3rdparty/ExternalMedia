#include "coolpropsolver.h"

#include "include.h"
#if (EXTERNALMEDIA_COOLPROP == 1)

#include "CoolPropTools.h"
#include "CoolPropLib.h"
#include "CoolProp.h"
#include "AbstractState.h"
#include "CoolProp/Configuration.h"
#include <iostream>
#include <string>
#include <stdlib.h>
#include <cmath>
#include <algorithm>

/*
 * Settings of the SVDSBTL option, see the comment block "SVDSBTL support"
 * further down.
 */
/* Convergence tolerance of the Newton steps: relative change of density
   and relative change of the second input (h or s) */
static const double SVDSBTL_TOL = 1e-9;
/* At most this many Newton steps in p-h and p-s. The measurements in
   Projects/Tests/svdsbtl_prototype_results.md never needed more than 2. */
static const int SVDSBTL_MAX_STEPS = 5;
/* At most this many Newton steps in p-T, where the table guess can be
   poor close to the saturation line and near the critical point */
static const int SVDSBTL_MAX_STEPS_PT = 20;
/* Scales that keep the tolerance on h and s finite where h or s is zero */
static const double SVDSBTL_H_SCALE_J_KG = 1e5;
static const double SVDSBTL_S_SCALE_J_KGK = 1e3;

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
	_valid          = false;
	enable_TTSE     = false;
	enable_BICUBIC  = false;
	enable_SVDSBTL  = false;
	debug_level     = 0;
	calc_transport  = true;
	extend_twophase = true;
	twophase_derivsmoothing_xend = 0;
	rho_smoothing_xend = 0;
	int cache_size  = 1024; // slots in the state caches, 0 switches them off

	//Check if a backend has been added to the fluid name (ex: REFPROP::Propane)
    std::string backend;
    // Set the default composition
    std::vector<double> fractions(1, 1.0);
    const std::vector<double> *fractions_ptr = NULL;
    // errorMessage does not return on most platforms. Where it does (Windows
    // before ModelicaError is found), this flag keeps the solver invalid.
    bool names_ok = true;
    try{
        CoolProp::extract_backend(name_options[0], backend, this->substanceName);
        this->substanceName = CoolProp::extract_fractions(this->substanceName, fractions);
    } catch(std::exception &e) {
        names_ok = false;
        errorMessage((char*)e.what());
    }
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
			else if (!param_val[0].compare("enable_SVDSBTL"))
			{
				if (!param_val[1].compare("1") || !param_val[1].compare("true"))
				{
					std::cout << "SVDSBTL is on\n";
					enable_SVDSBTL = true;
				}
				else if (!param_val[1].compare("0") || !param_val[1].compare("false"))
				{
					std::cout << "SVDSBTL is off\n";
					enable_SVDSBTL = false;
				}
				else
					errorMessage((char*)format("I don't know how to handle this option [%s]",name_options[i].c_str()).c_str());
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
			else if (!param_val[0].compare("cache_size"))
			{
				long size = strtol(param_val[1].c_str(),NULL,0); // check before the cast to int
				if (size<0 || size > 1048576)
				{
					errorMessage((char*)format("I don't know how to handle this cache size [%s]",param_val[1].c_str()).c_str());
					// errorMessage can return on Windows, switch the caches off
					// rather than allocating a rejected size
					size = 0;
				}
				cache_size = (int)size;
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

	// Apply the table option. Only one table method can be active at a time.
	if ((int)enable_TTSE + (int)enable_BICUBIC + (int)enable_SVDSBTL > 1)
	{
		errorMessage((char*)format("Only one of enable_TTSE, enable_BICUBIC and enable_SVDSBTL can be set for [%s]",substanceName.c_str()).c_str());
	}
	// The reference backend, e.g. "HEOS", "REFPROP" or "IF97"
	const std::string source_backend = backend;
	if (enable_TTSE)
	{
		backend = "TTSE&" + backend;
	}
	else if (enable_BICUBIC)
	{
		backend = "BICUBIC&" + backend;
	}
	// With enable_SVDSBTL the main state keeps the reference backend,
	// the SVDSBTL tables are created below in table_state.

	// Handle the name
	if (debug_level > 5) std::cout << "Check passed, reducing " << substanceName << " to fluid " << this->substanceName << ", with " << backend << " backend."<< std::endl;

	// Check if incompressible
	isCompressible = (backend.find("INCOMP") == std::string::npos);

	// CoolProp reports problems, such as an unknown fluid name, with C++ exceptions.
	// They must not leave the library, translate them into an ExternalMedia error.
	try{
		// Create the state class
		//this->state = CoolProp::AbstractState::factory(backend, this->substanceName);
		this->state.reset(CoolProp::AbstractState::factory(backend, this->substanceName));
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

	    if (enable_SVDSBTL)
	    {
	        // The SVDSBTL tables only serve as starting values for the
	        // reference equation of state, which is evaluated at (T, rho).
	        // This works for HEOS and REFPROP. IF97 cannot be evaluated at
	        // (T, rho), and it is fast without tables anyway.
	        if (source_backend != "HEOS" && source_backend != "REFPROP")
	        {
	            names_ok = false;
	            errorMessage((char*)format("enable_SVDSBTL is only available with the HEOS and REFPROP backends, not with %s [%s]",source_backend.c_str(),substanceName.c_str()).c_str());
	        }
	        if (!isCompressible)
	        {
	            names_ok = false;
	            errorMessage((char*)format("enable_SVDSBTL is not available for incompressible fluids [%s]",substanceName.c_str()).c_str());
	        }
	        if (this->state->get_mole_fractions().size() != 1)
	        {
	            names_ok = false;
	            errorMessage((char*)format("enable_SVDSBTL is only available for pure fluids, not for mixtures [%s]",substanceName.c_str()).c_str());
	        }
	        try
	        {
	            // The first use of a fluid builds the tables (tens of seconds to
	            // a few minutes) and stores them in the CoolProp cache folder,
	            // later runs load them from there. Use all processor cores for
	            // the first build, unless the user chose a number of threads
	            // with the environment variable COOLPROP_SVDSBTL_SAMPLING_THREADS.
	            // This is a global CoolProp setting, it only affects table builds.
	            if (std::getenv("COOLPROP_SVDSBTL_SAMPLING_THREADS") == NULL)
	            {
	                CoolProp::set_config_int(SVDSBTL_SAMPLING_THREADS, 0);
	            }
	            // SVDSBTL names its cache files after the fluid name as given,
	            // so "water" and "Water" would build two sets of tables. Use the
	            // name of the reference state, which is the canonical one.
	            std::string table_fluid = this->substanceName;
	            const std::vector<std::string> names = this->state->fluid_names();
	            if (names.size() == 1 && !names[0].empty()) table_fluid = names[0];
	            this->table_state.reset(CoolProp::AbstractState::factory("SVDSBTL&" + source_backend, table_fluid));
	        }
	        catch(std::exception &e)
	        {
	            names_ok = false;
	            errorMessage((char*)format("Could not create the SVDSBTL tables for [%s] with the %s backend: %s",substanceName.c_str(),source_backend.c_str(),e.what()).c_str());
	        }
	    }


	    // Switch on the state caches, see BaseSolver::_stateCache
	    _stateCache.resize(cache_size);
	    _satCache.resize(cache_size);

	    // ... all is set, start using the state class.
		this->setFluidConstants();
		_valid = names_ok;
	} catch(std::exception &e) {
		errorMessage((char*)e.what());
	}
}


CoolPropSolver::~CoolPropSolver(){
	//delete state;
};

// Print the hit and miss counts of the state caches every 2^20 lookups.
// The solver objects are never deleted, so this cannot be done in the destructor.
void CoolPropSolver::printCacheStatistics(){
	if (debug_level <= 0) return;
	unsigned long long n_state = _stateCache.hits() + _stateCache.misses();
	unsigned long long n_sat = _satCache.hits() + _satCache.misses();
	// switched-off caches never count, n = 0 must not print on every call
	if (n_state + n_sat > 0 && (n_state + n_sat) % 1048576 == 0)
		std::cout << format("State cache for %s: %llu hits, %llu misses; saturation cache: %llu hits, %llu misses\n",
		                    substanceName.c_str(), _stateCache.hits(), _stateCache.misses(),
		                    _satCache.hits(), _satCache.misses());
}

// Store a state in the cache. If the calculation failed without aborting
// the simulation (possible on Windows, see errorMessage), the record holds
// invalid numbers and must not be returned again for the same inputs.
// The callers only store after postStateChange succeeded, this check is a
// second guard for the main properties.
void CoolPropSolver::storeState(int inputs, double x1, double x2, int phase, const ExternalThermodynamicState *const properties){
	if (ValidNumber(properties->p) && ValidNumber(properties->T) && ValidNumber(properties->d) && ValidNumber(properties->h))
		_stateCache.store(inputs, x1, x2, phase, *properties);
}

void CoolPropSolver::storeSat(int inputs, double x, const ExternalSaturationProperties *const properties){
	if (ValidNumber(properties->psat) && ValidNumber(properties->Tsat) && ValidNumber(properties->hl) && ValidNumber(properties->hv)
	    && ValidNumber(properties->dl) && ValidNumber(properties->dv))
		_satCache.store(inputs, x, 0.0, 0, *properties);
}


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


bool CoolPropSolver::postStateChange(ExternalThermodynamicState *const properties) {
    /// Some common code to avoid pitfalls from incompressibles
    bool success = true; // false if a property call failed, see the catch blocks
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
            if ((state->phase() == CoolProp::iphase_twophase) && state->Q() >= 0 && state->Q() <= twophase_derivsmoothing_xend && twophase_derivsmoothing_xend > 0.0)
            {
                // Use the smoothed derivatives between a quality of 0 and twophase_derivsmoothing_xend
                properties->ddph = state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass, twophase_derivsmoothing_xend); // [1/(kJ/kg) -- > 1/(J/kg)]
                properties->ddhp = state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP, twophase_derivsmoothing_xend); // [1/kPa -- > 1/Pa]
            }
            else if ((state->phase() == CoolProp::iphase_twophase) && state->Q() >= 0 && state->Q() <= rho_smoothing_xend && rho_smoothing_xend > 0.0)
            {
                // Use the smoothed density between a quality of 0 and rho_smoothing_xend
                properties->ddph = state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass, rho_smoothing_xend);
                properties->ddhp =  state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP, rho_smoothing_xend);
                properties->d = state->first_two_phase_deriv_splined(CoolProp::iDmass, CoolProp::iDmass, CoolProp::iDmass, rho_smoothing_xend);
            }
            else if (state->phase() == CoolProp::iphase_twophase)
            {
                properties->ddhp = state->first_two_phase_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
                properties->ddph = state->first_two_phase_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass);
            }
            else
            {
                properties->ddhp = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
                properties->ddph = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass);
            }
            // When two phases and EXTTP activated, interpolate some values from the saturated ones.
            // Theses values have generally no physical meaning in this area.
            if (state->phase() == CoolProp::iphase_twophase){
                if (extend_twophase)
                {
                    // Interpolation
                    properties->cv = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iCvmass), state->saturated_vapor_keyed_output(CoolProp::iCvmass));
                    properties->a = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::ispeed_sound), state->saturated_vapor_keyed_output(CoolProp::ispeed_sound));
                    properties->cp = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iCpmass), state->saturated_vapor_keyed_output(CoolProp::iCpmass));
                    //properties->kappa = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iisothermal_compressibility), state->saturated_vapor_keyed_output(CoolProp::iisothermal_compressibility));
                    //properties->beta = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iisobaric_expansion_coefficient), state->saturated_vapor_keyed_output(CoolProp::iisobaric_expansion_coefficient));
                    properties->kappa = NAN;
                    properties->beta = NAN;

                    if (calc_transport)
                    {
                        properties->eta = interp_recip(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iviscosity), state->saturated_vapor_keyed_output(CoolProp::iviscosity));
                        properties->lambda = interp_linear(state->Q(), state->saturated_liquid_keyed_output(CoolProp::iconductivity), state->saturated_vapor_keyed_output(CoolProp::iconductivity));
                    }
                    else {
                        properties->eta = NAN;
                        properties->lambda = NAN;
                    }

                }
                else {
                    // No interpolation
                    properties->cv = NAN;
                    properties->a = NAN;
                    properties->cp = NAN;
                    properties->kappa = NAN;
                    properties->beta = NAN;
                    properties->eta = NAN;
                    properties->lambda = NAN;
                }
            }
            else{
				properties->cv = state->cvmass();
				properties->a = state->speed_sound();
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
            success = false;
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
            properties->ddhp = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iHmass, CoolProp::iP);
            properties->ddph = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iHmass);
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
            success = false;
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
    return success;
}

void fillCritSatState(ExternalSaturationProperties* const properties, const ExternalSaturationProperties &_satPropsClose2Crit) {
	properties->Tsat = _satPropsClose2Crit.Tsat;  // saturation temperature
	properties->dTp = _satPropsClose2Crit.dTp;   // derivative of Ts by pressure
	properties->ddldp = _satPropsClose2Crit.ddldp; // derivative of dls by pressure
	properties->ddvdp = _satPropsClose2Crit.ddvdp; // derivative of dvs by pressure
	properties->dhldp = _satPropsClose2Crit.dhldp; // derivative of hls by pressure
	properties->dhvdp = _satPropsClose2Crit.dhvdp; // derivative of hvs by pressure
	properties->dl = _satPropsClose2Crit.dl;    // bubble density
	properties->dv = _satPropsClose2Crit.dv;    // dew density
	properties->hl = _satPropsClose2Crit.hl;    // bubble specific enthalpy
	properties->hv = _satPropsClose2Crit.hv;    // dew specific enthalpy
	properties->psat = _satPropsClose2Crit.psat;  // saturation pressure
	properties->sigma = _satPropsClose2Crit.sigma; // Surface tension
	properties->sl = _satPropsClose2Crit.sl;    // Specific entropy at bubble line (for pressure ps)
	properties->sv = _satPropsClose2Crit.sv;    // Specific entropy at dew line (for pressure ps)
}

void fillBubbleState(shared_ptr<CoolProp::AbstractState> state, ExternalSaturationProperties* const properties) {
	properties->psat = state->p(); // At bubble line! (matters for pseudo-pure fluids, potentially wrong for mixtures)
	properties->Tsat = state->T(); // At bubble line! (matters for pseudo-pure fluids, potentially wrong for mixtures)
	properties->dTp = state->first_saturation_deriv(CoolProp::iT, CoolProp::iP); // At bubble line! (matters for pseudo-pure fluids, potentially wrong for mixtures)

	try {
		//! Surface tension
		properties->sigma = state->surface_tension();
	} catch (...) {
		properties->sigma = NAN;
	}

	properties->dl = state->rhomass();
	properties->hl = state->hmass();
	properties->ddldp = state->first_saturation_deriv(CoolProp::iDmass, CoolProp::iP);
	properties->dhldp = state->first_saturation_deriv(CoolProp::iHmass, CoolProp::iP);
	properties->sl = state->smass();
}

void fillDewState(shared_ptr<CoolProp::AbstractState> state, ExternalSaturationProperties* const properties) {
	properties->dv = state->rhomass();
	properties->hv = state->hmass();
	properties->ddvdp = state->first_saturation_deriv(CoolProp::iDmass, CoolProp::iP);
	properties->dhvdp = state->first_saturation_deriv(CoolProp::iHmass, CoolProp::iP);
	properties->sv = state->smass();
}


void CoolPropSolver::setSat_p(double &p, ExternalSaturationProperties *const properties){

	if (debug_level > 5)
		std::cout << format("setSat_p(%0.16e)\n",p);

	if (enable_SVDSBTL) state->unspecify_phase();

	if (p > _satPropsClose2Crit.psat) { // supercritical conditions
		fillCritSatState(properties, _satPropsClose2Crit);
	} else {
	  // Same p as a stored call: return the stored properties
	  bool hit = _satCache.lookup(CACHE_SAT_P, p, 0.0, 0, properties);
	  printCacheStatistics();
	  if (hit) return;

	  // With enable_SVDSBTL: HEOS with the table as starting guess
	  if (enable_SVDSBTL && svdsbtl_setSat_p(p, properties)) {
		  storeSat(CACHE_SAT_P, p, properties);
		  return;
	  }

	  //this->preStateChange();
	  try {
		  /* Use of ancillary equations possible - fast but not 100% consistent with the rest                      **
		  ** properties->dl = state->saturation_ancillary(CoolProp::iDmass,0,CoolProp::iT,T);                      **
		  ** properties->dl = state->saturation_ancillary(CoolProp::iDmolar,0,CoolProp::iT,T)/state->molar_mass(); */

		  /* Use of                                                      **
		  ** state->saturated_liquid_keyed_output(CoolProp::iDmass)      **
		  ** state->saturated_vapor_keyed_output(CoolProp::iDmass)       **
		  ** would be interesting but this doesn't solve the derivatives */

		  // At bubble line:
		  state->specify_phase(CoolProp::iphase_liquid);
		  state->update(CoolProp::PQ_INPUTS, p, 0);
		  fillBubbleState(state, properties);

		  // At dew line:
		  state->specify_phase(CoolProp::iphase_gas);
		  state->update(CoolProp::PQ_INPUTS, p, 1);
		  fillDewState(state, properties);

		  // Reset the state (to be sure a new one is created before computing new values):
		  state->clear();
          state->unspecify_phase();

		  storeSat(CACHE_SAT_P, p, properties);

	  } catch(std::exception &e) {
		errorMessage((char*)e.what());
	  }
    }
}

void CoolPropSolver::setSat_T(double &T, ExternalSaturationProperties *const properties){

	if (debug_level > 5)
		std::cout << format("setSat_T(%0.16e)\n",T);

	if (enable_SVDSBTL) state->unspecify_phase();

	if (T > _satPropsClose2Crit.Tsat) { // supercritical conditions
		fillCritSatState(properties, _satPropsClose2Crit);
	} else {
	  // Same T as a stored call: return the stored properties
	  bool hit = _satCache.lookup(CACHE_SAT_T, T, 0.0, 0, properties);
	  printCacheStatistics();
	  if (hit) return;

	  // With enable_SVDSBTL: HEOS with the table as starting guess
	  if (enable_SVDSBTL && svdsbtl_setSat_T(T, properties)) {
		  storeSat(CACHE_SAT_T, T, properties);
		  return;
	  }

	  //this->preStateChange();
	  try
	  {
		  // Ancillary equations - fast but not 100% consistent with the rest
		  //properties->dl = state->saturation_ancillary(CoolProp::iDmass,0,CoolProp::iT,T);
		  //properties->dl = state->saturation_ancillary(CoolProp::iDmolar,0,CoolProp::iT,T)/state->molar_mass();

		  // At bubble line:
		  state->specify_phase(CoolProp::iphase_liquid);
		  state->update(CoolProp::QT_INPUTS,0,T);
		  fillBubbleState(state, properties);

		  // At dew line:
		  state->specify_phase(CoolProp::iphase_gas);
		  state->update(CoolProp::QT_INPUTS,1,T);
		  fillDewState(state, properties);

		  // Reset the state (to be sure a new one is created before computing new values):
		  state->clear();
		  state->unspecify_phase();

		  storeSat(CACHE_SAT_T, T, properties);

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

	// Same inputs as a stored call: return the stored state
	bool hit = _stateCache.lookup(CACHE_PH, p, h, phase, properties);
	printCacheStatistics();
	if (hit) return;

	//this->preStateChange();

	try{
		if (enable_SVDSBTL)
		{
			state->unspecify_phase();
			if (svdsbtl_solve_p_y(CoolProp::HmassP_INPUTS, p, h, CoolProp::iHmass, SVDSBTL_H_SCALE_J_KG))
			{
				// Single-phase state from HEOS at the exact input pair
				// The returned p and h are the inputs, the state matches them
				// to the Newton tolerance
				const bool success = this->postStateChange(properties);
				properties->p = p;
				properties->h = h;
				state->unspecify_phase();
				if (success)
					storeState(CACHE_PH, p, h, phase, properties);
				return;
			}
			// Two-phase state, or the table route failed: plain flash below
		}

		// Update the internal variables in the state instance
		state->update(CoolProp::HmassP_INPUTS,h,p);

		if (!ValidNumber(state->rhomass()) || !ValidNumber(state->T()))
		{
			throw CoolProp::ValueError(format("p-h [%g, %g] failed for update",p,h));
		}

		// Set the values in the output structure
		// and store it for later calls with the same inputs
		if (this->postStateChange(properties))
			storeState(CACHE_PH, p, h, phase, properties);
	}
	catch(std::exception &e)
	{
		errorMessage((char*)e.what());
	}
}

void CoolPropSolver::setState_pT(double &p, double &T, ExternalThermodynamicState *const properties){

	if (debug_level > 5)
		std::cout << format("setState_pT(p=%0.16e,T=%0.16e)\n",p,T);

	// Same inputs as a stored call: return the stored state
	bool hit = _stateCache.lookup(CACHE_PT, p, T, 0, properties);
	printCacheStatistics();
	if (hit) return;

	//this->preStateChange();

	try{
		if (enable_SVDSBTL)
		{
			state->unspecify_phase();
			if (svdsbtl_solve_pT(p, T))
			{
				// The returned p and T are the inputs, the state matches them
				// to the Newton tolerance
				const bool success = this->postStateChange(properties);
				properties->p = p;
				properties->T = T;
				state->unspecify_phase();
				if (success)
					storeState(CACHE_PT, p, T, 0, properties);
				return;
			}
			// The table route failed: plain flash below
		}

		// Update the internal variables in the state instance
		state->update(CoolProp::PT_INPUTS,p,T);

		// Set the values in the output structure
		// and store it for later calls with the same inputs
		if (this->postStateChange(properties))
			storeState(CACHE_PT, p, T, 0, properties);
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

	// Same inputs as a stored call: return the stored state
	bool hit = _stateCache.lookup(CACHE_DT, d, T, phase, properties);
	printCacheStatistics();
	if (hit) return;

	//this->preStateChange();

	try{
		// With enable_SVDSBTL, d-T inputs go to the reference equation of
		// state directly: it is explicit in (T, rho), a table brings nothing.
		if (enable_SVDSBTL) state->unspecify_phase();

		// Update the internal variables in the state instance
		state->update(CoolProp::DmassT_INPUTS,d,T);

		// Set the values in the output structure
		// and store it for later calls with the same inputs
		if (this->postStateChange(properties))
			storeState(CACHE_DT, d, T, phase, properties);
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

	// Same inputs as a stored call: return the stored state
	bool hit = _stateCache.lookup(CACHE_PS, p, s, phase, properties);
	printCacheStatistics();
	if (hit) return;

	//this->preStateChange();

	try{
		if (enable_SVDSBTL)
		{
			state->unspecify_phase();
			if (svdsbtl_solve_p_y(CoolProp::PSmass_INPUTS, p, s, CoolProp::iSmass, SVDSBTL_S_SCALE_J_KGK))
			{
				// Single-phase state from HEOS at the exact input pair
				// The returned p and s are the inputs, the state matches them
				// to the Newton tolerance
				const bool success = this->postStateChange(properties);
				properties->p = p;
				properties->s = s;
				state->unspecify_phase();
				if (success)
					storeState(CACHE_PS, p, s, phase, properties);
				return;
			}
			// Two-phase state, or the table route failed: plain flash below
		}

		// Update the internal variables in the state instance
		state->update(CoolProp::PSmass_INPUTS,p,s);

		// Set the values in the output structure
		// and store it for later calls with the same inputs
		if (this->postStateChange(properties))
			storeState(CACHE_PS, p, s, phase, properties);
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

	// Same inputs as a stored call: return the stored state
	bool hit = _stateCache.lookup(CACHE_HS, h, s, phase, properties);
	printCacheStatistics();
	if (hit) return;

	//this->preStateChange();

	try{
		// With enable_SVDSBTL, h-s inputs go to the reference equation of
		// state directly, there is no h-s table.
		if (enable_SVDSBTL) state->unspecify_phase();

		// Update the internal variables in the state instance
		state->update(CoolProp::HmassSmass_INPUTS,h,s);

		// Set the values in the output structure
		// and store it for later calls with the same inputs
		if (this->postStateChange(properties))
			storeState(CACHE_HS, h, s, phase, properties);
	}
	catch(std::exception &e)
	{
		errorMessage((char*)e.what());
	}
}

double CoolPropSolver::partialDeriv_state(const string &of, const string &wrt, const string &cst, ExternalThermodynamicState *const properties){
	if (debug_level > 5)
		std::cout << format("partialDeriv_state(of=%s,wrt=%s,cst=%s,state)\n",of.c_str(),wrt.c_str(),cst.c_str());

	// Translate the variable names before entering the try block, the
	// translation reports unknown names via errorMessage itself.
	CoolProp::parameters iOf  = static_cast<CoolProp::parameters>(derivParameter(of,  "Derivatives of"));
	CoolProp::parameters iWrt = static_cast<CoolProp::parameters>(derivParameter(wrt, "Derivatives with respect to"));
	CoolProp::parameters iCst = static_cast<CoolProp::parameters>(derivParameter(cst, "Derivatives at constant"));
	double res = NAN;

	try{
		// The SVDSBTL route can leave an imposed phase behind, release it
		if (enable_SVDSBTL) state->unspecify_phase();
		// Restore the state from the record. Incompressibles do not support
		// density-temperature inputs. With rho_smoothing_xend, postStateChange
		// replaces the density of two-phase states by a smoothed value, so d and T
		// no longer describe the state and p and h are used instead.
		if (!isCompressible)
			state->update(CoolProp::PT_INPUTS,properties->p,properties->T);
		else if (properties->phase == 2 && rho_smoothing_xend > 0.0)
			state->update(CoolProp::HmassP_INPUTS,properties->h,properties->p);
		else
			state->update(CoolProp::DmassT_INPUTS,properties->d,properties->T);

		if (isCompressible && state->phase() == CoolProp::iphase_twophase){
			// Inside the two-phase dome, the normal partial derivatives are not defined.
			// Apply the same smoothing as postStateChange, so that the result matches
			// ddph and ddhp in the state record. CoolProp can only smooth the
			// derivatives of density with respect to p and h.
			bool rho_ph = (iOf == CoolProp::iDmass)
				&& ((iWrt == CoolProp::iP && iCst == CoolProp::iHmass) || (iWrt == CoolProp::iHmass && iCst == CoolProp::iP));
			double Q = state->Q();
			double x_end = 0.0;
			if (Q >= 0 && Q <= twophase_derivsmoothing_xend && twophase_derivsmoothing_xend > 0.0)
				x_end = twophase_derivsmoothing_xend;
			else if (Q >= 0 && Q <= rho_smoothing_xend && rho_smoothing_xend > 0.0)
				x_end = rho_smoothing_xend;
			if (rho_ph && x_end > 0.0)
				res = state->first_two_phase_deriv_splined(iOf, iWrt, iCst, x_end);
			else
				res = state->first_two_phase_deriv(iOf, iWrt, iCst);
		}
		else
			res = state->first_partial_deriv(iOf, iWrt, iCst);
	} catch(std::exception &e) {
		errorMessage((char*)e.what());
	}
	return res;
}

long CoolPropSolver::derivParameter(const string &name, const char *role){
	// Map the ExternalMedia variable names to CoolProp parameter indices
	     if (!name.compare("p")){ return CoolProp::iP; }
	else if (!name.compare("T")){ return CoolProp::iT; }
	else if (!name.compare("d")){ return CoolProp::iDmass; }
	else if (!name.compare("h")){ return CoolProp::iHmass; }
	else if (!name.compare("s")){ return CoolProp::iSmass; }
	errorMessage((char*) format("Internal error: %s %s are not defined in the Solver object.",role,name.c_str()).c_str());
	return CoolProp::iundefined_parameter;
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
	// setSat_p and setSat_T fill properties->sigma, with NAN if CoolProp has no surface tension
	if (!ValidNumber(properties->sigma))
		errorMessage((char*)"CoolProp does not provide the surface tension for this fluid");
	return properties->sigma;
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

/*
 * SVDSBTL support
 *
 * The SVDSBTL backend of CoolProp 8 stores T, rho, h, s, u, the speed of
 * sound and the transport properties on compressed tables over the p-h,
 * p-T, p-s and d-T planes. It does not provide cp, cv or any derivative.
 *
 * A Helmholtz equation of state such as HEOS is explicit in (T, rho):
 * every property and every derivative at a given (T, rho) is a direct
 * evaluation. The expensive part of a HEOS call with (p, h) inputs is
 * the iteration that finds (T, rho), plus the check whether the state
 * lies inside the two-phase region.
 *
 * With enable_SVDSBTL the solver therefore uses the table only to find
 * T and rho, and evaluates the reference equation of state (`state`) at
 * that point with the phase imposed, so CoolProp skips its own iteration
 * and phase check. The table values of T and rho are not exact, so a
 * few Newton steps on HEOS correct them until p and h (or p and s) match
 * the inputs. All outputs, including cp, cv, kappa, beta, ddph, ddhp and
 * the transport properties, then come from HEOS at the input state, and
 * postStateChange is the same as without tables.
 *
 * Newton step for the inputs p_in and y_in (y = h or s), unknowns T, rho:
 *
 *   residuals    r_p = p(T, rho) - p_in,   r_y = y(T, rho) - y_in
 *   Jacobian     J = | dp/dT  dp/drho |    (partial derivatives of HEOS,
 *                    | dy/dT  dy/drho |     analytical)
 *   correction   (dT, drho) = J^-1 (r_p, r_y)
 *   new values   T = T - dT,   rho = rho - drho
 *
 * The iteration stops when kappa*|r_p| < SVDSBTL_TOL and
 * |r_y| < SVDSBTL_TOL*(|y_in| + scale). The pressure residual is measured
 * by the relative density change it corresponds to (kappa is the
 * isothermal compressibility), because p computed from (T, rho) of a
 * stiff liquid carries a round-off error that is larger than 1e-9 p at
 * low pressure.
 *
 * Two-phase states (the table says two-phase) and everything that fails
 * on the table route go to the plain flash of the reference equation of
 * state, which is as fast as the table route in the two-phase region.
 * So the two-phase behaviour, extend_twophase and the smoothing options
 * are the same as without tables.
 *
 * The saturation routines take T_sat and the saturated densities from
 * the table (SVDSBTL uses the CoolProp SuperAncillary functions for
 * these, which match HEOS to round-off), evaluate HEOS on the liquid and
 * on the vapour side, and compute the derivatives along the saturation
 * line from the Clausius-Clapeyron equation (see svdsbtl_sat).
 *
 * The measurements behind this design are in
 * Projects/Tests/svdsbtl_prototype_results.md.
 */

/// CoolProp phase label of a single-phase state at T [K], rho [kg/m3] and p [Pa].
/// SVDSBTL only reports whether a point is two-phase. Any single-phase
/// label makes HEOS skip its phase check at (T, rho), so a simple rule is
/// enough: supercritical above the critical temperature or pressure,
/// otherwise liquid if denser than the critical density, else vapour.
CoolProp::phases CoolPropSolver::svdsbtl_phase_label(double T_K, double rho_kg_m3, double p_Pa){
	if (T_K >= _fluidConstants.Tc && p_Pa >= _fluidConstants.pc) return CoolProp::iphase_supercritical;
	if (T_K >= _fluidConstants.Tc) return CoolProp::iphase_supercritical_gas;
	if (p_Pa >= _fluidConstants.pc) return CoolProp::iphase_supercritical_liquid;
	return (rho_kg_m3 > _fluidConstants.dc) ? CoolProp::iphase_liquid : CoolProp::iphase_gas;
}

/// Evaluate the reference equation of state at T [K] and rho [kg/m3] with the phase imposed
void CoolPropSolver::svdsbtl_eval_Trho(double T_K, double rho_kg_m3, CoolProp::phases phase){
	state->specify_phase(phase);
	state->update(CoolProp::DmassT_INPUTS, rho_kg_m3, T_K);
}

/// Single-phase state for the inputs p [Pa] and y_in, with y = h [J/kg]
/// (table_inputs = HmassP_INPUTS, y_key = iHmass) or y = s [J/(kg K)]
/// (PSmass_INPUTS, iSmass). Returns true if `state` holds the converged
/// state, with the phase still imposed. Returns false for two-phase
/// states and for any failure; the phase is then released.
bool CoolPropSolver::svdsbtl_solve_p_y(CoolProp::input_pairs table_inputs, double p_Pa, double y_in, CoolProp::parameters y_key, double y_scale){
	if (!table_state) return false;
	double T_K, rho_kg_m3;
	// 1. Starting values from the table
	try{
		if (table_inputs == CoolProp::HmassP_INPUTS)
			table_state->update(table_inputs, y_in, p_Pa);
		else
			table_state->update(table_inputs, p_Pa, y_in);
		if (table_state->phase() == CoolProp::iphase_twophase) return false;
		T_K = table_state->T();
		rho_kg_m3 = table_state->rhomass();
	}
	catch(std::exception &e){
		if (debug_level > 5) std::cout << format("SVDSBTL table failed at p = %g Pa, y = %g: %s\n", p_Pa, y_in, e.what());
		return false;
	}
	if (!ValidNumber(T_K) || !ValidNumber(rho_kg_m3) || T_K <= 0 || rho_kg_m3 <= 0) return false;
	const CoolProp::phases phase = svdsbtl_phase_label(T_K, rho_kg_m3, p_Pa);

	// 2. HEOS at the table point, then Newton steps until the inputs match
	try{
		svdsbtl_eval_Trho(T_K, rho_kg_m3, phase);
		for (int step = 0; ; step++){
			const double r_p = state->p() - p_Pa;
			const double r_y = state->keyed_output(y_key) - y_in;
			const double kappa = state->isothermal_compressibility();
			if (!ValidNumber(r_p) || !ValidNumber(r_y) || !ValidNumber(kappa)) break;
			if (std::abs(r_p*kappa) < SVDSBTL_TOL && std::abs(r_y) < SVDSBTL_TOL*(std::abs(y_in) + y_scale)){
				if (debug_level > 50) std::cout << format("SVDSBTL converged after %d Newton steps\n", step);
				return true;
			}
			if (step == SVDSBTL_MAX_STEPS) break;
			const double dp_dT   = state->first_partial_deriv(CoolProp::iP, CoolProp::iT, CoolProp::iDmass);
			const double dp_drho = state->first_partial_deriv(CoolProp::iP, CoolProp::iDmass, CoolProp::iT);
			const double dy_dT   = state->first_partial_deriv(y_key, CoolProp::iT, CoolProp::iDmass);
			const double dy_drho = state->first_partial_deriv(y_key, CoolProp::iDmass, CoolProp::iT);
			const double det = dp_dT*dy_drho - dp_drho*dy_dT;
			if (!ValidNumber(det) || det == 0) break;
			// Solve the 2x2 system J*(dT, drho) = (r_p, r_y) by Cramer's rule
			T_K       -= (r_p*dy_drho - r_y*dp_drho)/det;
			rho_kg_m3 -= (dp_dT*r_y - dy_dT*r_p)/det;
			if (!ValidNumber(T_K) || !ValidNumber(rho_kg_m3) || T_K <= 0 || rho_kg_m3 <= 0) break;
			svdsbtl_eval_Trho(T_K, rho_kg_m3, phase);
		}
	}
	catch(std::exception &e){
		if (debug_level > 5) std::cout << format("SVDSBTL Newton step failed at p = %g Pa, y = %g: %s\n", p_Pa, y_in, e.what());
	}
	if (debug_level > 5) std::cout << format("SVDSBTL did not converge at p = %g Pa, y = %g, using the plain flash\n", p_Pa, y_in);
	state->unspecify_phase();
	return false;
}

/// Single-phase state for the inputs p [Pa] and T [K], see svdsbtl_solve_p_y.
/// The SVDSBTL p-T table is a poor starting guess close to the saturation
/// line (within about 0.01 K it can return the density of the other
/// phase) and near the critical point (errors of 10 to 30 % in density).
/// So below the critical point the phase is decided by comparing T with
/// T_sat(p), and the table density is moved to the right side of the
/// saturated density if needed. Then a 1-D Newton iteration in rho at
/// fixed T solves p(T, rho) = p_in:
///   rho = rho - (p(T, rho) - p_in) / (dp/drho)_T
bool CoolPropSolver::svdsbtl_solve_pT(double p_Pa, double T_K){
	if (!table_state) return false;
	try{
		table_state->update(CoolProp::PT_INPUTS, p_Pa, T_K);
		double rho_kg_m3 = table_state->rhomass();
		if (!ValidNumber(rho_kg_m3) || rho_kg_m3 <= 0) return false;
		CoolProp::phases phase;
		if (p_Pa < _fluidConstants.pc && T_K < _fluidConstants.Tc){
			table_state->update(CoolProp::PQ_INPUTS, p_Pa, 0);
			const double T_sat_K = table_state->T();
			const double rho_l_kg_m3 = table_state->rhomass();
			table_state->update(CoolProp::PQ_INPUTS, p_Pa, 1);
			const double rho_v_kg_m3 = table_state->rhomass();
			if (T_K < T_sat_K){
				phase = CoolProp::iphase_liquid;
				rho_kg_m3 = std::max(rho_kg_m3, rho_l_kg_m3);
			}
			else{
				phase = CoolProp::iphase_gas;
				rho_kg_m3 = std::min(rho_kg_m3, rho_v_kg_m3);
			}
		}
		else{
			phase = svdsbtl_phase_label(T_K, rho_kg_m3, p_Pa);
		}
		for (int step = 0; step <= SVDSBTL_MAX_STEPS_PT; step++){
			svdsbtl_eval_Trho(T_K, rho_kg_m3, phase);
			const double r_p = state->p() - p_Pa;
			const double kappa = state->isothermal_compressibility();
			if (!ValidNumber(r_p) || !ValidNumber(kappa)) break;
			if (std::abs(r_p*kappa) < SVDSBTL_TOL) return true;
			const double dp_drho = state->first_partial_deriv(CoolProp::iP, CoolProp::iDmass, CoolProp::iT);
			if (!ValidNumber(dp_drho) || dp_drho == 0) break;
			rho_kg_m3 -= r_p/dp_drho;
			if (!ValidNumber(rho_kg_m3) || rho_kg_m3 <= 0) break;
		}
	}
	catch(std::exception &e){
		if (debug_level > 5) std::cout << format("SVDSBTL p-T failed at p = %g Pa, T = %g K: %s\n", p_Pa, T_K, e.what());
	}
	if (debug_level > 5) std::cout << format("SVDSBTL p-T did not converge at p = %g Pa, T = %g K, using the plain flash\n", p_Pa, T_K);
	state->unspecify_phase();
	return false;
}

/// Saturation properties from the reference equation of state at the
/// saturation pressure p [Pa], temperature T [K] and the saturated
/// densities rho_l and rho_v [kg/m3] from the table.
///
/// Along the saturation line T = T_sat(p). The slope follows from the
/// Clausius-Clapeyron equation
///   dT_sat/dp = T (v_v - v_l) / (h_v - h_l)
/// and the derivatives of the saturated density and enthalpy follow
/// from the single-phase derivatives on each side, for example
///   d rho_l/dp|sat = (d rho/dp)_T + (d rho/dT)_p * dT_sat/dp
/// Returns false if a value is not finite.
bool CoolPropSolver::svdsbtl_sat(double p_Pa, double T_K, double rho_l_kg_m3, double rho_v_kg_m3, ExternalSaturationProperties *const properties){
	if (!ValidNumber(p_Pa) || !ValidNumber(T_K) || !ValidNumber(rho_l_kg_m3) || !ValidNumber(rho_v_kg_m3)) return false;
	// Liquid side
	svdsbtl_eval_Trho(T_K, rho_l_kg_m3, CoolProp::iphase_liquid);
	const double h_l_J_kg = state->hmass();
	const double s_l_J_kgK = state->smass();
	const double drho_dp_T_l = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iT);
	const double drho_dT_p_l = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iT, CoolProp::iP);
	const double dh_dp_T_l   = state->first_partial_deriv(CoolProp::iHmass, CoolProp::iP, CoolProp::iT);
	const double dh_dT_p_l   = state->first_partial_deriv(CoolProp::iHmass, CoolProp::iT, CoolProp::iP);
	// Vapour side
	svdsbtl_eval_Trho(T_K, rho_v_kg_m3, CoolProp::iphase_gas);
	const double h_v_J_kg = state->hmass();
	const double s_v_J_kgK = state->smass();
	const double drho_dp_T_v = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iP, CoolProp::iT);
	const double drho_dT_p_v = state->first_partial_deriv(CoolProp::iDmass, CoolProp::iT, CoolProp::iP);
	const double dh_dp_T_v   = state->first_partial_deriv(CoolProp::iHmass, CoolProp::iP, CoolProp::iT);
	const double dh_dT_p_v   = state->first_partial_deriv(CoolProp::iHmass, CoolProp::iT, CoolProp::iP);
	// Surface tension: CoolProp only gives it for a state flagged as
	// two-phase. It depends on T only, and a QT update of HEOS is cheap
	// (SuperAncillary), so use one here.
	double sigma_N_m;
	try{
		state->specify_phase(CoolProp::iphase_twophase);
		state->update(CoolProp::QT_INPUTS, 0, T_K);
		sigma_N_m = state->surface_tension();
	}
	catch(...){
		sigma_N_m = NAN;
	}
	state->clear();
	state->unspecify_phase();

	// Clausius-Clapeyron
	const double dTp_K_Pa = T_K*(1.0/rho_v_kg_m3 - 1.0/rho_l_kg_m3)/(h_v_J_kg - h_l_J_kg);

	ExternalSaturationProperties sat = *properties;
	sat.psat  = p_Pa;
	sat.Tsat  = T_K;
	sat.dTp   = dTp_K_Pa;
	sat.dl    = rho_l_kg_m3;
	sat.hl    = h_l_J_kg;
	sat.sl    = s_l_J_kgK;
	sat.ddldp = drho_dp_T_l + drho_dT_p_l*dTp_K_Pa;
	sat.dhldp = dh_dp_T_l + dh_dT_p_l*dTp_K_Pa;
	sat.dv    = rho_v_kg_m3;
	sat.hv    = h_v_J_kg;
	sat.sv    = s_v_J_kgK;
	sat.ddvdp = drho_dp_T_v + drho_dT_p_v*dTp_K_Pa;
	sat.dhvdp = dh_dp_T_v + dh_dT_p_v*dTp_K_Pa;
	sat.sigma = sigma_N_m;
	const double check[] = {sat.dTp, sat.hl, sat.sl, sat.ddldp, sat.dhldp, sat.hv, sat.sv, sat.ddvdp, sat.dhvdp};
	for (double v : check){
		if (!ValidNumber(v)) return false;
	}
	*properties = sat;
	return true;
}

/// Saturation properties at p [Pa], false if the table route fails
bool CoolPropSolver::svdsbtl_setSat_p(double p_Pa, ExternalSaturationProperties *const properties){
	if (!table_state) return false;
	try{
		table_state->update(CoolProp::PQ_INPUTS, p_Pa, 0);
		const double T_K = table_state->T();
		const double rho_l_kg_m3 = table_state->rhomass();
		table_state->update(CoolProp::PQ_INPUTS, p_Pa, 1);
		const double rho_v_kg_m3 = table_state->rhomass();
		if (svdsbtl_sat(p_Pa, T_K, rho_l_kg_m3, rho_v_kg_m3, properties)) return true;
	}
	catch(std::exception &e){
		if (debug_level > 5) std::cout << format("SVDSBTL saturation failed at p = %g Pa: %s\n", p_Pa, e.what());
	}
	state->unspecify_phase();
	return false;
}

/// Saturation properties at T [K], false if the table route fails
bool CoolPropSolver::svdsbtl_setSat_T(double T_K, ExternalSaturationProperties *const properties){
	if (!table_state) return false;
	try{
		table_state->update(CoolProp::QT_INPUTS, 0, T_K);
		const double p_Pa = table_state->p();
		const double rho_l_kg_m3 = table_state->rhomass();
		table_state->update(CoolProp::QT_INPUTS, 1, T_K);
		const double rho_v_kg_m3 = table_state->rhomass();
		if (svdsbtl_sat(p_Pa, T_K, rho_l_kg_m3, rho_v_kg_m3, properties)) return true;
	}
	catch(std::exception &e){
		if (debug_level > 5) std::cout << format("SVDSBTL saturation failed at T = %g K: %s\n", T_K, e.what());
	}
	state->unspecify_phase();
	return false;
}

/// Interpolation routines
double CoolPropSolver::interp_linear(double Q, double valueL, double valueV) {
	return valueL+Q*(valueV-valueL);
}
double CoolPropSolver::interp_recip(double Q, double valueL, double valueV){
	return 1.0 / interp_linear(Q, 1.0/valueL, 1.0/valueV);
}

#endif
