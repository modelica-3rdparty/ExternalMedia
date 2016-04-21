#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
//#define _SECURE_SCL 0
#include <stdlib.h>
#include <crtdbg.h>

#include "CoolProp.h"
#include "math.h"
#include "time.h"
#include "stdlib.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <conio.h>
#include "Solvers.h"
#include "AbstractState.h"
#include "externalmedialib.h"
#include "crossplatform_shared_ptr.h"
//#include "include.h"
//#include "coolpropsolver.h"
//#include "basesolver.h"
//#include "ModelicaUtilities.h"
//#include "errorhandling.h"
//#include "documentation.h"
//#include "solvermap.h"

void printFluidConstants(const char *mediumName, const char *libraryName, const char *substanceName)
{
	std::cout << "\nMediumName = " << mediumName << ", libraryName = " << libraryName << ", substanceName = " << substanceName << std::endl;
	std::cout << "MolarMass = " << TwoPhaseMedium_getMolarMass_C_impl(mediumName, libraryName, substanceName) << std::endl;
	std::cout << "CriticalTemperature = " << TwoPhaseMedium_getCriticalTemperature_C_impl(mediumName, libraryName, substanceName) << std::endl;
	std::cout << "CriticalPressure = " << TwoPhaseMedium_getCriticalPressure_C_impl(mediumName, libraryName, substanceName) << std::endl;
	std::cout << "CriticalMolarVolume = " << TwoPhaseMedium_getCriticalMolarVolume_C_impl(mediumName, libraryName, substanceName) << std::endl;
}

void printValues(ExternalThermodynamicState *state)
{
	std::cout << "temperature = " << state->T << std::endl;
	std::cout << "velocityOfSound = " << state->a << std::endl;
	std::cout << "isobaric expansion coefficient = " << state->beta << std::endl;
	std::cout << "specificHeatCapacityCp = " << state->cp << std::endl;
	std::cout << "specificHeatCapacityCv = " << state->cv << std::endl;
	std::cout << "density = " << state->d << std::endl;
	std::cout << "density_derh_p = " << state->ddhp << std::endl;
	std::cout << "density_derp_h = " << state->ddph << std::endl;
	std::cout << "dynamicViscosity = " << state->eta << std::endl;
	std::cout << "specificEnthalpy = " << state->h << std::endl;
	std::cout << "isothermalCompressibility = " << state->kappa << std::endl;
	std::cout << "thermalConductivity = " << state->lambda << std::endl;
	std::cout << "pressure = " << state->p << std::endl;
	std::cout << "specificEntropy = " << state->s << std::endl;
	std::cout << "phase = " << state->phase << std::endl;
}

struct structtype{
	double Pr;		//prandtl number
    double T;		//temperature
    double a;		//velocity of sound
    double beta;	//isobaric expansion coefficient
    double cp;		//specific heat capacity cp
    double cv;		//specific heat capacity cv
    double d;		//density
    double ddhp;	//derivative of density wrt enthalpy at constant pressure
    double ddph;	// derivative of density wrt pressure at constant enthalpy
    double eta;		//dynamic viscosity
    double h;		//specific enthalpy
    double kappa;	//compressibility
    double lambda;	//thermal conductivity
    double p;		//pressure
    double s;		//specific entropy
	int phase;		//phase
};

structtype mystruct;

// The CoolProp2Modelica library can only compile as a Dynamic Link Library if BUILD_DLL is set to 1 in include.h
// Do not forget to set it back to zero when compiling the final library!
ExternalThermodynamicState state;	// Defined in the externalmedia headers

