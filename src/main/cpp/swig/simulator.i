%module Simulator

// headers for generated file
%{
#include "sim/Simulator.h"
#include "behaviour/information_policies/NoGlobalInformation.h"

using namespace stride;
%}

// So we can pass python strings to arguments that require std::string.
//%include std_string.i
//%include <std_shared_ptr.i>

// We want to generate code for these modules classes.
//%include "boost.i"

//%shared_ptr(stride::Simulator)

%include "sim/Simulator.h"

%template(NoGlobNoLocNoBeliefNoBehaviour) stride::Simulator<NoGlobalInformation, NoLocalInformation, NoBelief, NoBehaviour<NoBelief> >;
