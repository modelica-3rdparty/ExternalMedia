#include "FluidProp_IF.h"

#pragma comment(lib, "comsupp.lib")


// {F30D147D-1F7C-4092-B481-ADE326A2ECD5}
//
static const GUID CLSID_FluidProp =
{ 0xF30D147DL, 0x1F7C, 0x4092,
{ 0xB4, 0x81, 0xAD, 0xE3, 0x26, 0xA2, 0xEC, 0xD5 } };

// {2430EE09-2C1E-4A86-AB62-CB67AEF6E484}
//
static const IID IID_IFluidProp =
{ 0x2430EE09L, 0x2C1E, 0x4A86,
{ 0xAB, 0x62, 0xCB, 0x67, 0xAE, 0xF6, 0xE4, 0x84 } };


CFluidProp::CFluidProp()
{
   // Init OLE
   CoInitialize(0);

   // Retrieve class factory interface pointer to our FluidProp COM object
   HRESULT hr = CoGetClassObject( CLSID_FluidProp, CLSCTX_INPROC_SERVER, 0,
                                  IID_IClassFactory, (void**)&ClassFactory);

   // Have class factory make the object for us - then release factory
   if (SUCCEEDED(hr))
   {
      ClassFactory->CreateInstance( 0, IID_IFluidProp, (void**)&FluidProp_COM);
      ClassFactory->Release();
      ClassFactory = 0;
   }
}

CFluidProp::~CFluidProp()
{
   // Free Fortran COM objects
   FluidProp_COM->ReleaseObjects();

   // Free FluidProp object
   FluidProp_COM->Release();

   // Unitialize OLE
   CoUninitialize();
}

void CFluidProp::SetFluid( char* ModelName, int nComp, char* Comp[], double* Conc, char*& ErrorMsg)
{
   // _com_util::Convert model name to binary string
   BSTR BSTR_Model = _com_util::ConvertStringToBSTR( ModelName);

   // _com_util::Convert character array Comp via binary strings (BSTR) into an OLE SafeArray
   SAFEARRAYBOUND sa_bounds_Comp[1];
   sa_bounds_Comp[0].lLbound = 0;
   sa_bounds_Comp[0].cElements = nComp;

   SAFEARRAY FAR* sa_Comp;
   sa_Comp = SafeArrayCreate( VT_BSTR, 1, sa_bounds_Comp);
   for( long i = 0; i < (long)nComp; i++)
   {
      BSTR BSTR_Comp = _com_util::ConvertStringToBSTR( Comp[i]);
      SafeArrayPutElement(sa_Comp, &i, BSTR_Comp);
   }

   // _com_util::Convert the double array Conc into an OLE SafeArray
   SAFEARRAYBOUND sa_bounds_Conc[1];
   sa_bounds_Conc[0].lLbound = 0;
   sa_bounds_Conc[0].cElements = nComp;

   SAFEARRAY FAR* sa_Conc;
   sa_Conc = SafeArrayCreate( VT_R8, 1, sa_bounds_Conc);
   for( i = 0; i < (long)nComp; i++)
      SafeArrayPutElement(sa_Conc, &i, &Conc[i]);

   // Now load the fluid parameters for the model selected.
   BSTR BSTR_Error;
   FluidProp_COM->SetFluid( BSTR_Model, nComp, &sa_Comp, &sa_Conc, &BSTR_Error);

   // Error handling
   ErrorMsg = _com_util::ConvertBSTRToString( BSTR_Error);
}

void CFluidProp::AllProps( char* InputSpec, double Input1, double Input2, double& P, double& T,
                           double& v, double& d, double& h, double& s, double& u, double& q,
                           double* x, double* y, double& cv, double& cp, double& c, double& alpha,
                           double& beta, double& chi, double& fi, double& ksi, double& psi,
                           double& gamma, double& eta, double& lambda, char*& ErrorMsg)
{
   BSTR BSTR_InputSpec = _com_util::ConvertStringToBSTR( InputSpec);
   BSTR BSTR_Error;

   // Convert double arrays with liquid and vapor phase compositions x and y into OLE SafeArrays
   SAFEARRAYBOUND sa_bounds_x[1], sa_bounds_y[1];
   sa_bounds_x[0].lLbound = 0;
   sa_bounds_y[0].lLbound = 0;
   sa_bounds_x[0].cElements = 20;
   sa_bounds_y[0].cElements = 20;
   SAFEARRAY *sa_x, *sa_y;
   sa_x = SafeArrayCreate( VT_R8, 1, sa_bounds_x);
   sa_y = SafeArrayCreate( VT_R8, 1, sa_bounds_y);

   FluidProp_COM->AllProps( BSTR_InputSpec, Input1, Input2, &P, &T, &v, &d, &h, &s, &u, &q, &sa_x,
                            &sa_y, &cv, &cp, &c, &alpha, &beta, &chi, &fi, &ksi, &psi, &gamma, &eta,
                            &lambda, &BSTR_Error);

   // Retrieve array with liquid and vapor phase compositions from SafeArrays
   for( long i = 0; i < (signed)sa_bounds_x[0].cElements; i++)
      SafeArrayGetElement( sa_x, &i, &x[i]);

   for( i = 0; i < (signed)sa_bounds_y[0].cElements; i++)
      SafeArrayGetElement( sa_y, &i, &y[i]);

   // Destroy the SafeArrays
   SafeArrayDestroy( sa_x);
   SafeArrayDestroy( sa_y);

   ErrorMsg = _com_util::ConvertBSTRToString( BSTR_Error);
}

//==================================================================================== EOF ===//


