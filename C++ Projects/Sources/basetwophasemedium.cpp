/* *****************************************************************
 * Implementation of the base class BaseTwoPhaseMedium
 *
 * The BaseTwoPhaseMedium class defines all the variables and member
 * functions which are needed to use external Modelica medium models
 * extending from PartialExternalTwoPhaseMedium.
 * 
 * The functions defined here are not fluid-specific, thus need not
 * be adapted to your own specific fluid property computation code.
 *
 * Francesco Casella, Christoph Richter Sep 2006
 ********************************************************************/

#include "basetwophasemedium.h"

#include <math.h>
#include "mediummap.h"

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

BaseTwoPhaseMedium::~BaseTwoPhaseMedium(){
}

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

int BaseTwoPhaseMedium::uniqueID() const{
	return _uniqueID;
}

string BaseTwoPhaseMedium::mediumName() const{
	return _mediumName;
}

string BaseTwoPhaseMedium::libraryName() const{
	return _libraryName;
}

string BaseTwoPhaseMedium::substanceName() const{
	return _substanceName;
}

TwoPhaseMediumProperties *BaseTwoPhaseMedium::properties() const{
	return _properties;
}

BaseSolver *BaseTwoPhaseMedium::solver() const{
	return _solver;
}

void BaseTwoPhaseMedium::setSolver(BaseSolver *const solver){
	_solver = solver;
}

double BaseTwoPhaseMedium::beta() const{
	if (ISNAN(_properties->beta))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->beta field was not set");
	return _properties->beta;
}

