#include "basetwophasemedium.h"
#include <math.h>
#include "mediummap.h"

//! Constructor
/*!
  The constructor initializes the following variables with the specified values:

  - _mediumName = mediumName
  - _libraryName = libraryName
  - _substanceName = substanceName
  - _solver = solver
  - _uniqueID = uniqueID
  - _dewUniqueIDOnePhase = 0
  - _dewUniqueIDTwoPhase = 0
  - _bubbleUniqueIDOnePhase = 0
  - _bubbleUniqueIDTwoPhase = 0
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
  @param solver Solver
  @param uniqueID Unique ID number
*/
BaseTwoPhaseMedium::BaseTwoPhaseMedium(const string &mediumName, const string &libraryName, 
									   const string &substanceName, BaseSolver *const solver, 
									   const int &uniqueID)
	: _mediumName(mediumName), _libraryName(libraryName), _substanceName(substanceName),
	  _solver(solver), 
	  _uniqueID(uniqueID),
	  _dewUniqueIDOnePhase(0),
	  _dewUniqueIDTwoPhase(0),
	  _bubbleUniqueIDOnePhase(0),
	  _bubbleUniqueIDTwoPhase(0) {
}

//! Destructor
/*!
  The destructor for the base solver if currently not doing anything.
*/
BaseTwoPhaseMedium::~BaseTwoPhaseMedium(){
}

//! Reinit medium constants
/*!
  This function reinites the medium constans to the inputs and the properties
  to their default values.
  @param mediumName Medium name
  @param libraryName Library name
  @param substanceName Substance name
  @param solver Solver
  @param uniqueID Unique ID number
  @see BaseTwoPhaseMedium()
*/
void BaseTwoPhaseMedium::reinitMedium(const string &mediumName, const string &libraryName, 
									  const string &substanceName, BaseSolver *const solver, 
									  const int &uniqueID){
	_mediumName = mediumName;
	_libraryName = libraryName;
	_substanceName = substanceName;
	_solver = solver;
	_uniqueID = uniqueID;
	_dewUniqueIDOnePhase = 0;
	_dewUniqueIDTwoPhase = 0;
	_bubbleUniqueIDOnePhase = 0;
	_bubbleUniqueIDTwoPhase = 0;
	_properties->initializeFields();
}

//! Return unique ID number
int BaseTwoPhaseMedium::uniqueID() const{
	return _uniqueID;
}

//! Return medium name
string BaseTwoPhaseMedium::mediumName() const{
	return _mediumName;
}

//! Return library name
string BaseTwoPhaseMedium::libraryName() const{
	return _libraryName;
}

//! Return substance name
string BaseTwoPhaseMedium::substanceName() const{
	return _substanceName;
}

//! Return pointer to properties
TwoPhaseMediumProperties *BaseTwoPhaseMedium::properties() const{
	return _properties;
}

//! Return pointer to solver
BaseSolver *BaseTwoPhaseMedium::solver() const{
	return _solver;
}

//! Set solver
/*!
  This functions ets the solver.
  @param solver New solver
*/
void BaseTwoPhaseMedium::setSolver(BaseSolver *const solver){
	_solver = solver;
}

//! Return phase (error handling included)
int BaseTwoPhaseMedium::phase() const{
	if (_properties->phase == 0)
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->phase field was not set");
	return _properties->phase;
}

//! Return isothermal expansion coefficient (error handling included)
double BaseTwoPhaseMedium::beta() const{
	if (ISNAN(_properties->beta))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->beta field was not set");
	return _properties->beta;
}

