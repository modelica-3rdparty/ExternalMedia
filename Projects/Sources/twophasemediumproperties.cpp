#include "twophasemediumproperties.h"


// ! Constructor
/*!
  The constructor is calling initializeFields.
*/
TwoPhaseMediumProperties::ExternalThermodynamicState(){
    // Initialize entire class
	initializeFields();
}

TwoPhaseMediumProperties::ExternalSaturationProperties(){
    // Initialize entire class
	initializeFields();
}

// ! Initialize all fields to the default/NAN values
/*!
  This function initializes all fields of ExternalMediaThermodynamicState to the
  NAN value defined in include.h. The phase is initialized to zero.
*/
void ExternalThermodynamicState::initializeFields(){
	Pr = NAN;
	T = NAN;
	a = NAN;
	beta = NAN;
	cp = NAN;
	cv = NAN;
	d = NAN;
	ddhp = NAN;
	ddph = NAN;
	eta = NAN;
	h = NAN;
	kappa = NAN;
	lambda = NAN;
	p = NAN;
	phase = 0;
	s = NAN;
}

/*!
  This function initializes all fields of ExternalMediaSaturationProperties to the
  NAN value defined in include.h.
*/
void ExternalSaturationProperties::initializeFields(){
	Tsat = NAN;
    dTp = NAN;
	ddldp = NAN;
	ddvdp = NAN;
    dhldp = NAN;
	dhvdp = NAN;
	dl = NAN;
	dv = NAN;
	hl = NAN;
	hv = NAN;
	psat = NAN;
	sigma = NAN;
	sl = NAN;
	sv = NAN;
}