double BaseTwoPhaseMedium::cp() const{
	if (ISNAN(_properties->cp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->cp field was not set");
	return _properties->cp;
}

double BaseTwoPhaseMedium::cv() const{
	if (ISNAN(_properties->cv))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->cv field was not set");
	return _properties->cv;
}

double BaseTwoPhaseMedium::d() const{
	if (ISNAN(_properties->d))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d field was not set");
	return _properties->d;
}

double BaseTwoPhaseMedium::dd_dp_h() const{
	if (ISNAN(_properties->dd_dp_h))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dd_dp_h field was not set");
	return _properties->dd_dp_h;
}

double BaseTwoPhaseMedium::dd_dh_p() const{
	if (ISNAN(_properties->dd_dh_p))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dd_dh_p field was not set");
	return _properties->dd_dh_p;
}

double BaseTwoPhaseMedium::h() const{
	if (ISNAN(_properties->h))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->h field was not set");
	return _properties->h;
}

double BaseTwoPhaseMedium::kappa() const{
	if (ISNAN(_properties->kappa))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->kappa field was not set");
	return _properties->kappa;
}

double BaseTwoPhaseMedium::p() const{
	if (ISNAN(_properties->p))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->p field was not set");
	return _properties->p;
}

double BaseTwoPhaseMedium::s() const{
	if (ISNAN(_properties->s))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->s field was not set");
	return _properties->s;
}

double BaseTwoPhaseMedium::T() const{
	if (ISNAN(_properties->T))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->T field was not set");
	return _properties->T;
}

double BaseTwoPhaseMedium::dT_dp_h() const{
	if (ISNAN(_properties->dT_dp_h))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dT_dp_h field was not set");
	return _properties->dT_dp_h;
}

double BaseTwoPhaseMedium::dT_dh_p() const{
	if (ISNAN(_properties->dT_dh_p))
		errorMessage("Internal error in TwoPhaseMedium object:\ndT_dh_p field was not set");
	return _properties->dT_dh_p;
}

double BaseTwoPhaseMedium::ps() const{
	if (ISNAN(_properties->ps))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->ps field was not set");
	return _properties->ps;
}

double BaseTwoPhaseMedium::Ts() const{
	if (ISNAN(_properties->Ts))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->Ts field was not set");
	return _properties->Ts;
}

double BaseTwoPhaseMedium::dl() const{
	if (ISNAN(_properties->dl))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dl field was not set");
	return _properties->dl;
}

double BaseTwoPhaseMedium::dv() const{
	if (ISNAN(_properties->dv))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->dv field was not set");
	return _properties->dv;
}

double BaseTwoPhaseMedium::hl() const{
	if (ISNAN(_properties->hl))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->hl field was not set");
	return _properties->hl;
}

double BaseTwoPhaseMedium::hv() const{
	if (ISNAN(_properties->hv))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->hv field was not set");
	return _properties->hv;
}

double BaseTwoPhaseMedium::sl() const{
	if (ISNAN(_properties->sl))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->sl field was not set");
	return _properties->sl;
}

double BaseTwoPhaseMedium::sv() const{
	if (ISNAN(_properties->sv))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->sv field was not set");
	return _properties->sv;
}

double BaseTwoPhaseMedium::d_Ts_dp() const{
	if (ISNAN(_properties->d_Ts_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_Ts_dp field was not set");
	return _properties->d_Ts_dp;
}

double BaseTwoPhaseMedium::d_dl_dp() const{
	if (ISNAN(_properties->d_dl_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_dl_dp field was not set");
	return _properties->d_dl_dp;
}

double BaseTwoPhaseMedium::d_dv_dp() const{
	if (ISNAN(_properties->d_dv_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_dv_dp field was not set");
	return _properties->d_dv_dp;
}

double BaseTwoPhaseMedium::d_hl_dp() const{
	if (ISNAN(_properties->d_hl_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_hl_dp field was not set");
	return _properties->d_hl_dp;
}

double BaseTwoPhaseMedium::d_hv_dp() const{
	if (ISNAN(_properties->d_hv_dp))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->d_hv_dp field was not set");
	return _properties->d_hv_dp;
}

double BaseTwoPhaseMedium::eta() const{
	if (ISNAN(_properties->eta))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->eta field was not set");
	return _properties->eta;
}

double BaseTwoPhaseMedium::lambda() const{
	if (ISNAN(_properties->lambda))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->lambda field was not set");
	return _properties->lambda;
}

double BaseTwoPhaseMedium::Pr() const{
	if (ISNAN(_properties->Pr))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->Pr field was not set");
	return _properties->Pr;
}

double BaseTwoPhaseMedium::sigma() const{
	if (ISNAN(_properties->sigma))
		errorMessage("Internal error in TwoPhaseMedium object:\nthe _properties->sigma field was not set");
	return _properties->sigma;
}

void BaseTwoPhaseMedium::setSat_p(double &p){
	_solver->setSat_p(p, _properties);
}

void BaseTwoPhaseMedium::setSat_T(double &T){
	_solver->setSat_T(T, _properties);
}

void BaseTwoPhaseMedium::setSat_p_state(){
	_solver->setSat_p_state(_properties);
}

void BaseTwoPhaseMedium::setState_dT(double &d, double &T, int &phase){
	_solver->setState_dT(d, T, phase, _properties);
}

void BaseTwoPhaseMedium::setState_ph(double &p, double &h, int &phase){
	_solver->setState_ph(p, h, phase, _properties);
}

void BaseTwoPhaseMedium::setState_ps(double &p, double &s, int &phase){
	_solver->setState_ps(p, s, phase, _properties);
}

void BaseTwoPhaseMedium::setState_pT(double &p, double &T){
	_solver->setState_pT(p, T, _properties);
}

int BaseTwoPhaseMedium::getDewUniqueID(int phase){
	if (phase == 1){
        // Dew state on the one-phase side
		if (_dewUniqueIDOnePhase == 0){
			if (_uniqueID > 0)
				// allocate a permanent medium object for the dew state
				_dewUniqueIDOnePhase = 
					MediumMap::addMedium(_mediumName, 
						                 _libraryName,
										 _substanceName);
			else 
				// allocate a transient medium object for the dew state
				_dewUniqueIDOnePhase =
					MediumMap::addTransientMedium(_mediumName, 
												  _libraryName,
												  _substanceName);
		}
		// return the dew state unique ID
		return _dewUniqueIDOnePhase;
	}
	else {
		// Dew state on the two-phase side
		if (_dewUniqueIDTwoPhase == 0){
			if (_uniqueID > 0)
				// allocate a permanent medium object for the dew state
				_dewUniqueIDTwoPhase = 
					MediumMap::addMedium(_mediumName, 
						                 _libraryName,
										 _substanceName);
			else 
				// allocate a transient medium object for the dew state
				_dewUniqueIDTwoPhase =
					MediumMap::addTransientMedium(_mediumName, 
												  _libraryName,
												  _substanceName);
		}
		// return the dew state unique ID
		return _dewUniqueIDTwoPhase;
	}
}

int BaseTwoPhaseMedium::getBubbleUniqueID(int phase){
	if (phase == 1){
        // Bubble state on the one-phase side
		if (_bubbleUniqueIDOnePhase == 0){
			if (_uniqueID > 0)
				// allocate a permanent medium object for the bubble state
				_bubbleUniqueIDOnePhase = 
					MediumMap::addMedium(_mediumName, 
						                 _libraryName,
										 _substanceName);
			else 
				// allocate a transient medium object for the bubble state
				_bubbleUniqueIDOnePhase =
					MediumMap::addTransientMedium(_mediumName, 
												  _libraryName,
												  _substanceName);
		}
		// return the bubble state unique ID
		return _bubbleUniqueIDOnePhase;
	}
	else {
		// Bubble state on the two-phase side
		if (_bubbleUniqueIDTwoPhase == 0){
			if (_uniqueID > 0)
				// allocate a permanent medium object for the bubble state
				_bubbleUniqueIDTwoPhase = 
					MediumMap::addMedium(_mediumName, 
						                 _libraryName,
										 _substanceName);
			else 
				// allocate a transient medium object for the bubble state
				_bubbleUniqueIDTwoPhase =
					MediumMap::addTransientMedium(_mediumName, 
												  _libraryName,
												  _substanceName);
		}
		// return the bubble state unique ID
		return _bubbleUniqueIDTwoPhase;
	}
}

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
