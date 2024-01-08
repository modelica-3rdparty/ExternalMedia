#ifndef COOLPROPSOLVER_H_
#define COOLPROPSOLVER_H_

#include "include.h"

#include "basesolver.h"
#include "AbstractState.h"
#include "crossplatform_shared_ptr.h"

/*! CoolProp solver class */
/*!
  This class defines a solver that calls out to the open-source CoolProp
  property database and is partly inspired by the fluidpropsolver that
  was part of the first ExternalMedia release.


  libraryName = "CoolProp";

  Ian Bell (ian.h.bell@gmail.com)
  University of Liege,
  Liege, Belgium

  Jorrit Wronski (jowr@mek.dtu.dk)
  Technical University of Denmark,
  Kgs. Lyngby, Denmark

  2012-2014
*/
class CoolPropSolver : public BaseSolver{

protected:
	/* class CoolProp::AbstractState *state; */
	shared_ptr<CoolProp::AbstractState> state;
	bool enable_TTSE, enable_BICUBIC, calc_transport, extend_twophase, isCompressible;
	int debug_level;
	double twophase_derivsmoothing_xend;
	double rho_smoothing_xend;
	double _p_eps   ; /* relative tolerance margin for subcritical pressure conditions */
	double _delta_h ; /* delta_h for one-phase/two-phase discrimination */
	ExternalSaturationProperties _satPropsClose2Crit; /* saturation properties close to  critical conditions */

	virtual void postStateChange(ExternalThermodynamicState *const properties);
	long makeDerivString(const std::string &of, const std::string &wrt, const std::string &cst);
	double interp_linear(double Q, double valueL, double valueV);
	double interp_recip(double Q, double valueL, double valueV);

public:
	CoolPropSolver(const std::string &mediumName, const std::string &libraryName, const std::string &substanceName);
	~CoolPropSolver();
	virtual void setFluidConstants();

	virtual void setSat_p(double &p, ExternalSaturationProperties *const properties);
	virtual void setSat_T(double &T, ExternalSaturationProperties *const properties);

	virtual void setBubbleState(ExternalSaturationProperties *const properties, int phase, ExternalThermodynamicState *const bubbleProperties);
	virtual void setDewState   (ExternalSaturationProperties *const properties, int phase, ExternalThermodynamicState *const bubbleProperties);

	virtual void setState_ph(double &p, double &h, int &phase, ExternalThermodynamicState *const properties);
	virtual void setState_pT(double &p, double &T, ExternalThermodynamicState *const properties);
	virtual void setState_dT(double &d, double &T, int &phase, ExternalThermodynamicState *const properties);
	virtual void setState_ps(double &p, double &s, int &phase, ExternalThermodynamicState *const properties);
	virtual void setState_hs(double &h, double &s, int &phase, ExternalThermodynamicState *const properties);

	virtual double partialDeriv_state(const std::string &of, const std::string &wrt, const std::string &cst, ExternalThermodynamicState *const properties);

	virtual double Pr(ExternalThermodynamicState *const properties);
	virtual double T(ExternalThermodynamicState *const properties);
	virtual double a(ExternalThermodynamicState *const properties);
	virtual double beta(ExternalThermodynamicState *const properties);
	virtual double cp(ExternalThermodynamicState *const properties);
	virtual double cv(ExternalThermodynamicState *const properties);
	virtual double d(ExternalThermodynamicState *const properties);
	virtual double ddhp(ExternalThermodynamicState *const properties);
	virtual double ddph(ExternalThermodynamicState *const properties);
	virtual double eta(ExternalThermodynamicState *const properties);
	virtual double h(ExternalThermodynamicState *const properties);
	virtual double kappa(ExternalThermodynamicState *const properties);
	virtual double lambda(ExternalThermodynamicState *const properties);
	virtual double p(ExternalThermodynamicState *const properties);
	virtual int phase(ExternalThermodynamicState *const properties);
	virtual double s(ExternalThermodynamicState *const properties);
	virtual double d_der(ExternalThermodynamicState *const properties);
	virtual double isentropicEnthalpy(double &p, ExternalThermodynamicState *const properties);

	virtual double dTp(ExternalSaturationProperties *const properties);
	virtual double ddldp(ExternalSaturationProperties *const properties);
	virtual double ddvdp(ExternalSaturationProperties *const properties);
	virtual double dhldp(ExternalSaturationProperties *const properties);
	virtual double dhvdp(ExternalSaturationProperties *const properties);
	virtual double dl(ExternalSaturationProperties *const properties);
	virtual double dv(ExternalSaturationProperties *const properties);
	virtual double hl(ExternalSaturationProperties *const properties);
	virtual double hv(ExternalSaturationProperties *const properties);
	virtual double sigma(ExternalSaturationProperties *const properties);
	virtual double sl(ExternalSaturationProperties *const properties);
	virtual double sv(ExternalSaturationProperties *const properties);

	virtual double psat(ExternalSaturationProperties *const properties);
	virtual double Tsat(ExternalSaturationProperties *const properties);

};

#endif /* COOLPROPSOLVER_H_ */
