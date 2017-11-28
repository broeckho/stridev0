%module SimulatorBuilder

// headers for generated file
%{
#include "sim/SimulatorBuilder.h"
%}

// declare this
namespace stride{
  namespace output{}
  namespace util{}
}

// So we can pass python strings to arguments that require std::string.
%include std_string.i
//%include <std_shared_ptr.i>

// We want to generate code for these modules classes.
%include "boost.i"

//%shared_ptr(stride::Simulator)

%include "sim/SimulatorBuilder.h"

%template(NoGlobNoLocNoBeliefNoBehaviourBuilder) stride::SimulatorBuilder<NoGlobalInformation, NoLocalInformation, NoBelief, NoBehaviour<NoBelief> >;
