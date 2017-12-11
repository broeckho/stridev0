%module Population

// headers for generated file
%{
#include "pop/Person.h"
#include <vector>
#include <memory>
%}

%include <std_vector.i>
%include <std_shared_ptr.i>

%include "pop/Person.h"

%shared_ptr(std::vector<stride::Person>);
%shared_ptr(stride::Population);

%template(PopulationBaseClass) std::vector<stride::Person>;

%include "pop/Population.h"
