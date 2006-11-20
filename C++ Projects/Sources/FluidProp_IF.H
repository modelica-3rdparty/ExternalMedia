/* *****************************************************************
 * Interface of class CFluidProp
 *
 * This is a wrapper class around a FluidProp object
 *
 * Also requires FluidProp_IF.cpp and FluidProp_COM.h
 *
 * Teus van der Stelt, Sep 2006
 ********************************************************************/

#ifndef FluidProp_IF_h
#define FluidProp_IF_h

#ifndef   FluidProp_COM_h
#include "FluidProp_COM.h"
#endif

// The CFluidProp interface
//
class CFluidProp
{
   public:

      // Public ctor
	  CFluidProp();

	  // Public dtor
      ~CFluidProp();

	  // Set the fluid name and composition
      void SetFluid(const char* ModelName, int nComp, const char* Comp[], double* Conc, char*& ErrorMsg);

	  // Set the units of measurement
	  void SetUnits(const char* UnitSet, const char* MassOrMole, const char* Properties, const char* Units, char*& ErrorMsg);

	  // Compute all the properties at once 
	  void   AllProps( char* InputSpec, double Input1, double Input2, double& P, double& T,
                       double& v, double& d, double& h, double& s, double& u, double& q,
                       double* x, double* y, double& cv, double& cp, double& c, double& alpha,
                       double& beta, double& chi, double& fi, double& ksi, double& psi,
                       double& gamma, double& eta, double& lambda, char*& ErrorMsg);
   
   private:

      IClassFactory*  ClassFactory ;   		// Pointer to class factory
      IFluidProp_COM* FluidProp_COM;   		// Pointer to FluidProp interface
};


#endif


