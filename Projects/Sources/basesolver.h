#ifndef BASESOLVER_H_
#define BASESOLVER_H_

#include "externalmedialib.h"
#include "fluidconstants.h"
#include "include.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct FluidConstants;

/*! Base solver class. */
/*!
  This is the base class for all external solver objects
  (e.g. TestSolver, FluidPropSolver). A solver object
  encapsulates the interface to external fluid property
  computation routines

  Francesco Casella, Christoph Richter, Roberto Bonifetto
  2006-2012
  Copyright Politecnico di Milano, TU Braunschweig, Politecnico
  di Torino
*/
class BaseSolver {
public:
  BaseSolver(const string &mediumName, const string &libraryName,
             const string &substanceName);
  virtual ~BaseSolver();

  //! Return molar mass
  double molarMass() const;

  //! Return temperature at critical point
  double criticalTemperature() const;

  //! Return pressure at critical point
  double criticalPressure() const;

  //! Return molar volume at critical point
  double criticalMolarVolume() const;

  //! Return density at critical point
  double criticalDensity() const;

  //! Return specific enthalpy at critical point
  double criticalEnthalpy() const;

  //! Return specific entropy at critical point
  double criticalEntropy() const;

  //! Set fluid constants
  /*!
    This function sets the fluid constants which are defined in the
    FluidConstants record in Modelica. It should be called when a new
    solver is created.
  */
  virtual void setFluidConstants() = 0;

  //! Set state from p, h, and phase
  /*!
    This function sets the thermodynamic state record for the given pressure
    p, the specific enthalpy h and the specified phase. The computed values are
    written to the ExternalThermodynamicState property struct.

    Must be re-implemented in the specific solver
    @param p Pressure
    @param h Specific enthalpy
    @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
    @param properties ExternalThermodynamicState property struct
  */
  virtual void setState_ph(double &p, double &h, int &phase,
                           ExternalThermodynamicState *const properties) = 0;

  //! Set state from p and T
  /*!
    This function sets the thermodynamic state record for the given pressure
    p and the temperature T. The computed values are
    written to the ExternalThermodynamicState property struct.

    @param p Pressure
    @param T Temperature
    @param properties ExternalThermodynamicState property struct
  */
  virtual void setState_pT(double &p, double &T,
                           ExternalThermodynamicState *const properties) = 0;

  //! Set state from d, T, and phase
  /*!
    This function sets the thermodynamic state record for the given density
    d, the temperature T and the specified phase. The computed values are
    written to the ExternalThermodynamicState property struct.

    @param d Density
    @param T Temperature
    @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
    @param properties ExternalThermodynamicState property struct
  */
  virtual void setState_dT(double &d, double &T, int &phase,
                           ExternalThermodynamicState *const properties) = 0;

  //! Set state from p, s, and phase
  /*!
    This function sets the thermodynamic state record for the given pressure
    p, the specific entropy s and the specified phase. The computed values are
    written to the ExternalThermodynamicState property struct.

    @param p Pressure
    @param s Specific entropy
    @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
    @param properties ExternalThermodynamicState property struct
  */
  virtual void setState_ps(double &p, double &s, int &phase,
                           ExternalThermodynamicState *const properties) = 0;

  //! Set state from h, s, and phase
  /*!
    This function sets the thermodynamic state record for the given specific
    enthalpy p, the specific entropy s and the specified phase. The computed
    values are written to the ExternalThermodynamicState property struct.

    @param h Specific enthalpy
    @param s Specific entropy
    @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
    @param properties ExternalThermodynamicState property struct
  */
  virtual void setState_hs(double &h, double &s, int &phase,
                           ExternalThermodynamicState *const properties) = 0;

  //! Compute partial derivative from a populated state record
  /*!
    This function computes the derivative of the specified input. Note that it
    requires a populated state record as input.

    @param of Property to differentiate
    @param wrt Property to differentiate in
    @param cst Property to remain constant
    @param state Pointer to input values in state record
    @param mediumName Medium name
    @param libraryName Library name
    @param substanceName Substance name
  */
  virtual double
  partialDeriv_state(const string &of, const string &wrt, const string &cst,
                     ExternalThermodynamicState *const properties) = 0;