int main()
{	
	// Dirrect CoolProp usage:
	std::cout << "1.1) CoolProp AbstractState water" << std::endl;
	shared_ptr<CoolProp::AbstractState> Water(CoolProp::AbstractState::factory("HEOS","Water"));
    Water->update(CoolProp::PQ_INPUTS, 101325, 0); // SI units
	//std::cout << "sat Cp: " << Water->saturated_liquid_keyed_output(CoolProp::iCpmass) << std::endl;

    std::cout << "T: " << Water->T() << " K" << std::endl;
    std::cout << "rho': " << Water->rhomass() << " kg/m^3" << std::endl;
    std::cout << "rho': " << Water->rhomolar() << " mol/m^3" << std::endl;
    std::cout << "h': " << Water->hmass() << " J/kg" << std::endl;
    std::cout << "h': " << Water->hmolar() << " J/mol" << std::endl;
    std::cout << "s': " << Water->smass() << " J/kg/K" << std::endl;
    std::cout << "s': " << Water->smolar() << " J/mol/K\n" << std::endl;

	std::cout << "1.2) CoolProp AbstractState R407c" << std::endl;
	shared_ptr<CoolProp::AbstractState> R407c(CoolProp::AbstractState::factory("HEOS", "R407c"));
	R407c->update(CoolProp::PQ_INPUTS, 700000, 0.5);
	double dl = R407c->saturated_liquid_keyed_output(CoolProp::iDmass);
	double dv = R407c->saturated_vapor_keyed_output(CoolProp::iDmass);
	std::cout << "Liquid: " << dl << "kg/m^3" << std::endl;
	std::cout << "Vapour: " << dv << "kg/m^3\n" << std::endl;
	
	// Test of PropsSI
	std::cout << "2.1) CoolProp PropsSI Helium" << std::endl;
	std::cout << "T: " << CoolProp::PropsSI("T","H",7430.890839,"S",1324.077157,"Helium") << std::endl;
	std::cout << "P: " << CoolProp::PropsSI("P","H",7430.890839,"S",1324.077157,"Helium") << std::endl;

	std::cout << "\n2.2) Error caching with PropsSI" << std::endl;
	
	double prop = CoolProp::PropsSI("D","H",1E5,"P",1e5,"T66");
	std::string err = CoolProp::get_global_param_string("errstring");
	std::cout << err << std::endl;

	// Fluid constants by ExternalMedia
	// Uses the function `printFluidConstants` defined at the beginning of this file
	std::cout << "3.1) ExternalMedia Fluid Constants" << std::endl;
	printFluidConstants("Water", "CoolProp", "Water");
	printFluidConstants("Water", "CoolProp", "Water|debug=10");
	printFluidConstants("R245fa", "CoolProp", "R245fa");
	printFluidConstants("R407c", "CoolProp", "R407c");
	printFluidConstants("Helium", "CoolProp", "He");

	// Test ExternalMedia:
	struct structtype *struct_pointer;
	struct_pointer = &mystruct;
	struct_pointer->eta = 3;
	mystruct.p=2;

	ExternalThermodynamicState *pointer;	// no need to add struct before as above because it was defined with typedef
	pointer = &state;
	
	std::cout << "\n3.2) ExternalMedia water" << std::endl;
	TwoPhaseMedium_setState_ph_C_impl(8e5, 4E5, 2, pointer, "water", "CoolProp", "water");
	printValues(pointer);

	std::cout << "\n3.3) ExternalMedia R245fa calc_transport=0|debug=10" << std::endl;

	TwoPhaseMedium_setState_ph_C_impl(8e5, 4E5, 2, pointer, "R245fa", "CoolProp", "R245fa|calc_transport=0|debug=10");
	printValues(pointer);

	std::cout << "\n3.4) ExternalMedia R245fa" << std::endl;

	TwoPhaseMedium_setState_ph_C_impl(8e5, 4E5, 2, pointer, "R245fa", "CoolProp", "R245fa");
	printValues(pointer);

	std::cout << "\n3.5) ExternalMedia R407c rho_smoothing_xend=0.2" << std::endl;
	TwoPhaseMedium_setState_ph_C_impl(8e5, 4E5, 2, pointer, "R407c", "CoolProp", "R407c|rho_smoothing_xend=0.2");
	printValues(pointer);

	std::cout << "\n3.6) ExternalMedia R245fa calc_transport=0|enable_TTSE=1|debug=10" << std::endl;
	TwoPhaseMedium_setState_ph_C_impl(8e5, 4E5, 1, pointer, "R245fa", "CoolProp", "R245fa|calc_transport=0|enable_TTSE=1|debug=10");
	printValues(pointer);

	std::cout << "\n3.7) ExternalMedia R245fa calc_transport=0|enable_BICUBIC=1|debug=10" << std::endl;
	TwoPhaseMedium_setState_ph_C_impl(8e5, 4E5, 1, pointer, "R245fa", "CoolProp", "R245fa|calc_transport=0|enable_BICUBIC=1|debug=10");
	printValues(pointer);

	std::cout << "\n3.8) ExternalMedia R245fa calc_transport=0|enable_TTSE=1|debug=10|rho_smoothing_xend=0.2" << std::endl;
	TwoPhaseMedium_setState_ph_C_impl(8e5, 4E5, 1, pointer, "R245fa", "CoolProp", "R245fa|calc_transport=0|enable_TTSE=1|debug=100|rho_smoothing_xend=0.2");
	printValues(pointer);

	std::cout << "\n3.9)  ExternalMedia R245fa pT input" << std::endl;

	TwoPhaseMedium_setState_pT_C_impl(8e5, 350, pointer, "R245fa", "CoolProp", "R245fa");
	printValues(pointer);
	
	/*
	std::cout << "\n3.10)  ExternalMedia Helium pT input" << std::endl; //|debug=100
	TwoPhaseMedium_setState_pT_C_impl(4e5, 4, pointer, "Helium", "CoolProp", "Helium|debug=100");
	std::string err0 = CoolProp::get_global_param_string("errstring");
	std::cout << err0 << std::endl;
	printValues(pointer);

	std::cout << "\n3.11)  ExternalMedia Helium ph input" << std::endl;
	TwoPhaseMedium_setState_ph_C_impl(4e5, 218.14620463403665, 2, pointer, "Helium", "CoolProp", "Helium");
	printValues(pointer);
	
	std::cout << "3.12) CoolProp AbstractState Helium" << std::endl;
	shared_ptr<CoolProp::AbstractState> helium(CoolProp::AbstractState::factory("HEOS","Helium"));
    helium->update(CoolProp::PT_INPUTS, 4.e5, 4.); // SI units
	std::cout << "T: " << helium->T() << std::endl;
	std::cout << "P: " << helium->p() << std::endl;
	std::cout << "rho: " << helium->rhomass() << std::endl;
	std::cout << "h: " << helium->hmass() << std::endl;
	/*
	std::cout << "\n3.13)  ExternalMedia Water pT input" << std::endl; //|debug=100
	TwoPhaseMedium_setState_pT_C_impl(4e5, 4, pointer, "Water", "CoolProp", "Water|debug=100");
	std::string err2 = CoolProp::get_global_param_string("errstring");
	std::cout << err2 << std::endl;
	printValues(pointer);

	std::cout << "\n3.14)  ExternalMedia Water pT input 2" << std::endl;
	TwoPhaseMedium_setState_pT_C_impl(4e5, 400, pointer, "Water", "CoolProp", "Water|debug=100");
	printValues(pointer);
	
	std::cout << "3.15) CoolProp AbstractState Water" << std::endl;
	shared_ptr<CoolProp::AbstractState> Water(CoolProp::AbstractState::factory("HEOS","Water"));
    Water->update(CoolProp::PT_INPUTS, 4e5, 4); // SI units
	std::cout << "T: " << Water->T() << std::endl;
	std::cout << "P: " << Water->p() << std::endl;
	std::cout << "rho: " << Water->rhomass() << std::endl;
	std::cout << "h: " << Water->hmass() << std::endl;

	
	std::cout << "3.16) CoolProp PropsSI Helium" << std::endl;
	std::cout << "h: " << CoolProp::PropsSI("H","P",4e5,"T",4,"Helium") << std::endl;
	
	std::cout << "3.17) CoolProp AbstractState Helium" << std::endl;
	shared_ptr<CoolProp::AbstractState> helium(CoolProp::AbstractState::factory("HEOS","Helium"));
    helium->update(CoolProp::HmassSmass_INPUTS, 7430.890839, 1324.077157);
	std::cout << "T: " << helium->T() << std::endl;
	std::cout << "P: " << helium->p() << std::endl;
	
	std::cout << "\n3.18)  ExternalMedia Helium hs input" << std::endl;
	TwoPhaseMedium_setState_hs_C_impl(7430.890839, 1324.077157, 2, pointer, "Helium", "CoolProp", "Helium|debug=100");
	std::string err1 = CoolProp::get_global_param_string("errstring");
	std::cout << err1 << std::endl;
	printValues(pointer);
	*/
	

	system("pause");
	return 0;
}
