//============================================================================//
//                                                                            //
//                          FluidProp C++ interface                           //
//                          -----------------------                           //
//                                                                            //
//  The class defined in this file, TFluidProp, is as a wrapper class for     //
//  the IFluidProp_COM interface.  TFluidProp hides COM specific details      //
//  like safe arrays (SAFEARRAY) and binary strings (BSTR) in IFluidProp_COM. //
//  In the TFluidProp class only standard C++ data types are used.  This is   //
//  the recommended way working with the FluidProp COM server in C++.         //
//                                                                            //
//  July, 2004, for FluidProp 1                                               //
//  January, 2006, for FluidProp 2                                            //
//  April, 2007, for FluidProp 2.3                                            //
//  November, 2012, for FluidProp 2.5                                         //
//                                                                            //
//============================================================================//

#include <iostream>
#include "include.h"
#include "FluidProp_IF.h"

#if defined(__MINGW32__)
#include <olectl.h>
#endif

// Conversion of a binary string BSTR to a string.
static inline string ConvertBSTRToString(BSTR BString)
{
   char* tmp_string = _com_util::ConvertBSTRToString(BString);
   string retval = tmp_string;
   delete[] tmp_string;
   return retval;
}
// Conversion of a string to a binary string BSTR.
static inline BSTR ConvertStringToBSTR(string &String)
{
   return _com_util::ConvertStringToBSTR(String.c_str());
}


// {F30D147D-1F7C-4092-B481-ADE326A2ECD5}
static const GUID CLSID_FluidProp =
{ 0xF30D147DL, 0x1F7C, 0x4092,
{ 0xB4, 0x81, 0xAD, 0xE3, 0x26, 0xA2, 0xEC, 0xD5 } };

// {2430EE09-2C1E-4A86-AB62-CB67AEF6E484}
static const IID IID_IFluidProp =
{ 0x2430EE09L, 0x2C1E, 0x4A86,
{ 0xAB, 0x62, 0xCB, 0x67, 0xAE, 0xF6, 0xE4, 0x84 } };


TFluidProp::TFluidProp() : FluidProp_COM(NULL)
{
   // Init COM
   CoInitialize(0);

   // Retrieve class factory interface pointer to our FluidProp COM object
   HRESULT hr = CoGetClassObject(CLSID_FluidProp, CLSCTX_INPROC_SERVER, 0, IID_IClassFactory, (void**) &ClassFactory);

   // Have class factory make the object for us - then release factory
   if (SUCCEEDED(hr)) {
      HRESULT hr2 = ClassFactory->CreateInstance(0, IID_IFluidProp, (void**) &FluidProp_COM);
      ClassFactory->Release();
      ClassFactory = 0;
      if (FAILED(hr2)) {
         if (hr2 == CLASS_E_NOTLICENSED)
            std::cout << "FluidProp license not valid." << std::endl;
         std::cout << "Failed to create FluidProp COM object" << std::endl;
      }
   }
}

TFluidProp::~TFluidProp()
{
   // Free FluidProp object
   if (FluidProp_COM)
      FluidProp_COM->Release();

   // Unitialize COM
   CoUninitialize();
}

bool TFluidProp::IsValid()
{
   return !!FluidProp_COM;
}

void TFluidProp::CreateObject( string ModelName, string* ErrorMsg)
{
   BSTR BSTR_Model = ConvertStringToBSTR(ModelName);
   BSTR BSTR_Error;

   FluidProp_COM->CreateObject(BSTR_Model, &BSTR_Error);
   SysFreeString(BSTR_Model);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}

void TFluidProp::ReleaseObjects()
{
   FluidProp_COM->ReleaseObjects();
}

