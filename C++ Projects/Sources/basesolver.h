#ifndef BASESOLVER_H_
#define BASESOLVER_H_

#include "include.h"
#include "fluidconstants.h"


struct TwoPhaseMediumProperties;
struct FluidConstants;

//! Base solver class.
/*!
  This is the base class for all external solver objects
  (e.g. TestSolver, FluidPropSolver)

  Christoph Richter, Francesco Casella, Sep 2006
*/
class BaseSolver{
public:
	//! Constructor.
	/*!
	  The constructor is copying the medium name, library name and substance name
	  to the locally defined variables.
	  @param mediumName Arbitrary medium name
	  @param libraryName Name of the external fluid property library
	  @param substanceName Substance name
	*/
	BaseSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	//! Destructor
	/*!
	  The destructor for the base solver if currently not doing anything.
	*/
	virtual ~BaseSolver();

	//! Set fluid constants
	/*!
	  This function sets the fluid constants which are defined in the
	  FluidConstants record in Modelica. It should be called when a new
	  solver is created.
	*/
    virtual void setFluidConstants();

	//! Set saturation properties
	/*!
	  This function sets the saturation properties for the given pressure p.
	  The computed values are written to the two phase medium propery struct.
	  @param p Pressure
	  @param properties Two phase medium property record
	*/
	virtual void setSat_p(double &p, TwoPhaseMediumProperties *const properties);

	//! Set saturation properties
	/*!
	  This function sets the saturation properties for the given temperature T.
	  The computed values are written to the two phase medium propery struct.
	  @param T Temperature
	  @param properties Two phase medium property record
	*/
	virtual void setSat_T(double &T, TwoPhaseMediumProperties *const properties);

	//! Set saturation properties
	/*!
	  This function sets the saturation properties for the given pressure p
	  and is desined to be used from within the BaseProperties model in
	  Modelica. The computed values are written to the two phase medium propery 
	  struct.
	  @param properties Two phase medium property record
	*/
	virtual void setSat_p_state(TwoPhaseMediumProperties *const properties);

	//! Set state
	/*!
	  This function sets the thermodynamic state record for the given density
	  d, the temperature T and the specified phase. The computed values are
	  written to the two phase medium property struct.
	  @param d Density
	  @param T Temperature
	  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
	  @param properties Two phase medium property record
	*/
	virtual void setState_dT(double &d, double &T, int &phase, TwoPhaseMediumProperties *const properties);

	//! Set state
	/*!
	  This function sets the thermodynamic state record for the given pressure
	  p, the specific enthalpy h and the specified phase. The computed values are
	  written to the two phase medium property struct.
	  @param p Pressure
	  @param h Specific enthalpy
	  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
	  @param properties Two phase medium property record
	*/
	virtual void setState_ph(double &p, double &h, int &phase, TwoPhaseMediumProperties *const properties);

	//! Set state
	/*!
	  This function sets the thermodynamic state record for the given pressure
	  p, the specific entropy s and the specified phase. The computed values are
	  written to the two phase medium property struct.
	  @param p Pressure
	  @param s Specific entropy
	  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
	  @param properties Two phase medium property record
	*/
	virtual void setState_ps(double &p, double &s, int &phase, TwoPhaseMediumProperties *const properties);

	//! Set state
	/*!
	  This function sets the thermodynamic state record for the given pressure
	  p and the temperature T. The computed values are
	  written to the two phase medium property struct.
	  @param p Pressure
	  @param T Temperature
	  @param properties Two phase medium property record
	*/
	virtual void setState_pT(double &p, double &T, TwoPhaseMediumProperties *const properties);

    //! Set bubble state
	/*!
	  This function sets the bubble state record bubbleProperties corresponding to the 
	  saturation data contained in the properties record
	  @param phase Phase (1: one-phase, 2: two-phase)
	  @param properties Two phase medium property record with saturation properties data
	  @param bubbleProperties Two phase medium property record where to write the bubble point properties
	*/
	virtual void setBubbleState(int phase, TwoPhaseMediumProperties *const properties,
		                                   TwoPhaseMediumProperties *const bubbleProperties);
    
    //! Set dew state
	/*!
	  This function sets the dew state record dewProperties corresponding to the 
	  saturation data contained in the properties record
	  @param phase Phase (1: one-phase, 2: two-phase)
	  @param properties Two phase medium property record with saturation properties data
	  @param dewProperties Two phase medium property record where to write the dew point properties
	*/
	virtual void setDewState(int phase, TwoPhaseMediumProperties *const properties,
		                                TwoPhaseMediumProperties *const bubbleProperties);

	//! Compute derivatives
	/*!
	  This function computes the derivatives according to the Bridgman's table.
	  The computed values are written to the two phase medium property struct.
	  This function can be called from within the setState_XXX routines 
	  when implementing a new solver. Please be aware that cp, beta and
	  kappa have to be provided to allow the computation of the derivatives. It
	  returns false if the computation failed.
	  @param properties Two phase medium property record
	*/
	virtual bool computeDerivatives(TwoPhaseMediumProperties *const properties);

	//! Return molar mass
    double molarMass() const;
	//! Return temperature at critical point
	double criticalTemperature() const;
	//! Return pressure at critical point
	double criticalPressure() const;
	//! Return density at critical point
	double criticalDensity() const;
	//! Return molar volume at critical point
	double criticalMolarVolume() const;
	//! Return specific enthalpy at critical point
	double criticalEnthalpy() const;
	//! Return specific entropy at critical point
	double criticalEntropy() const;

	// Solver properties
	//! Medium name
	string mediumName;
	//! Library name
	string libraryName;
	//! Substance name
	string substanceName;

	//! Clear property struct
	/*!
	  This function sets all values in the property struct to
	  appropriate default values.
	*/
	void clearTwoPhaseMediumProperties(TwoPhaseMediumProperties *const properties);

protected:
	//! Fluid constants
	FluidConstants _fluidConstants; 
};

#endif /*BASESOLVER_H_*/
