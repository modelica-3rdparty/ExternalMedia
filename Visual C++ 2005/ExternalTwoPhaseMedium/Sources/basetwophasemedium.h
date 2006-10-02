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

// Define maximum relative difference between input values
#define EPSILON 1e-6

#include <string>
using std::string;

class BaseTwoPhaseMedium{
public:
	BaseTwoPhaseMedium(const string &mediumName);
	virtual ~BaseTwoPhaseMedium();

	virtual double beta() const;
	virtual double cp() const;
	virtual double cv() const;
	virtual double d() const;
	virtual double h() const;
	virtual double kappa() const;
	virtual double p() const;
	virtual double s() const;
	virtual double T() const;

	virtual double ps() const;
	virtual double Ts() const;

	virtual double dl() const;
	virtual double dv() const;
	virtual double hl() const;
	virtual double hv() const;
	virtual double sl() const;
	virtual double sv() const;

	virtual double dc() const;
	virtual double pc() const;
	virtual double Tc() const;

	virtual double MM() const;

	virtual double eta() const;
	virtual double lambda() const;
	virtual double Pr() const;
	virtual double sigma() const;

	virtual void setSat_p(const double &p) = 0;
	virtual void setSat_T(const double &T) = 0;

	virtual double saturationPressure(const double &T, const string &mediumName) = 0;
	virtual double saturationTemperature(const double &p, const string &mediumName) = 0;

	virtual void setState_dT(const double &d, const double &T, const int &phase) = 0;
	virtual void setState_ph(const double &p, const double &h, const int &phase) = 0;
	virtual void setState_ps(const double &p, const double &s, const int &phase) = 0;
	virtual void setState_pT(const double &p, const double &T) = 0;

	// Functions to check whether input values changed since last library call
	virtual bool inputIsEqual_p(const double &p);
	virtual bool inputIsEqual_T(const double &T);

	virtual bool inputsAreEqual_dT(const double &d, const double &T, const int &phase);
	virtual bool inputsAreEqual_ph(const double &p, const double &h, const int &phase);
	virtual bool inputsAreEqual_ps(const double &p, const double &s, const int &phase);
	virtual bool inputsAreEqual_pT(const double &p, const double &T);

protected:	
	// This function initializes the external library code
	// It should be only called once, by using firstCall
	virtual void initializeLibrary() const;
   
	// Static boolean to decide whether library has already been initialized
    static bool firstCall;

	string _mediumName;			// medium name
	int _phase;		// 2 for two-phase, 1 for one-phase, 0 if not known

	double _beta;	// isothermal expansion coefficient
	double _cp;		// specific heat capacity cp
	double _cv;		// specific heat capacity cv
	double _d;		// density
	double _h;		// specific enthalpy
	double _kappa;	// compressibility
	double _p;		// pressure
	double _s;		// specific entropy
	double _T;		// temperature

	double _ps;		// saturation pressure
	double _Ts;		// saturation temperature

	double _dl;		// bubble density
	double _dv;		// dew density
	double _hl;		// bubble specific enthalpy
	double _hv;		// dew specific enthalpy
	double _sl;		// bubble specific entropy
	double _sv;		// dew specific entropy

	double _dc;		// critical density
	double _pc;		// critical pressure
	double _Tc;		// critical temperature

	double _MM;		// molar mass

	double _eta;	// dynamic viscosity
	double _lambda;	// thermal conductivity
	double _Pr;		// Prandtl number
	double _sigma;	// surface tension
};

#endif /*BASETWOPHASEMEDIUM_H_*/
