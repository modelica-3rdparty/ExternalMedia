/*
  C/C++ layer for external medium models extending from 
  PartialExternalTwoPhaseMedium.
*/

#include "externalmedialib.h"
#include "basesolver.h"
#include "solvermap.h"
#include <math.h>

//! Get molar mass
/*!
  This function returns the molar mass of the specified medium.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
double TwoPhaseMedium_getMolarMass_(const char *mediumName, const char *libraryName, const char *substanceName){
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
double TwoPhaseMedium_getCriticalTemperature_(const char *mediumName, const char *libraryName, const char *substanceName){
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
double TwoPhaseMedium_getCriticalPressure_(const char *mediumName, const char *libraryName, const char *substanceName){
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
double TwoPhaseMedium_getCriticalMolarVolume_(const char *mediumName, const char *libraryName, const char *substanceName){
	// Return critical molar volume
	return SolverMap::getSolver(mediumName, libraryName, substanceName)->criticalMolarVolume();
}

//! Compute properties from p, h, and phase
/*!
  This function computes the properties for the specified inputs.
  @param p Pressure
  @param h Specific enthalpy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param ExternalThermodynamicState Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_ph_(double p, double h, int phase, ExternalThermodynamicState *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_ph(p, h, phase, state);
}

//! Compute properties from p and T
/*!
  This function computes the properties for the specified inputs.

  Attention: The phase input is ignored for this function!
  @param p Pressure
  @param T Temperature
  @param ExternalThermodynamicState Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_pT_(double p, double T, ExternalThermodynamicState *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_pT(p, T, state);
}

//! Compute properties from d, T, and phase
/*!
  This function computes the properties for the specified inputs.
  @param d Density
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param ExternalThermodynamicState Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_dT_(double d, double T, int phase, ExternalThermodynamicState *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_dT(d, T, phase, state);
}

//! Compute properties from p, s, and phase
/*!
  This function computes the properties for the specified inputs.
  @param p Pressure
  @param s Specific entropy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param ExternalThermodynamicState Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setState_ps_(double p, double s, int phase, ExternalThermodynamicState *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setState_ps(p, s, phase, state);
}

//! Return Prandtl number of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_prandtlNumber_(ExternalThermodynamicState *state,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->Pr(state);
}

//! Return temperature of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_temperature_(ExternalThermodynamicState *state,
								   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->T(state);
}

//! Return velocity of sound of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_velocityOfSound_(ExternalThermodynamicState *state,
									   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->a(state);
}

//! Return isobaric expansion coefficient of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_isobaricExpansionCoefficient_(ExternalThermodynamicState *state,
													const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->beta(state);
}

//! Return specific heat capacity cp of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_specificHeatCapacityCp_(ExternalThermodynamicState *state,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->cp(state);
}

//! Return specific heat capacity cv of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_specificHeatCapacityCv_(ExternalThermodynamicState *state,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->cv(state);
}

//! Return density of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_density_(ExternalThermodynamicState *state,
							   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->d(state);
}

//! Return derivative of density wrt specific enthalpy at constant pressure of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_density_derh_p_(ExternalThermodynamicState *state,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->ddhp(state);
}

//! Return derivative of density wrt pressure at constant specific enthalpy of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_density_derp_h_(ExternalThermodynamicState *state,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->ddph(state);
}

//! Return dynamic viscosity of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dynamicViscosity_(ExternalThermodynamicState *state,
										const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->eta(state);
}

//! Return specific enthalpy of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_specificEnthalpy_(ExternalThermodynamicState *state,
										const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->h(state);
}

//! Return isothermal compressibility of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_isothermalCompressibility_(ExternalThermodynamicState *state,
												 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->kappa(state);
}

//! Return thermal conductivity of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_thermalConductivity_(ExternalThermodynamicState *state,
										   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->lambda(state);
}

//! Return pressure of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_pressure_(ExternalThermodynamicState *state,
								const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->p(state);
}

//! Return specific entropy of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_specificEntropy_(ExternalThermodynamicState *state,
									   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->s(state);
}

//! Return derivative of density wrt pressure and specific enthalpy of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_density_ph_der_(ExternalThermodynamicState *state,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->d_der(state);
}

//! Return the enthalpy at pressure p after an isentropic transformation from the specified medium state
double TwoPhaseMedium_isentropicEnthalpy_(double p_downstream, ExternalThermodynamicState *refState,
										  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->isentropicEnthalpy(p_downstream, refState);
}

//! Compute saturation properties from p
/*!
  This function computes the saturation properties for the specified inputs.
  @param p Pressure
  @param ExternalSaturationProperties Pointer to return values for ExternalSaturationProperties struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setSat_p_(double p, ExternalSaturationProperties *sat,
							  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setSat_p(p, sat);
}

//! Compute saturation properties from T
/*!
  This function computes the saturation properties for the specified inputs.
  @param T Temperature
  @param ExternalSaturationProperties Pointer to return values for ExternalSaturationProperties struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setSat_T_(double T, ExternalSaturationProperties *sat,
							  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setSat_T(T, sat);
}

//! Compute bubble state
/*!
  This function computes the  bubble state for the specified medium.
  @param ExternalSaturationProperties Pointer to values of ExternalSaturationProperties struct
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param ExternalThermodynamicState Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setBubbleState_(ExternalSaturationProperties *sat, int phase, ExternalThermodynamicState *state,
									const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setBubbleState(sat, phase, state);
}

//! Compute dew state
/*!
  This function computes the dew state for the specified medium.
  @param ExternalSaturationProperties Pointer to values of ExternalSaturationProperties struct
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
  @param ExternalThermodynamicState Pointer to return values for ExternalThermodynamicState struct
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
*/
void TwoPhaseMedium_setDewState_(ExternalSaturationProperties *sat, int phase, ExternalThermodynamicState *state,
								 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    solver->setDewState(sat, phase, state);
}