  //! Compute Prandtl number
  /*!
    This function returns the Prandtl number
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double Pr(ExternalThermodynamicState *const properties) = 0;
  //! Compute temperature
  /*!
    This function returns the temperature
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double T(ExternalThermodynamicState *const properties) = 0;

  //! Compute velocity of sound
  /*!
    This function returns the velocity of sound
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double a(ExternalThermodynamicState *const properties) = 0;
  //! Compute isobaric expansion coefficient
  /*!
    This function returns the isobaric expansion coefficient
    from the state specified by the properties input

    Must be re-implemented in the specific solver
    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double beta(ExternalThermodynamicState *const properties) = 0;
  //! Compute specific heat capacity cp
  /*!
    This function returns the specific heat capacity cp
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double cp(ExternalThermodynamicState *const properties) = 0;
  //! Compute specific heat capacity cv
  /*!
    This function returns the specific heat capacity cv
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double cv(ExternalThermodynamicState *const properties) = 0;
  //! Compute density
  /*!
    This function returns the density
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double d(ExternalThermodynamicState *const properties) = 0;
  //! Compute derivative of density wrt enthalpy at constant pressure
  /*!
    This function returns the derivative of density wrt enthalpy at constant
    pressure from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double ddhp(ExternalThermodynamicState *const properties) = 0;
  //! Compute derivative of density wrt pressure at constant enthalpy
  /*!
    This function returns the derivative of density wrt pressure at constant
    enthalpy from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double ddph(ExternalThermodynamicState *const properties) = 0;
  //! Compute dynamic viscosity
  /*!
    This function returns the dynamic viscosity
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double eta(ExternalThermodynamicState *const properties) = 0;
  //! Compute specific enthalpy
  /*!
    This function returns the specific enthalpy
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double h(ExternalThermodynamicState *const properties) = 0;
  //! Compute compressibility
  /*!
    This function returns the compressibility
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double kappa(ExternalThermodynamicState *const properties) = 0;

  //! Compute thermal conductivity
  /*!
    This function returns the thermal conductivity
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double lambda(ExternalThermodynamicState *const properties) = 0;
  //! Compute pressure
  /*!
    This function returns the pressure
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double p(ExternalThermodynamicState *const properties) = 0;
  //! Compute phase flag
  /*!
    This function returns the phase flag
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual int phase(ExternalThermodynamicState *const properties) = 0;
  //! Compute specific entropy
  /*!
    This function returns the specific entropy
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double s(ExternalThermodynamicState *const properties) = 0;
  //! Compute total derivative of density ph
  /*!
    This function returns the total derivative of density ph
    from the state specified by the properties input

    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double d_der(ExternalThermodynamicState *const properties) = 0;

  //! Compute isentropic enthalpy
  /*!
    This function returns the enthalpy at pressure p after an isentropic
    transformation from the state specified by the properties input

    @param p New pressure
    @param properties ExternalThermodynamicState property struct corresponding
    to current state
  */
  virtual double
  isentropicEnthalpy(double &p,
                     ExternalThermodynamicState *const properties) = 0;

  //! Set saturation properties from p
  /*!
    This function sets the saturation properties for the given pressure p.
    The computed values are written to the ExternalSaturationProperties propery
    struct.

    @param p Pressure
    @param properties ExternalSaturationProperties property struct
  */
  virtual void setSat_p(double &p,
                        ExternalSaturationProperties *const properties) = 0;
  //! Set saturation properties from T
  /*!
    This function sets the saturation properties for the given temperature T.
    The computed values are written to the ExternalSaturationProperties propery
    struct.

    @param T Temperature
    @param properties ExternalSaturationProperties property struct
  */
  virtual void setSat_T(double &T,
                        ExternalSaturationProperties *const properties) = 0;
  //! Set bubble state
  /*!
    This function sets the bubble state record bubbleProperties corresponding to
    the saturation data contained in the properties record.

    The default implementation of the setBubbleState function is relying on the
    correct behaviour of setState_ph with respect to the state input. Can be
    overridden in the specific solver code to get more efficient or correct
    handling of this situation.
    @param properties ExternalSaturationProperties record with saturation
    properties data
    @param phase Phase (1: one-phase, 2: two-phase)
    @param bubbleProperties ExternalThermodynamicState record where to write the
    bubble point properties
  */
  virtual void
  setBubbleState(ExternalSaturationProperties *const properties, int phase,
                 ExternalThermodynamicState *const bubbleProperties) = 0;
  //! Set dew state
  /*!
    This function sets the dew state record dewProperties corresponding to the
    saturation data contained in the properties record.

    The default implementation of the setDewState function is relying on the
    correct behaviour of setState_ph with respect to the state input. Can be
    overridden in the specific solver code to get more efficient or correct
    handling of this situation.
    @param properties ExternalSaturationProperties record with saturation
    properties data
    @param phase Phase (1: one-phase, 2: two-phase)
    @param dewProperties ExternalThermodynamicState record where to write the
    dew point properties
  */
  virtual void
  setDewState(ExternalSaturationProperties *const properties, int phase,
              ExternalThermodynamicState *const bubbleProperties) = 0;