void TFluidProp::SetFluid( string ModelName, int nComp, string* Comp, double* Conc, string* ErrorMsg)
{
   // Convert model name to binary string
   BSTR BSTR_Model = ConvertStringToBSTR(ModelName);

   // Convert character array Comp via binary strings (BSTR) into a SafeArray
   SAFEARRAY FAR* sa_Comp = SafeArrayCreateVector(VT_BSTR, 1, nComp);
   for (long i = 1; i <= (long)nComp; i++) {
      BSTR BSTR_Comp = ConvertStringToBSTR(Comp[i-1]);
      SafeArrayPutElement(sa_Comp, &i, BSTR_Comp);
      SysFreeString(BSTR_Comp);
   }

   // Convert the double array Conc into a SafeArray
   SAFEARRAY FAR* sa_Conc = SafeArrayCreateVector(VT_R8, 1, nComp);
   for (long i = 1; i <= (long)nComp; i++)
      SafeArrayPutElement(sa_Conc, &i, &Conc[i-1]);

   // Now load the fluid parameters for the model selected.
   BSTR BSTR_Error;
   FluidProp_COM->SetFluid( BSTR_Model, nComp, &sa_Comp, &sa_Conc, &BSTR_Error);
   SysFreeString(BSTR_Model);
   SafeArrayDestroy(sa_Comp);
   SafeArrayDestroy(sa_Conc);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}

void TFluidProp::GetFluid( string* ModelName, int* nComp, string* Comp, double* Conc, bool CompInfo)
{
   // When CompInfo is true, the components and their concentrations in the mixture are returned
   // instead of the mixture name.

   long long_nComp = *nComp;

   // Convert character array Comp via binary strings (BSTR) into a SafeArray
   // This needs to be done because in the COM interface Comp is an inout argument,
   // because for out arguments an assumed array size is not allowed...
   SAFEARRAY FAR* sa_Comp = SafeArrayCreateVector(VT_BSTR, 1, long_nComp);
   for (long i = 1; i <= long_nComp; i++) {
      BSTR BSTR_Comp = ConvertStringToBSTR(Comp[i-1]);
      SafeArrayPutElement(sa_Comp, &i, BSTR_Comp);
      SysFreeString(BSTR_Comp);
   }

   // Convert the double array Conc into a SafeArray
   // This needs to be done because in the COM interface Conc is an inout argument,
   // because for out arguments an assumed array size is not allowed...
   SAFEARRAY FAR* sa_Conc = SafeArrayCreateVector(VT_R8, 1, long_nComp);
   for (long i = 1; i <= long_nComp; i++)
      SafeArrayPutElement(sa_Conc, &i, &Conc[i-1]);

   // Now retrieve the fluid parameters set wit SetFluid
   string blank = string(" ");
   BSTR BSTR_Model = ConvertStringToBSTR(blank);
   if (CompInfo)
      long_nComp = -1;
   FluidProp_COM->GetFluid( BSTR_Model, &long_nComp, &sa_Comp, &sa_Conc);

   // Convert model name from binary string to string
   string TmpName = ConvertBSTRToString(BSTR_Model);
   SysFreeString(BSTR_Model);
   *ModelName = TmpName;

   // Convert from long to int
   *nComp = long_nComp;

   // Put the values in the string array Comp
   for (long i = 1; i <= long_nComp; i++) {
      BSTR BSTR_Comp;
      SafeArrayGetElement(sa_Comp, &i, &BSTR_Comp);
      Comp[i-1] = ConvertBSTRToString(BSTR_Comp);
      SysFreeString(BSTR_Comp);
   }
   SafeArrayDestroy(sa_Comp);

   // Put the values in the double array Conc
   for (long i = 1; i <= long_nComp; i++)
      SafeArrayGetElement(sa_Conc, &i, &Conc[i-1]);
   SafeArrayDestroy(sa_Conc);
}

