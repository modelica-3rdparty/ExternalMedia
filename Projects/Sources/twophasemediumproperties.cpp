#include "twophasemediumproperties.h"


// ! Constructor
/*!
  The constructor is calling initializeFields.
*/
TwoPhaseMediumProperties::ExternalMediaThermodynamicState(){
    // Initialize entire class
	initializeFields();
}

TwoPhaseMediumProperties::ExternalMediaSaturationProperties(){
    // Initialize entire class
	initializeFields();
}

// ! Initialize all fields to the default/NAN values
/*!
  This function initializes all fields of ExternalMediaThermodynamicState to the
  NAN value defined in include.h. The phase is initialized to zero.
*/
void ExternalMediaThermodynamicState::initializeFields(){
	d = NAN;
	h = NAN;
	p = NAN;
	s = NAN;
	T = NAN;
	phase = 0;
	beta = NAN;
	cp = NAN;
	cv = NAN;
	ddph = NAN;
	ddhp = NAN;
	kappa = NAN;
	dT_dp_h = NAN;
	dT_dh_p = NAN;
	eta = NAN;
	lambda = NAN;
	Pr = NAN;
	a = NAN;
}

/*!
  This function initializes all fields of ExternalMediaSaturationProperties to the
  NAN value defined in include.h.
*/
void ExternalMediaSaturationProperties::initializeFields(){
	psat = NAN;
	Tsat = NAN;
	dl = NAN;
	dv = NAN;
	hl = NAN;
	hv = NAN;
	sl = NAN;
	sv = NAN;
	sigma = NAN;
    dTp = NAN;
	ddldp = NAN;
	ddvdp = NAN;
    dhldp = NAN;
	dhvdp = NAN;
}
