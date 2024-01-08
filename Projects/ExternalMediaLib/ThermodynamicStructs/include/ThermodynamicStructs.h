#ifndef THERMODYNAMICSTRUCTS_H_
#define THERMODYNAMICSTRUCTS_H_

/* Define struct */
/*! ExternalThermodynamicState property struct */
/*!
  The ExternalThermodynamicState propery struct defines all the properties that
  are computed by external Modelica medium models extending from
  PartialExternalTwoPhaseMedium.
  Note: the exported interface functions
  use typeless void *state instead of ExternalThermodynamicState *state
  as Modelica does not treat external struct types so far.
*/

typedef struct ExternalThermodynamicState {

	/*! Temperature */
    double T;
	/*! Velocity of sound */
    double a;
	/*! Isobaric expansion coefficient */
    double beta;
	/*! Specific heat capacity cp */
    double cp;
	/*! Specific heat capacity cv */
    double cv;
	/*! Density */
    double d;
	/*! Derivative of density wrt enthalpy at constant pressure */
    double ddhp;
	/*! Derivative of density wrt pressure at constant enthalpy */
    double ddph;
	/*! Dynamic viscosity */
    double eta;
	/*! Specific enthalpy */
    double h;
	/*! Compressibility */
    double kappa;
	/*! Thermal conductivity */
    double lambda;
	/*! Pressure */
    double p;
	/*! Phase flag: 2 for two-phase, 1 for one-phase */
    int phase;
	/*! Specific entropy */
    double s;

	/*! Constructor. */
	/*!
	  The constructor only initializes the variables.
	*/
	#ifdef __cplusplus
	ExternalThermodynamicState() : T(-1), a(-1), beta(-1), cp(-1), cv(-1), d(-1), ddhp(-1), ddph(-1), eta(-1), h(-1), kappa(-1), lambda(-1), p(-1), phase(-1), s(-1) {};
	#endif

 } ExternalThermodynamicState;



 /*! ExternalSaturationProperties property struct */
/*!
  The ExternalSaturationProperties propery struct defines all the saturation properties
  for the dew and the bubble line that are computed by external Modelica medium models
  extending from PartialExternalTwoPhaseMedium.
  Note: the exported interface functions
  use typeless void *sat instead of ExternalSaturationProperties *sat
  as Modelica does not treat external struct types so far.
*/

typedef struct ExternalSaturationProperties {
	/*! Saturation temperature */
    double Tsat;
	/*! Derivative of Ts wrt pressure */
    double dTp;
	/*! Derivative of dls wrt pressure */
    double ddldp;
	/*! Derivative of dvs wrt pressure */
    double ddvdp;
	/*! Derivative of hls wrt pressure */
    double dhldp;
	/*! Derivative of hvs wrt pressure */
    double dhvdp;
	/*! Density at bubble line (for pressure ps) */
    double dl;
	/*! Density at dew line (for pressure ps) */
    double dv;
	/*! Specific enthalpy at bubble line (for pressure ps) */
    double hl;
	/*! Specific enthalpy at dew line (for pressure ps) */
    double hv;
	/*! Saturation pressure */
    double psat;
	/*! Surface tension */
    double sigma;
	/*! Specific entropy at bubble line (for pressure ps) */
    double sl;
	/*! Specific entropy at dew line (for pressure ps) */
    double sv;

	/*! Constructor. */
	/*!
	  The constructor only initializes the variables.
	*/
	#ifdef __cplusplus
	ExternalSaturationProperties() : Tsat(-1), dTp(-1), ddldp(-1), ddvdp(-1), dhldp(-1), dhvdp(-1), dl(-1), dv(-1), hl(-1), hv(-1), psat(-1), sigma(-1), sl(-1), sv(-1) {};
	#endif

} ExternalSaturationProperties;

#endif /* THERMODYNAMICSTRUCTS_H_ */
