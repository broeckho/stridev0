//%module(directors="1") Simulator
%module SimulatorInterface

%rename("Simulator") SimulatorInterface;

// headers for generated file
%{
#include "sim/SimulatorInterface.h"
%}

// So we can pass python strings to arguments that require std::string.
//%include std_string.i
//%include <std_shared_ptr.i>

// We want to generate code for these modules classes.
%include "boost.i"

//%shared_ptr(stride::Simulator)
//%shared_ptr(stride::SimulatorObserver)
//%feature("director") stride::SimulatorObserver;
//%include "sim/Simulator.h";

%include "sim/SimulatorInterface.h"

//%extend stride::Simulator {
//	void registerObserver(std::shared_ptr<stride::SimulatorObserver> obs){
//		 $self->registerObserver(obs);
//	}
//}
