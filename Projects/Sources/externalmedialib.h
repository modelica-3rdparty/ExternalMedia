/*! 
  \file externalmedialib.h
  \brief Header file to be included in Dymola
  
  This is the header file to be included in the Dymola/Source directory.
  It proived function prototypes for all the external functions needed
  by medium models extending from PartialExternalTwoPhaseMedium.

  Please be aware that other Modelica tools might require a slightly
  different header files depending on the compiler.

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/

#ifndef EXTERNALMEDIALIB_H_
#define EXTERNALMEDIALIB_H_

// Constants for input choices (see ExternalMedia.Common.InputChoices)
#define CHOICE_dT 1
#define CHOICE_ph 2
#define CHOICE_ps 3
#define CHOICE_pT 4

// Define struct
//! ExternalThermodynamicState property struct
/*!
  The ExternalThermodynamicState propery struct defines all the properties that
  are computed by external Modelica medium models extending from
  PartialExternalTwoPhaseMedium.

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/

typedef struct {

	//! Prandtl number
    double Pr;
	//! Temperature
	double T;
	//! Velocity of sound
	double a;
	//! Isobaric expansion coefficient
    double beta;
	//! Specific heat capacity cp
    double cp;
	//! Specific heat capacity cv
    double cv;
	//! Density
    double d;
	//! Derivative of density wrt enthalpy at constant pressure
    double ddhp;
	//! Derivative of density wrt pressure at constant enthalpy
    double ddph;
	//! Dynamic viscosity
    double eta;
	//! Specific enthalpy
    double h;
	//! Compressibility
    double kappa;
	//! Thermal conductivity
    double lambda;
	//! Pressure
    double p;
	//! Phase flag: 2 for two-phase, 1 for one-phase
    int phase;
	//! Specific entropy
    double s;

} ExternalThermodynamicState;

//! ExternalSaturationProperties property struct
/*!
  The ExternalSaturationProperties propery struct defines all the saturation properties
  for the dew and the bubble line that are computed by external Modelica medium models
  extending from PartialExternalTwoPhaseMedium. It also contains the  which makes it a
  real two phase medium property struct.

  Francesco Casella, Christoph Richter, Sep 2006
  Copyright Politecnico di Milano and TU Braunschweig
*/

typedef struct {

	//! Saturation temperature
    double Tsat;
	//! Derivative of Ts wrt pressure
    double dTp;
	//! Derivative of dls wrt pressure
    double ddldp;
	//! Derivative of dvs wrt pressure
    double ddvdp;
	//! Derivative of hls wrt pressure
    double dhldp;
	//! Derivative of hvs wrt pressure
    double dhvdp;
	//! Density at bubble line (for pressure ps)
    double dl;
	//! Density at dew line (for pressure ps)
    double dv;
	//! Specific enthalpy at bubble line (for pressure ps)
    double hl;
	//! Specific enthalpy at dew line (for pressure ps)
    double hv;
	//! Saturation pressure
    double psat;
	//! Surface tension
    double sigma;
	//! Specific entropy at bubble line (for pressure ps)
    double sl;
	//! Specific entropy at dew line (for pressure ps)
    double sv;

} ExternalSaturationProperties;

// Define export
#ifdef __cplusplus
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif // __cplusplus

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//	EXPORT int TwoPhaseMedium_createMedium_(const char *mediumName, const char *libraryName, const char *substanceName, int oldUniqueID);
	
	EXPORT double TwoPhaseMedium_getMolarMass_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_getCriticalTemperature_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_getCriticalPressure_(const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_getCriticalMolarVolume_(const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT void TwoPhaseMedium_setState_ph_(double p, double h, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setState_pT_(double p, double T, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setState_dT_(double d, double T, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setState_ps_(double p, double s, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double TwoPhaseMedium_prandtlNumber_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_temperature_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_velocityOfSound_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_isobaricExpansionCoefficient_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_specificHeatCapacityCp_(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_specificHeatCapacityCv_(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_density_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_density_derh_p_(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_density_derp_h_(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dynamicViscosity_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_specificEnthalpy_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_isothermalCompressibility_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_thermalConductivity_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_pressure_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_specificEntropy_(ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_density_ph_der_(ExternalThermodynamicState *state,	const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_isentropicEnthalpy_(double p_downstream, ExternalThermodynamicState *refState,	const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT void TwoPhaseMedium_setSat_p_(double p, ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setSat_T_(double T, ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setBubbleState_(ExternalSaturationProperties *sat, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT void TwoPhaseMedium_setDewState_(ExternalSaturationProperties *sat, int phase, ExternalThermodynamicState *state, const char *mediumName, const char *libraryName, const char *substanceName);
	
	EXPORT double TwoPhaseMedium_saturationTemperature_(double p, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_saturationTemperature_derp_(double p, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_saturationTemperature_derp_sat_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);

	EXPORT double TwoPhaseMedium_dBubbleDensity_dPressure_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dDewDensity_dPressure_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dBubbleEnthalpy_dPressure_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dDewEnthalpy_dPressure_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_bubbleDensity_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dewDensity_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_bubbleEnthalpy_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dewEnthalpy_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_saturationPressure_(double T, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_surfaceTension_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_bubbleEntropy_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	EXPORT double TwoPhaseMedium_dewEntropy_(ExternalSaturationProperties *sat, const char *mediumName, const char *libraryName, const char *substanceName);
	
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /*EXTERNALMEDIALIB_H_*/