void TFluidProp::GetFluidNames( string LongShort, string ModelName, int* nFluids,
                                string* FluidNames, string* ErrorMsg)
{
   long long_nFluids;

   // Get available fluids
   BSTR BSTR_Model = ConvertStringToBSTR(ModelName);
   BSTR BSTR_LongShort = ConvertStringToBSTR(LongShort);
   BSTR BSTR_Error;

   // Convert character array FluidNames via binary strings (BSTR) into a SafeArray
   // This needs to be done because in the COM interface FluidNames is an inout argument,
   // because Visual Basic is not able to deal out arrays...
   SAFEARRAY* sa_FluidNames = SafeArrayCreateVector(VT_BSTR, 1, 250);

   FluidProp_COM->GetFluidNames(BSTR_LongShort, BSTR_Model, &long_nFluids, &sa_FluidNames, &BSTR_Error);
   SysFreeString(BSTR_Model);
   SysFreeString(BSTR_LongShort);

   // Retrieve array with components from SafeArray
   for (long i = 1; i <= long_nFluids; i++) {
      BSTR BSTR_Fluid;
      SafeArrayGetElement(sa_FluidNames, &i, &BSTR_Fluid);
      FluidNames[i-1] = ConvertBSTRToString(BSTR_Fluid);
      SysFreeString(BSTR_Fluid);
   }
   SafeArrayDestroy(sa_FluidNames);
   *nFluids = long_nFluids;

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}

void TFluidProp::GetCompSet( string ModelName, int* nComps, string* CompSet, string* ErrorMsg)
{
   long long_nComps;

   // Get available fluids
   BSTR BSTR_Model = ConvertStringToBSTR(ModelName);
   BSTR BSTR_Error;

   // Convert character array CompSet via binary strings (BSTR) into a SafeArray
   // This needs to be done because in the COM interface CompSet is an inout argument,
   // because Visual Basic is not able to deal out arrays...
   SAFEARRAY* sa_CompSet = SafeArrayCreateVector(VT_BSTR, 1, 250);

   FluidProp_COM->GetCompSet( BSTR_Model, &long_nComps, &sa_CompSet, &BSTR_Error);
   SysFreeString(BSTR_Model);

   // Retrieve array with components from SafeArray
   for (long i = 1; i <= long_nComps; i++) {
      BSTR BSTR_Comp;
      SafeArrayGetElement(sa_CompSet, &i, &BSTR_Comp);
      CompSet[i-1] = ConvertBSTRToString(BSTR_Comp);
      SysFreeString(BSTR_Comp);
   }
   SafeArrayDestroy(sa_CompSet);
   *nComps = long_nComps;

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}

