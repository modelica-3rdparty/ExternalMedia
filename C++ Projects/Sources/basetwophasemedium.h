/* *****************************************************************
 * Interface of the base class BaseTwoPhaseMedium.cpp
 *
 * The BaseTwoPhaseMedium class defines all the variables and member
 * functions which are needed to use external Modelica medium models
 * extending from PartialExternalTwoPhaseMedium.
 * 
 * Francesco Casella, Christoph Richter Sep 2006
 ********************************************************************/

#ifndef BASETWOPHASEMEDIUM_H_
#define BASETWOPHASEMEDIUM_H_

#include "include.h"

#include "basesolver.h"
#include "twophasemediumproperties.h"

class BaseTwoPhaseMedium{
public:
	BaseTwoPhaseMedium(const string &mediumName, const string &libraryName, 
					   const string &substanceName, BaseSolver *const solver,
					   const int &uniqueID);
	virtual ~BaseTwoPhaseMedium();

	int uniqueID() const;
	string mediumName() const;
	string libraryName() const;
	string substanceName() const;

	TwoPhaseMediumProperties *properties() const;

	BaseSolver *solver() const;
	virtual void setSolver(BaseSolver *const solver);

	double beta() const;
	double cp() const;
	double cv() const;
	double d() const;
	double dd_dp_h() const;
	double dd_dh_p() const;
	double h() const;
	double kappa() const;
	double p() const;
	double s() const;
	double T() const;

	double ps() const;
	double Ts() const;

	double dl() const;
	double dv() const;
	double hl() const;
	double hv() const;
	double sl() const;
	double sv() const;

    double d_Ts_dp() const;
	double d_dl_dp() const;
	double d_dv_dp() const;
	double d_hl_dp() const;
	double d_hv_dp() const;

	double eta() const;
	double lambda() const;
	double Pr() const;
	double sigma() const;

	virtual void setSat_p(double &p);
	virtual void setSat_T(double &T);
	virtual void setSat_p_state();

	virtual void setState_dT(double &d, double &T, int &phase);
	virtual void setState_ph(double &p, double &h, int &phase);
	virtual void setState_ps(double &p, double &s, int &phase);
	virtual void setState_pT(double &p, double &T);

protected:	
	// Pointer to medium property record
	TwoPhaseMediumProperties *_properties;

	// Pointer to solver
	BaseSolver *_solver;
};

#endif /*BASETWOPHASEMEDIUM_H_*/