//! Compute saturation temperature for specified medium and pressure
double TwoPhaseMedium_saturationTemperature_(double p, const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    ExternalSaturationProperties sat;
	solver->setSat_p(p, &sat);
	return sat.Tsat;
}

//! Compute derivative of saturation temperature for specified medium and pressure
double TwoPhaseMedium_saturationTemperature_derp_(double p, const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    ExternalSaturationProperties sat;
	solver->setSat_p(p, &sat);
	return sat.dTp;
}

//! Return derivative of saturation temperature of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_saturationTemperature_derp_sat_(ExternalSaturationProperties *sat,
													  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dTp(sat);
}

//! Return derivative of bubble density wrt pressure of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dBubbleDensity_dPressure_(ExternalSaturationProperties *sat,
												const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->ddldp(sat);
}

//! Return derivative of dew density wrt pressure of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dDewDensity_dPressure_(ExternalSaturationProperties *sat,
											 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->ddvdp(sat);
}

//! Return derivative of bubble specific enthalpy wrt pressure of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dBubbleEnthalpy_dPressure_(ExternalSaturationProperties *sat,
												 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dhldp(sat);
}

//! Return derivative of dew specific enthalpy wrt pressure of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dDewEnthalpy_dPressure_(ExternalSaturationProperties *sat,
											  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dhvdp(sat);
}

//! Return bubble density of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_bubbleDensity_(ExternalSaturationProperties *sat,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dl(sat);
}

//! Return dew density of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dewDensity_(ExternalSaturationProperties *sat,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->dv(sat);
}

//! Return bubble specific enthalpy of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_bubbleEnthalpy_(ExternalSaturationProperties *sat,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->hl(sat);
}

//! Return dew specific enthalpy of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dewEnthalpy_(ExternalSaturationProperties *sat,
								   const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->hv(sat);
}

//! Compute saturation pressure for specified medium and temperature
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_saturationPressure_(double T, const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    ExternalSaturationProperties sat;
	solver->setSat_T(T, &sat);
	return sat.psat;
}

//! Return surface tension of specified medium
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_surfaceTension_(ExternalSaturationProperties *sat,
									  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->sigma(sat);
}

//! Return bubble specific entropy of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_bubbleEntropy_(ExternalSaturationProperties *sat,
									 const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->sl(sat);
}

//! Return dew specific entropy of specified medium from saturation properties
/*! Note: This function is not used by the default implementation of ExternalTwoPhaseMedium class.
    It might be used by external medium models customized solvers redeclaring the default functions
*/
double TwoPhaseMedium_dewEntropy_(ExternalSaturationProperties *sat,
								  const char *mediumName, const char *libraryName, const char *substanceName){
	BaseSolver *solver = SolverMap::getSolver(mediumName, libraryName, substanceName);
    return solver->sv(sat);
}