  //! Compute derivative of Ts wrt pressure
  /*!
    This function returns the derivative of Ts wrt pressure
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double dTp(ExternalSaturationProperties *const properties) = 0;
  //! Compute derivative of dls wrt pressure
  /*!
    This function returns the derivative of dls wrt pressure
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double ddldp(ExternalSaturationProperties *const properties) = 0;
  //! Compute derivative of dvs wrt pressure
  /*!
    This function returns the derivative of dvs wrt pressure
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double ddvdp(ExternalSaturationProperties *const properties) = 0;
  //! Compute derivative of hls wrt pressure
  /*!
    This function returns the derivative of hls wrt pressure
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double dhldp(ExternalSaturationProperties *const properties) = 0;
  //! Compute derivative of hvs wrt pressure
  /*!
    This function returns the derivative of hvs wrt pressure
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double dhvdp(ExternalSaturationProperties *const properties) = 0;
  //! Compute density at bubble line
  /*!
    This function returns the density at bubble line
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double dl(ExternalSaturationProperties *const properties) = 0;
  //! Compute density at dew line
  /*!
    This function returns the density at dew line
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double dv(ExternalSaturationProperties *const properties) = 0;
  //! Compute enthalpy at bubble line
  /*!
    This function returns the enthalpy at bubble line
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double hl(ExternalSaturationProperties *const properties) = 0;
  //! Compute enthalpy at dew line
  /*!
    This function returns the enthalpy at dew line
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double hv(ExternalSaturationProperties *const properties) = 0;
  //! Compute surface tension
  /*!
    This function returns the surface tension
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double sigma(ExternalSaturationProperties *const properties) = 0;
  //! Compute entropy at bubble line
  /*!
    This function returns the entropy at bubble line
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double sl(ExternalSaturationProperties *const properties) = 0;
  //! Compute entropy at dew line
  /*!
    This function returns the entropy at dew line
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double sv(ExternalSaturationProperties *const properties) = 0;

  //! Compute derivatives
  /*!
    This function computes the derivatives according to the Bridgman's table.
    The computed values are written to the two phase medium property struct.
    This function can be called from within the setState_XX routines
    when implementing a new solver. Please be aware that cp, beta and
    kappa have to be provided to allow the computation of the derivatives. It
    returns false if the computation failed.

    Default implementation provided.
    @param properties ExternalThermodynamicState property record
  */
  virtual bool
  computeDerivatives(ExternalThermodynamicState *const properties);

  //! Compute saturation pressure
  /*!
    This function returns the saturation pressure
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double psat(ExternalSaturationProperties *const properties) = 0;
  //! Compute saturation temperature
  /*!
    This function returns the saturation temperature
    from the state specified by the properties input

    @param properties ExternalSaturationProperties property struct corresponding
    to current state
  */
  virtual double Tsat(ExternalSaturationProperties *const properties) = 0;

  /*! Medium name */
  string mediumName;
  /*! Library name */
  string libraryName;
  /*! Substance name */
  string substanceName;

protected:
  /*! Fluid constants */
  FluidConstants _fluidConstants;
};

#endif /* BASESOLVER_H_ */
