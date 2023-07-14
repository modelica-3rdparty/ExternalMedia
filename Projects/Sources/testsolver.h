#ifndef TESTSOLVER_H_
#define TESTSOLVER_H_

#include "basesolver.h"

/*! Test solver class */
/*!
  This class defines a dummy solver object, computing properties of
  a fluid roughly resembling warm water at low pressure, without
  the need of any further external code. The class is useful for debugging
  purposes, to test whether the C compiler and the Modelica tools are set
  up correctly before tackling problems with the actual - usually way more
  complex - external code. It is *not* meant to be used as an actual
  fluid model for any real application.

  To keep complexity down to the absolute medium, the current version of the
  solver can only compute the fluid properties in the liquid phase region:
      1e5 Pa < p < 2e5 Pa
      300 K  < T < 350 K ;
  results returned with inputs outside that range (possibly corresponding
  to two-phase or vapour points) are not reliable.
  Saturation properies are computed in the range
      1e5 Pa < psat < 2e5 Pa ;
  results obtained outside that range might be unrealistic.

  To instantiate this solver, it is necessary to set the library name package
  constant in Modelica as follows:

  libraryName = "TestMedium";

  Francesco Casella, Christoph Richter, Roberto Bonifetto
  2006-2012
  Copyright Politecnico di Milano, TU Braunschweig, Politecnico di Torino
*/
class TestSolver : public BaseSolver{
public:
	TestSolver(const string &mediumName, const string &libraryName, const string &substanceName);
	~TestSolver();
	virtual void setFluidConstants();

	virtual void setSat_p(double &p, ExternalSaturationProperties *const properties);
	virtual void setSat_T(double &T, ExternalSaturationProperties *const properties);

	virtual void setState_ph(double &p, double &h, int &phase, ExternalThermodynamicState *const properties);
	virtual void setState_pT(double &p, double &T, ExternalThermodynamicState *const properties);
	virtual void setState_dT(double &d, double &T, int &phase, ExternalThermodynamicState *const properties);
	virtual void setState_ps(double &p, double &s, int &phase, ExternalThermodynamicState *const properties);


  //to be implemented:

  virtual void setState_hs(double &h, double &s, int &phase, ExternalThermodynamicState *const properties);
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

 virtual double partialDeriv_state(const string &of, const string &wrt, const string &cst,
                           ExternalThermodynamicState *const properties);

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

#endif /* TESTSOLVER_H_ */