//! Return specific heat capacity cp (error handling included)
double BaseTwoPhaseMedium::cp() const{
	if (ISNAN(_properties->cp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->cp field was not set");
	return _properties->cp;
}

//! Return specific heat capacity cv (error handling included)
double BaseTwoPhaseMedium::cv() const{
	if (ISNAN(_properties->cv))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->cv field was not set");
	return _properties->cv;
}

//! Return density (error handling included)
double BaseTwoPhaseMedium::d() const{
	if (ISNAN(_properties->d))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d field was not set");
	return _properties->d;
}

//! Return derivative of density wrt pressure at constant specific enthalpy (error handling included)
double BaseTwoPhaseMedium::dd_dp_h() const{
	if (ISNAN(_properties->dd_dp_h))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dd_dp_h field was not set");
	return _properties->dd_dp_h;
}

//! Return derivative of density wrt specific enthalpy at constant pressure (error handling included)
double BaseTwoPhaseMedium::dd_dh_p() const{
	if (ISNAN(_properties->dd_dh_p))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dd_dh_p field was not set");
	return _properties->dd_dh_p;
}

//! Return specific enthalpy (error handling included)
double BaseTwoPhaseMedium::h() const{
	if (ISNAN(_properties->h))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->h field was not set");
	return _properties->h;
}

//! Return compressibility (error handling included)
double BaseTwoPhaseMedium::kappa() const{
	if (ISNAN(_properties->kappa))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->kappa field was not set");
	return _properties->kappa;
}

//! Return pressure (error handling included)
double BaseTwoPhaseMedium::p() const{
	if (ISNAN(_properties->p))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->p field was not set");
	return _properties->p;
}

//! Return specific entropy (error handling included)
double BaseTwoPhaseMedium::s() const{
	if (ISNAN(_properties->s))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->s field was not set");
	return _properties->s;
}

//! Return temperature (error handling included)
double BaseTwoPhaseMedium::T() const{
	if (ISNAN(_properties->T))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->T field was not set");
	return _properties->T;
}

//! Return derivative of temperature wrt pressure at constant specific enthalpy (error handling included)
double BaseTwoPhaseMedium::dT_dp_h() const{
	if (ISNAN(_properties->dT_dp_h))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dT_dp_h field was not set");
	return _properties->dT_dp_h;
}

//! Return derivative of temperature wrt specific enthalpy at constant pressure (error handling included)
double BaseTwoPhaseMedium::dT_dh_p() const{
	if (ISNAN(_properties->dT_dh_p))
		errorMessage("Internal error in TwoPhaseMedium object:\ndT_dh_p field was not set");
	return _properties->dT_dh_p;
}

//! Return saturation pressure (error handling included)
double BaseTwoPhaseMedium::ps() const{
	if (ISNAN(_properties->ps))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->ps field was not set");
	return _properties->ps;
}

//! Return saturation temperature (error handling included)
double BaseTwoPhaseMedium::Ts() const{
	if (ISNAN(_properties->Ts))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->Ts field was not set");
	return _properties->Ts;
}

//! Return bubble density (error handling included)
double BaseTwoPhaseMedium::dl() const{
	if (ISNAN(_properties->dl))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dl field was not set");
	return _properties->dl;
}

//! Return dew density (error handling included)
double BaseTwoPhaseMedium::dv() const{
	if (ISNAN(_properties->dv))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dv field was not set");
	return _properties->dv;
}

//! Return bubble specific enthalpy (error handling included)
double BaseTwoPhaseMedium::hl() const{
	if (ISNAN(_properties->hl))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->hl field was not set");
	return _properties->hl;
}

//! Return dew specific enthalpy (error handling included)
double BaseTwoPhaseMedium::hv() const{
	if (ISNAN(_properties->hv))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->hv field was not set");
	return _properties->hv;
}

//! Return bubble specific entropy (error handling included)
double BaseTwoPhaseMedium::sl() const{
	if (ISNAN(_properties->sl))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->sl field was not set");
	return _properties->sl;
}

//! Return dew specific entropy (error handling included)
double BaseTwoPhaseMedium::sv() const{
	if (ISNAN(_properties->sv))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->sv field was not set");
	return _properties->sv;
}

//! Return derivative of saturation temperature wrt pressure (error handling included)
double BaseTwoPhaseMedium::d_Ts_dp() const{
	if (ISNAN(_properties->d_Ts_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_Ts_dp field was not set");
	return _properties->d_Ts_dp;
}

//! Return derivative of bubble density wrt pressure (error handling included)
double BaseTwoPhaseMedium::d_dl_dp() const{
	if (ISNAN(_properties->d_dl_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_dl_dp field was not set");
	return _properties->d_dl_dp;
}

//! Return derivative of dew density wrt pressure (error handling included)
double BaseTwoPhaseMedium::d_dv_dp() const{
	if (ISNAN(_properties->d_dv_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_dv_dp field was not set");
	return _properties->d_dv_dp;
}

//! Return derivative of bubble specific enthalpy wrt pressure (error handling included)
double BaseTwoPhaseMedium::d_hl_dp() const{
	if (ISNAN(_properties->d_hl_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_hl_dp field was not set");
	return _properties->d_hl_dp;
}

//! Return derivative of dew specific enthalpy wrt pressure (error handling included)
double BaseTwoPhaseMedium::d_hv_dp() const{
	if (ISNAN(_properties->d_hv_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_hv_dp field was not set");
	return _properties->d_hv_dp;
}

//! Return dynamic viscosity (error handling included)
double BaseTwoPhaseMedium::eta() const{
	if (ISNAN(_properties->eta))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->eta field was not set");
	return _properties->eta;
}

//! Return thermal conductivity (error handling included)
double BaseTwoPhaseMedium::lambda() const{
	if (ISNAN(_properties->lambda))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->lambda field was not set");
	return _properties->lambda;
}

//! Return Prandtl number (error handling included)
double BaseTwoPhaseMedium::Pr() const{
	if (ISNAN(_properties->Pr))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->Pr field was not set");
	return _properties->Pr;
}

//! Return surface tension (error handling included)
double BaseTwoPhaseMedium::sigma() const{
	if (ISNAN(_properties->sigma))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->sigma field was not set");
	return _properties->sigma;
}

//! Set saturation properties from p
/*!
  This function calls the setSat_p() function of the solver.
  @param p Pressure
*/
void BaseTwoPhaseMedium::setSat_p(double &p){
	_solver->setSat_p(p, _properties);
}

//! Set saturation properties from T
/*!
  This function calls the setSat_T() function of the solver.
  @param T Temperature
*/
void BaseTwoPhaseMedium::setSat_T(double &T){
	_solver->setSat_T(T, _properties);
}

//! Set saturation properties
/*!
  This function calls the setSat_p_state() function of the solver and is designed
  to be used for function calls from within BaseProperties.
*/
void BaseTwoPhaseMedium::setSat_p_state(){
	_solver->setSat_p_state(_properties);
}

//! Set properties from d, T, and phase
/*!
  This function calls the setState_dT() function of the solver.
  @param d Density
  @param T Temperature
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
*/
void BaseTwoPhaseMedium::setState_dT(double &d, double &T, int &phase){
	_solver->setState_dT(d, T, phase, _properties);
}

//! Set properties from p, h, and phase
/*!
  This function calls the setState_ph() function of the solver.
  @param p Pressure
  @param h Specific enthalpy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
*/
void BaseTwoPhaseMedium::setState_ph(double &p, double &h, int &phase){
	_solver->setState_ph(p, h, phase, _properties);
}

//! Set properties from p, s, and phase
/*!
  This function calls the setState_ps() function of the solver.
  @param p Pressure
  @param s Specific entropy
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
*/
void BaseTwoPhaseMedium::setState_ps(double &p, double &s, int &phase){
	_solver->setState_ps(p, s, phase, _properties);
}

//! Set properties from p and T
/*!
  This function calls the setState_pT() function of the solver.
  @param p Pressure
  @param T Temperature
*/
void BaseTwoPhaseMedium::setState_pT(double &p, double &T){
	_solver->setState_pT(p, T, _properties);
}

//! Get bubble unique ID
/*!
  Get the unique ID of the bubble point medium object corresponding to psat, Tsat
  and the phase input, possibly allocating a medium object on the medium map if needed.
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
*/
int BaseTwoPhaseMedium::getBubbleUniqueID(int phase){
	if (phase == 1){
        // Bubble state on the one-phase side
		if (_bubbleUniqueIDOnePhase == 0){
			if (_uniqueID > 0)
				// allocate a permanent medium object for the bubble state
				_bubbleUniqueIDOnePhase = MediumMap::addMedium(_mediumName, _libraryName, _substanceName);
			else 
				// allocate a transient medium object for the bubble state
				_bubbleUniqueIDOnePhase = MediumMap::addTransientMedium(_mediumName, _libraryName, _substanceName);
		}
		// return the bubble state unique ID
		return _bubbleUniqueIDOnePhase;
	}
	else {
		// Bubble state on the two-phase side
		if (_bubbleUniqueIDTwoPhase == 0){
			if (_uniqueID > 0)
				// allocate a permanent medium object for the bubble state
				_bubbleUniqueIDTwoPhase = MediumMap::addMedium(_mediumName, _libraryName, _substanceName);
			else 
				// allocate a transient medium object for the bubble state
				_bubbleUniqueIDTwoPhase = MediumMap::addTransientMedium(_mediumName, _libraryName, _substanceName);
		}
		// return the bubble state unique ID
		return _bubbleUniqueIDTwoPhase;
	}
}

//! Get dew unique ID
/*!
  Get the unique ID of the dew point medium object corresponding to psat, Tsat
  and the phase input, possibly allocating a medium object on the medium map if needed.
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
*/
int BaseTwoPhaseMedium::getDewUniqueID(int phase){
	if (phase == 1){
        // Dew state on the one-phase side
		if (_dewUniqueIDOnePhase == 0){
			if (_uniqueID > 0)
				// allocate a permanent medium object for the dew state
				_dewUniqueIDOnePhase = MediumMap::addMedium(_mediumName, _libraryName, _substanceName);
			else 
				// allocate a transient medium object for the dew state
				_dewUniqueIDOnePhase = MediumMap::addTransientMedium(_mediumName, _libraryName, _substanceName);
		}
		// return the dew state unique ID
		return _dewUniqueIDOnePhase;
	}
	else {
		// Dew state on the two-phase side
		if (_dewUniqueIDTwoPhase == 0){
			if (_uniqueID > 0)
				// allocate a permanent medium object for the dew state
				_dewUniqueIDTwoPhase = MediumMap::addMedium(_mediumName, _libraryName, _substanceName);
			else 
				// allocate a transient medium object for the dew state
				_dewUniqueIDTwoPhase = MediumMap::addTransientMedium(_mediumName, _libraryName, _substanceName);
		}
		// return the dew state unique ID
		return _dewUniqueIDTwoPhase;
	}
}

//! Set bubble state
/*!
  Sets the properties of the bubble medium depending on the specified phase.
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
*/
void BaseTwoPhaseMedium::setBubbleState(int phase){
	// Get a pointer to the bubble state medium object
	BaseTwoPhaseMedium *bubbleMedium;
	if (phase == 1)
		bubbleMedium = MediumMap::medium(_bubbleUniqueIDOnePhase);
	else
		bubbleMedium = MediumMap::medium(_bubbleUniqueIDTwoPhase);
	// Call the solver to set the bubble state medium properties
	_solver->setBubbleState(phase, _properties, bubbleMedium->_properties);
}

//! Set dew state
/*!
  Sets the properties of the dew medium depending on the specified phase.
  @param phase Phase (2 for two-phase, 1 for one-phase, 0 if not known)
*/
void BaseTwoPhaseMedium::setDewState(int phase){
	// Get a pointer to the dew state medium object
	BaseTwoPhaseMedium *dewMedium;
	if (phase == 1)
		dewMedium = MediumMap::medium(_dewUniqueIDOnePhase);
	else
		dewMedium = MediumMap::medium(_dewUniqueIDTwoPhase);

	// Call the solver to set the dew state medium properties
	_solver->setDewState(phase, _properties, dewMedium->_properties);
}
