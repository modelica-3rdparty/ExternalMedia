#include "twophasemediumproperties.h"

TwoPhaseMediumProperties::TwoPhaseMediumProperties(){
	// Initialize entire class
	mediumName = "";
	libraryName = "";
	substanceName = "";
	uniqueID = 0;

	phase = 0;

	dewUniqueIDOnePhase = 0;
	dewUniqueIDTwoPhase = 0;
	bubbleUniqueIDOnePhase = 0;
	bubbleUniqueIDTwoPhase = 0;

	beta = NAN;
	cp = NAN;
	cv = NAN;
	d = NAN;
	dd_dp_h = NAN;
	dd_dh_p = NAN;
	h = NAN;
	kappa = NAN;
	p = NAN;
	s = NAN;
	T = NAN;
	dT_dh_p = NAN;
	dT_dp_h = NAN;

	ps = NAN;
	Ts = NAN;

	dl = NAN;
	dv = NAN;
	hl = NAN;
	hv = NAN;
	sl = NAN;
	sv = NAN;

	eta = NAN;
	lambda = NAN;
	Pr = NAN;
	sigma = NAN;

    d_Ts_dp = NAN;
	d_dl_dp = NAN;
	d_dv_dp = NAN;
    d_hl_dp = NAN;
	d_hv_dp = NAN;
}
