/*
  C/C++ layer for external medium models extending from
  PartialExternalTwoPhaseMedium.
*/

#include "externalmedialib.h"
#include "basesolver.h"
#include "solvermap.h"
#include "errorhandling.h"
#include <math.h>

//! Get molar mass
/*!
  This function returns the molar mass of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getMolarMass_C_impl(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return molar mass
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->molarMass();
}

//! Get critical temperature
/*!
  This function returns the critical temperature of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getCriticalTemperature_C_impl(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return critical temperature
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalTemperature();
}

//! Get critical pressure
/*!
  This function returns the critical pressure of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getCriticalPressure_C_impl(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return critical pressure
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalPressure();
}

//! Get critical molar volume
/*!
  This function returns the critical molar volume of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getCriticalMolarVolume_C_impl(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return critical molar volume
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalMolarVolume();
}

//! Compute properties from p, h, and phase
/*!
  This function computes the properties for the specified inputs.
  @param p Pressure
  @param h Specific enthalpy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_ph_C_impl(double p, double h, int phase, void *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_ph(p, h, phase, static_cast<ExternalThermodynamicState*>(state));
}

//! Compute properties from p and T
/*!
  This function computes the properties for the specified inputs.

  Attention: The phase input is ignored for this function!
  @param p Pressure
  @param T Temperature
  @param state Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_pT_C_impl(double p, double T, void *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_pT(p, T, static_cast<ExternalThermodynamicState*>(state));
}

//! Compute properties from d, T, and phase
/*!
  This function computes the properties for the specified inputs.
  @param d Density
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_dT_C_impl(double d, double T, int phase, void *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_dT(d, T, phase, static_cast<ExternalThermodynamicState*>(state));
}

//! Compute properties from p, s, and phase
/*!
  This function computes the properties for the specified inputs.
  @param p Pressure
  @param s Specific entropy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_ps_C_impl(double p, double s, int phase, void *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_ps(p, s, phase, static_cast<ExternalThermodynamicState*>(state));
}

//! Compute properties from h, s, and phase
/*!
  This function computes the properties for the specified inputs.
  @param h Specific enthalpy
  @param s Specific entropy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_hs_C_impl(double h, double s, int phase, void *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_hs(h, s, phase, static_cast<ExternalThermodynamicState*>(state));
}

//! Compute partial derivative from a populated state record
/*!
  This function computes the derivative of the specified input.
  @param of Property to differentiate
  @param wrt Property to differentiate in
  @param cst Property to remain constant
  @param state Pointer to input values in state record
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_partialDeriv_state_C_impl(const char *of, const char *wrt, const char *cst,
		void *state, const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->partialDeriv_state(of, wrt, cst, static_cast<ExternalThermodynamicState*>(state));
}


//! Return Prandtl number of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_prandtlNumber_C_impl(void *state,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->Pr(static_cast<ExternalThermodynamicState*>(state));
}

//! Return temperature of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_temperature_C_impl(void *state,
								   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->T(static_cast<ExternalThermodynamicState*>(state));
}

//! Return velocity of sound of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_velocityOfSound_C_impl(void *state,
									   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->a(static_cast<ExternalThermodynamicState*>(state));
}

//! Return isobaric expansion coefficient of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_isobaricExpansionCoefficient_C_impl(void *state,
													const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->beta(static_cast<ExternalThermodynamicState*>(state));
}

//! Return specific heat capacity cp of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_specificHeatCapacityCp_C_impl(void *state,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->cp(static_cast<ExternalThermodynamicState*>(state));
}

//! Return specific heat capacity cv of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_specificHeatCapacityCv_C_impl(void *state,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->cv(static_cast<ExternalThermodynamicState*>(state));
}

//! Return density of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_density_C_impl(void *state,
							   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->d(static_cast<ExternalThermodynamicState*>(state));
}

//! Return derivative of density wrt specific enthalpy at constant pressure of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_density_derh_p_C_impl(void *state,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->ddhp(static_cast<ExternalThermodynamicState*>(state));
}

//! Return derivative of density wrt pressure at constant specific enthalpy of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_density_derp_h_C_impl(void *state,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->ddph(static_cast<ExternalThermodynamicState*>(state));
}

//! Return dynamic viscosity of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dynamicViscosity_C_impl(void *state,
										const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->eta(static_cast<ExternalThermodynamicState*>(state));
}

//! Return specific enthalpy of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_specificEnthalpy_C_impl(void *state,
										const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->h(static_cast<ExternalThermodynamicState*>(state));
}

//! Return isothermal compressibility of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_isothermalCompressibility_C_impl(void *state,
												 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->kappa(static_cast<ExternalThermodynamicState*>(state));
}

//! Return thermal conductivity of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_thermalConductivity_C_impl(void *state,
										   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->lambda(static_cast<ExternalThermodynamicState*>(state));
}

//! Return pressure of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_pressure_C_impl(void *state,
								const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->p(static_cast<ExternalThermodynamicState*>(state));
}

//! Return specific entropy of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_specificEntropy_C_impl(void *state,
									   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->s(static_cast<ExternalThermodynamicState*>(state));
}

//! Return derivative of density wrt pressure and specific enthalpy of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_density_ph_der_C_impl(void *state,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->d_der(static_cast<ExternalThermodynamicState*>(state));
}

//! Return the enthalpy at pressure p after an isentropic transformation from the specified medium state
double TwoPhaseMedium_isentropicEnthalpy_C_impl(double p_downstream, ExternalThermodynamicState *refState,
										  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->isentropicEnthalpy(p_downstream, refState);
}

//! Compute saturation properties from p
/*!
  This function computes the saturation properties for the specified inputs.
  @param p Pressure
  @param sat Pointer to return values for ExternalSaturationProperties struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setSat_p_C_impl(double p, void *sat,
							  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setSat_p(p, static_cast<ExternalSaturationProperties*>(sat));
}

//! Compute saturation properties from T
/*!
  This function computes the saturation properties for the specified inputs.
  @param T Temperature
  @param sat Pointer to return values for ExternalSaturationProperties struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setSat_T_C_impl(double T, void *sat,
							  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setSat_T(T, static_cast<ExternalSaturationProperties*>(sat));
}

//! Compute bubble state
/*!
  This function computes the  bubble state for the specified medium.
  @param sat Pointer to values of ExternalSaturationProperties struct
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setBubbleState_C_impl(void *sat, int phase, void *state,
									const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setBubbleState(static_cast<ExternalSaturationProperties*>(sat), phase, static_cast<ExternalThermodynamicState*>(state));
}

//! Compute dew state
/*!
  This function computes the dew state for the specified medium.
  @param sat Pointer to values of ExternalSaturationProperties struct
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param state Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setDewState_C_impl(void *sat, int phase, void *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setDewState(static_cast<ExternalSaturationProperties*>(sat), phase, static_cast<ExternalThermodynamicState*>(state));
}

//! Compute saturation temperature for specified medium and pressure
double TwoPhaseMedium_saturationTemperature_C_impl(double p, const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    ExternalSaturationProperties sat;
	solver->setSat_p(p, &sat);
	return sat.Tsat;
}

//! Compute derivative of saturation temperature for specified medium and pressure
double TwoPhaseMedium_saturationTemperature_derp_C_impl(double p, const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    ExternalSaturationProperties sat;
	solver->setSat_p(p, &sat);
	return sat.dTp;
}

//! Return derivative of saturation temperature of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_saturationTemperature_derp_sat_C_impl(void *sat,
													  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dTp(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return derivative of bubble density wrt pressure of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dBubbleDensity_dPressure_C_impl(void *sat,
												const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->ddldp(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return derivative of dew density wrt pressure of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dDewDensity_dPressure_C_impl(void *sat,
											 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->ddvdp(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return derivative of bubble specific enthalpy wrt pressure of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dBubbleEnthalpy_dPressure_C_impl(void *sat,
												 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dhldp(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return derivative of dew specific enthalpy wrt pressure of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dDewEnthalpy_dPressure_C_impl(void *sat,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dhvdp(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return bubble density of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_bubbleDensity_C_impl(void *sat,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dl(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return dew density of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dewDensity_C_impl(void *sat,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dv(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return bubble specific enthalpy of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_bubbleEnthalpy_C_impl(void *sat,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->hl(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return dew specific enthalpy of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dewEnthalpy_C_impl(void *sat,
								   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->hv(static_cast<ExternalSaturationProperties*>(sat));
}

//! Compute saturation pressure for specified medium and temperature
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_saturationPressure_C_impl(double T, const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    ExternalSaturationProperties sat;
	solver->setSat_T(T, &sat);
	return sat.psat;
}

//! Return surface tension of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_surfaceTension_C_impl(void *sat,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->sigma(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return bubble specific entropy of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_bubbleEntropy_C_impl(void *sat,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->sl(static_cast<ExternalSaturationProperties*>(sat));
}

//! Return dew specific entropy of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dewEntropy_C_impl(void *sat,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->sv(static_cast<ExternalSaturationProperties*>(sat));
}

// The following functions implement a workaround to handle ModelicaError and ModelicaWarning on Windows
// until a proper solution based on exporting symbols becomes available in Modelica tools
// pointers to ModelicaError and ModelicaWarning functions are passed from wrapper functions
// that are defined and compile in the Include annotation, which call these function, that in turn
// set global pointers for ModelicaError and ModelicaWarning. 
// These pointers are used by the errorMessage and warningMessage function when compiling for Windows

void TwoPhaseMedium_setState_ph_C_impl_err(double p, double h, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *))
{
    // Assign the global pointers to the function parameters, so they are initialized for all other functions
    #ifdef WIN32
    ::ModelicaErrorPtr = ModelicaErrorPtr;
    ::ModelicaWarningPtr = ModelicaWarningPtr;
    #endif
    // Call the actual C implementation function
    TwoPhaseMedium_setState_ph_C_impl(p, h, phase, static_cast<ExternalThermodynamicState*>(state), mediumName, libraryName, substanceName);
}

void TwoPhaseMedium_setState_pT_C_impl_err(double p, double T, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *))
{
    // Assign the global pointers to the function parameters, so they are initialized for all other functions
    #ifdef WIN32
    ::ModelicaErrorPtr = ModelicaErrorPtr;
    ::ModelicaWarningPtr = ModelicaWarningPtr;
    #endif
    // Call the actual C implementation function
    TwoPhaseMedium_setState_pT_C_impl(p, T, static_cast<ExternalThermodynamicState*>(state), mediumName, libraryName, substanceName);
}

void TwoPhaseMedium_setState_dT_C_impl_err(double d, double T, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *))
{
    // Assign the global pointers to the function parameters, so they are initialized for all other functions
    #ifdef WIN32
    ::ModelicaErrorPtr = ModelicaErrorPtr;
    ::ModelicaWarningPtr = ModelicaWarningPtr;
    #endif
    // Call the actual C implementation function
    TwoPhaseMedium_setState_dT_C_impl(d, T, phase, static_cast<ExternalThermodynamicState*>(state), mediumName, libraryName, substanceName);
}

void TwoPhaseMedium_setState_ps_C_impl_err(double p, double s, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *))
{
    // Assign the global pointers to the function parameters, so they are initialized for all other functions
    #ifdef WIN32
    ::ModelicaErrorPtr = ModelicaErrorPtr;
    ::ModelicaWarningPtr = ModelicaWarningPtr;
    #endif
    // Call the actual C implementation function
    TwoPhaseMedium_setState_ps_C_impl(p, s, phase, static_cast<ExternalThermodynamicState*>(state), mediumName, libraryName, substanceName);
}

void TwoPhaseMedium_setState_hs_C_impl_err(double h, double s, int phase, void *state, const char *mediumName, const char *libraryName, const char *substanceName, void (*ModelicaErrorPtr)(const char *), void (*ModelicaWarningPtr)(const char *))
{
    // Assign the global pointers to the function parameters, so they are initialized for all other functions
    #ifdef WIN32
    ::ModelicaErrorPtr = ModelicaErrorPtr;
    ::ModelicaWarningPtr = ModelicaWarningPtr;
    #endif
    // Call the actual C implementation function
    TwoPhaseMedium_setState_hs_C_impl(h, s, phase, static_cast<ExternalThermodynamicState*>(state), mediumName, libraryName, substanceName);
}
