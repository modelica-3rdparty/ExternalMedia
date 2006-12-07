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

	beta = 0.0;
	cp = 0.0;
	cv = 0.0;
	d = 0.0;
	dd_dp_h = 0.0;
	dd_dh_p = 0.0;
	h = 0.0;
	kappa = 0.0;
	p = 0.0;
	s = 0.0;
	T = 0.0;
	dT_dh_p = 0.0;
	dT_dp_h = 0.0;

	ps = 0.0;
	Ts = 0.0;

	dl = 0.0;
	dv = 0.0;
	hl = 0.0;
	hv = 0.0;
	sl = 0.0;
	sv = 0.0;

	eta = 0.0;
	lambda = 0.0;
	Pr = 0.0;
	sigma = 0.0;

    d_Ts_dp = 0.0;
	d_dl_dp = 0.0;
	d_dv_dp = 0.0;
    d_hl_dp = 0.0;
	d_hv_dp = 0.0;
}