double TFluidProp::Pressure( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Pressure( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Temperature( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Temperature( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::SpecVolume( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->SpecVolume( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Density( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Density( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Enthalpy( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Enthalpy( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Entropy( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Entropy( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::IntEnergy( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->IntEnergy( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::VaporQual( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->VaporQual( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double* TFluidProp::LiquidCmp( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double* Output = new double[20];

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert the double array Conc into a SafeArray
   SAFEARRAY* sa_Output = SafeArrayCreateVector(VT_R8, 1, 20);

   FluidProp_COM->LiquidCmp( BSTR_InputSpec, Input1, Input2, &sa_Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with concentrations from SafeArray
   for (long i = 1; i <= 20; i++)
      SafeArrayGetElement(sa_Output, &i, &Output[i-1]);
   SafeArrayDestroy(sa_Output);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double* TFluidProp::VaporCmp( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double* Output = new double[20];

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert the double array Conc into a SafeArray
   SAFEARRAY* sa_Output = SafeArrayCreateVector(VT_R8, 1, 20);

   FluidProp_COM->VaporCmp( BSTR_InputSpec, Input1, Input2, &sa_Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with concentrations from SafeArray
   for (long i = 1; i <= 20; i++)
      SafeArrayGetElement(sa_Output, &i, &Output[i-1]);
   SafeArrayDestroy(sa_Output);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::HeatCapV( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->HeatCapV( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::HeatCapP( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->HeatCapP( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::SoundSpeed( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->SoundSpeed( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Alpha( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Alpha( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Beta( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Beta( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Chi( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Chi( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Fi( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Fi( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Ksi( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Ksi( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Psi( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Psi( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Zeta( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Zeta( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Theta( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Theta( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Kappa( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Kappa( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Gamma( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Gamma( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Viscosity( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Viscosity( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::ThermCond( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->ThermCond( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

void TFluidProp::AllProps( string InputSpec, double Input1, double Input2, double& P, double& T,
                           double& v, double& d, double& h, double& s, double& u, double& q,
                           double* x, double* y, double& cv, double& cp, double& c, double& alpha,
                           double& beta, double& chi, double& fi, double& ksi, double& psi,
                           double& zeta, double& theta, double& kappa, double& gamma, double& eta,
                           double& lambda, string* ErrorMsg)
{
   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert double arrays with liquid and vapor phase compositions x and y into SafeArrays
   SAFEARRAY *sa_x = SafeArrayCreateVector(VT_R8, 1, 20);
   SAFEARRAY *sa_y = SafeArrayCreateVector(VT_R8, 1, 20);

   FluidProp_COM->AllProps( BSTR_InputSpec, Input1, Input2, &P, &T, &v, &d, &h, &s, &u, &q, &sa_x,
                            &sa_y, &cv, &cp, &c, &alpha, &beta, &chi, &fi, &ksi, &psi, &zeta,
                            &theta, &kappa, &gamma, &eta, &lambda, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with liquid and vapor phase compositions from SafeArrays
   for (long i = 1; i <= 20; i++)
      SafeArrayGetElement(sa_x, &i, &x[i-1]);
   SafeArrayDestroy(sa_x);

   for (long i = 1; i <= 20; i++)
      SafeArrayGetElement(sa_y, &i, &y[i-1]);
   SafeArrayDestroy(sa_y);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}

void TFluidProp::AllPropsSat( string InputSpec, double Input1, double Input2, double& P, double& T,
                              double& v, double& d, double& h, double& s, double& u, double& q,
                              double* x, double* y, double& cv, double& cp, double& c, double& alpha,
                              double& beta, double& chi, double& fi, double& ksi, double& psi,
                              double& zeta, double& theta, double& kappa, double& gamma, double& eta,
                              double& lambda, double& d_liq, double& d_vap, double& h_liq, double& h_vap,
                              double& T_sat, double& dd_liq_dP, double& dd_vap_dP, double& dh_liq_dP,
                              double& dh_vap_dP, double& dT_sat_dP, string* ErrorMsg)
{
   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert double arrays with liquid and vapor phase compositions x and y into SafeArrays
   SAFEARRAY *sa_x = SafeArrayCreateVector(VT_R8, 1, 20);
   SAFEARRAY *sa_y = SafeArrayCreateVector(VT_R8, 1, 20);

   FluidProp_COM->AllPropsSat( BSTR_InputSpec, Input1, Input2, &P, &T, &v, &d, &h, &s, &u, &q, &sa_x,
                               &sa_y, &cv, &cp, &c, &alpha, &beta, &chi, &fi, &ksi, &psi, &zeta,
                               &theta, &kappa, &gamma, &eta, &lambda, &d_liq, &d_vap, &h_liq, &h_vap,
                               &T_sat, &dd_liq_dP, &dd_vap_dP, &dh_liq_dP, &dh_vap_dP, &dT_sat_dP,
                               &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with liquid and vapor phase compositions from SafeArrays
   for (long i = 1; i <= 20; i++)
      SafeArrayGetElement(sa_x, &i, &x[i-1]);
   SafeArrayDestroy(sa_x);

   for (long i = 1; i <= 20; i++)
      SafeArrayGetElement(sa_y, &i, &y[i-1]);
   SafeArrayDestroy(sa_y);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}


double TFluidProp::Solve( string FuncSpec, double FuncVal, string InputSpec, long Target,
                          double FixedVal, double MinVal, double MaxVal, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_FuncSpec = ConvertStringToBSTR(FuncSpec);
   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->Solve( BSTR_FuncSpec, FuncVal, BSTR_InputSpec, Target, FixedVal, MinVal,
                         MaxVal, &Output, &BSTR_Error);
   SysFreeString(BSTR_FuncSpec);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Mmol( string* ErrorMsg)
{
   double Output;

   BSTR BSTR_Error;

   FluidProp_COM->Mmol( &Output, &BSTR_Error);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Tcrit( string* ErrorMsg)
{
   double Output;

   BSTR BSTR_Error;

   FluidProp_COM->Tcrit( &Output, &BSTR_Error);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Pcrit( string* ErrorMsg)
{
   double Output;

   BSTR BSTR_Error;

   FluidProp_COM->Pcrit( &Output, &BSTR_Error);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Tmin( string* ErrorMsg)
{
   double Output;

   BSTR BSTR_Error;

   FluidProp_COM->Tmin( &Output, &BSTR_Error);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::Tmax( string* ErrorMsg)
{
   double Output;

   BSTR BSTR_Error;

   FluidProp_COM->Tmax( &Output, &BSTR_Error);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

void TFluidProp::AllInfo( double& Mmol, double& Tcrit, double& Pcrit, double& Tmin, double& Tmax,
                          string* ErrorMsg)
{
   BSTR BSTR_Error;

   FluidProp_COM->AllInfo( &Mmol, &Tcrit, &Pcrit, &Tmin, &Tmax, &BSTR_Error);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}

void TFluidProp::SetUnits( string UnitSet, string MassOrMole, string Properties, string Units,
                           string* ErrorMsg)
{
   BSTR BSTR_Error;
   BSTR BSTR_UnitSet = ConvertStringToBSTR(UnitSet);
   BSTR BSTR_MassOrMole = ConvertStringToBSTR(MassOrMole);
   BSTR BSTR_Properties = ConvertStringToBSTR(Properties);
   BSTR BSTR_Units = ConvertStringToBSTR(Units);

   FluidProp_COM->SetUnits( BSTR_UnitSet, BSTR_MassOrMole, BSTR_Properties, BSTR_Units, &BSTR_Error);
   SysFreeString(BSTR_UnitSet);
   SysFreeString(BSTR_MassOrMole);
   SysFreeString(BSTR_Properties);
   SysFreeString(BSTR_Units);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}

void TFluidProp::SetRefState( double T_ref, double P_ref, string* ErrorMsg)
{
   BSTR BSTR_Error;

   FluidProp_COM->SetRefState( T_ref, P_ref, &BSTR_Error);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}

void TFluidProp::GetVersion(string ModelName, int* version)
{
   // Convert model name to binary string
   BSTR BSTR_Model = ConvertStringToBSTR(ModelName);

   // Now get the version for the model selected
   SAFEARRAY* sa_version;
   FluidProp_COM->GetVersion(BSTR_Model, &sa_version);
   SysFreeString(BSTR_Model);

   for (long i = 1; i <= 4; i++) {
      SafeArrayGetElement(sa_version, &i, &version[i-1]);
   }
   SafeArrayDestroyData(sa_version);
   SafeArrayDestroyDescriptor(sa_version);
}

double* TFluidProp::FugaCoef( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double* Output = new double[20];

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert the double array Conc into a SafeArray
   SAFEARRAYBOUND sa_bounds_Output[1];
   sa_bounds_Output[0].lLbound = 0;
   sa_bounds_Output[0].cElements = 20;
   SAFEARRAY* sa_Output;
   sa_Output = SafeArrayCreate(VT_R8, 1, sa_bounds_Output);

   FluidProp_COM->FugaCoef( BSTR_InputSpec, Input1, Input2, &sa_Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with concentrations from SafeArray
   for (long i = 0; i < (signed)sa_bounds_Output[0].cElements; i++)
      SafeArrayGetElement(sa_Output, &i, &Output[i]);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::SurfTens( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->SurfTens( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double TFluidProp::GibbsEnergy( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double Output;

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   FluidProp_COM->GibbsEnergy( BSTR_InputSpec, Input1, Input2, &Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

void TFluidProp::CapeOpenDeriv( string InputSpec, double Input1, double Input2, double* v, double* h,
                                double* s, double* G, double* lnphi, string* ErrorMsg)
{
   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Create safe arrays for double arrays v[2n+2], h[2n+2], s[2n+2], G[2n+2] and lnphi[n,2n+2], where n_max = 20.
   SAFEARRAYBOUND sa_bnd_vhsG[1];
   sa_bnd_vhsG[0].lLbound = 0;
   sa_bnd_vhsG[0].cElements = 100;
   SAFEARRAY *sa_v, *sa_h, *sa_s, *sa_G;
   sa_v = SafeArrayCreate(VT_R8, 1, sa_bnd_vhsG);
   sa_h = SafeArrayCreate(VT_R8, 1, sa_bnd_vhsG);
   sa_s = SafeArrayCreate(VT_R8, 1, sa_bnd_vhsG);
   sa_G = SafeArrayCreate(VT_R8, 1, sa_bnd_vhsG);

   SAFEARRAYBOUND sa_bnd_lnphi[2];
   sa_bnd_lnphi[0].lLbound = 0;
   sa_bnd_lnphi[0].cElements = 20;
   sa_bnd_lnphi[1].lLbound = 0;
   sa_bnd_lnphi[1].cElements = 100;
   SAFEARRAY *sa_lnphi;
   sa_lnphi = SafeArrayCreate(VT_R8, 2, sa_bnd_lnphi);

   FluidProp_COM->CapeOpenDeriv(BSTR_InputSpec, Input1, Input2, &sa_v, &sa_h, &sa_s, &sa_G, &sa_lnphi, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Put the values from safearray's in the double array's v, h, s, and G
   for (long i = 0; i < (signed)sa_bnd_vhsG[0].cElements; i++) {
      SafeArrayGetElement(sa_v, &i, &v[i]);
      SafeArrayGetElement(sa_h, &i, &h[i]);
      SafeArrayGetElement(sa_s, &i, &s[i]);
      SafeArrayGetElement(sa_G, &i, &G[i]);
   }

   // Put the values from safearray's sa_lnphi into double array lnphi
   for (long i = 0; i < (signed)sa_bnd_lnphi[1].cElements; i++) {
      for (long j = 0; j < (signed)sa_bnd_lnphi[0].cElements; i++) {
		 long idx_bnd[2];
         idx_bnd[1] = i;
         idx_bnd[0] = j;
         SafeArrayGetElement(sa_lnphi, &idx_bnd[0], &lnphi[i*20+j]);
      }
   }

   // Destroy the SafeArrays
   SafeArrayDestroy(sa_v);
   SafeArrayDestroy(sa_h);
   SafeArrayDestroy(sa_s);
   SafeArrayDestroy(sa_G);
   SafeArrayDestroy(sa_lnphi);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);
}


double* TFluidProp::SpecVolume_Deriv( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double* Output = new double[42];

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert the double array Conc into a SafeArray
   SAFEARRAYBOUND sa_bounds_Output[1];
   sa_bounds_Output[0].lLbound = 0;
   sa_bounds_Output[0].cElements = 42;
   SAFEARRAY* sa_Output;
   sa_Output = SafeArrayCreate(VT_R8, 1, sa_bounds_Output);

   FluidProp_COM->SpecVolume_Deriv( BSTR_InputSpec, Input1, Input2, &sa_Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with concentrations from SafeArray
   for (long i = 0; i < (signed)sa_bounds_Output[0].cElements; i++)
      SafeArrayGetElement(sa_Output, &i, &Output[i]);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double* TFluidProp::Enthalpy_Deriv( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double* Output = new double[42];

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert the double array Conc into a SafeArray
   SAFEARRAYBOUND sa_bounds_Output[1];
   sa_bounds_Output[0].lLbound = 0;
   sa_bounds_Output[0].cElements = 42;
   SAFEARRAY* sa_Output;
   sa_Output = SafeArrayCreate(VT_R8, 1, sa_bounds_Output);

   FluidProp_COM->Enthalpy_Deriv( BSTR_InputSpec, Input1, Input2, &sa_Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with concentrations from SafeArray
   for (long i = 0; i < (signed)sa_bounds_Output[0].cElements; i++)
      SafeArrayGetElement(sa_Output, &i, &Output[i]);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double* TFluidProp::Entropy_Deriv( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double* Output = new double[42];

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert the double array Conc into a SafeArray
   SAFEARRAYBOUND sa_bounds_Output[1];
   sa_bounds_Output[0].lLbound = 0;
   sa_bounds_Output[0].cElements = 42;
   SAFEARRAY* sa_Output;
   sa_Output = SafeArrayCreate(VT_R8, 1, sa_bounds_Output);

   FluidProp_COM->Entropy_Deriv( BSTR_InputSpec, Input1, Input2, &sa_Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with concentrations from SafeArray
   for (long i = 0; i < (signed)sa_bounds_Output[0].cElements; i++)
      SafeArrayGetElement(sa_Output, &i, &Output[i]);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double* TFluidProp::GibbsEnergy_Deriv( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double* Output = new double[42];

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert the double array Conc into a SafeArray
   SAFEARRAYBOUND sa_bounds_Output[1];
   sa_bounds_Output[0].lLbound = 0;
   sa_bounds_Output[0].cElements = 42;
   SAFEARRAY* sa_Output;
   sa_Output = SafeArrayCreate(VT_R8, 1, sa_bounds_Output);

   FluidProp_COM->GibbsEnergy_Deriv( BSTR_InputSpec, Input1, Input2, &sa_Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with concentrations from SafeArray
   for (long i = 0; i < (signed)sa_bounds_Output[0].cElements; i++)
      SafeArrayGetElement(sa_Output, &i, &Output[i]);

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}

double* TFluidProp::FugaCoef_Deriv( string InputSpec, double Input1, double Input2, string* ErrorMsg)
{
   double* Output = new double[20*42];

   BSTR BSTR_InputSpec = ConvertStringToBSTR(InputSpec);
   BSTR BSTR_Error;

   // Convert the double array Conc into a SafeArray
   SAFEARRAYBOUND sa_bounds_Output[2];
   sa_bounds_Output[0].lLbound = 0;
   sa_bounds_Output[0].cElements = 20;
   sa_bounds_Output[1].lLbound = 0;
   sa_bounds_Output[1].cElements = 42;
   SAFEARRAY* sa_Output;
   sa_Output = SafeArrayCreate(VT_R8, 2, sa_bounds_Output);

   FluidProp_COM->FugaCoef_Deriv( BSTR_InputSpec, Input1, Input2, &sa_Output, &BSTR_Error);
   SysFreeString(BSTR_InputSpec);

   // Retrieve array with concentrations from SafeArray
   for (long i = 0; i < (signed)sa_bounds_Output[0].cElements; i++) {
      for (long j = 0; j < (signed)sa_bounds_Output[1].cElements; j++) {
         long idx[2] = {i,j};
         double f;
         SafeArrayGetElement(sa_Output, idx, (void*)&f);
         // Storage in Output
         // 1) first  20 elements : derivatives wrt T  (dlnphi_1/dT, dlnphi_2/dT, ... dlnphi_20/dT)
         // 2) second 20 elements : derivatives wrt P  (dlnphi_1/dP, dlnphi_2/dP, ... dlnphi_20/dP)
         // 3) next 20*20 elements: derivatives wrt x  (dlnphi_1/dx_1, dlnphi_2/dx_1, ... dlnphi_20/dx_1, dlnphi_1/dx_2, etc.)
         // 4) next 20*20 elements: derivatives wrt n  (dlnphi_1/dn_1, dlnphi_2/dn_1, ... dlnphi_20/dn_1, dlnphi_1/dn_2, etc.)
         Output[i+j*20] = f;
      }
   }

   *ErrorMsg = ConvertBSTRToString(BSTR_Error);
   SysFreeString(BSTR_Error);

   return Output;
}
//==================================================================================== EOF ===//
