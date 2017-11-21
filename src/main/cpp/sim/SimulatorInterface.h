#ifndef SIMULATOR_INTERFACE_H_INCLUDED
#define SIMULATOR_INTERFACE_H_INCLUDED
/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, Willem L, Kuylen E, Stijven S & Broeckhove J
 */

/**
 * @file
 * Clean interface for Simulator class
 * (for swig interaction)
 */

#include <boost/property_tree/ptree.hpp>

namespace stride {

class SimulatorInterface {
public:
	SimulatorInterface();

	virtual ~SimulatorInterface();

};

}

#endif


/*
#include "run_stride.h"

#include <exception>
#include <iostream>
#include <tclap/CmdLine.h>

using namespace std;
using namespace stride;
using namespace TCLAP;

/// Main program of the stride simulator.
int main(int argc, char** argv)
{
	int exit_status = EXIT_SUCCESS;
	try {
		// -----------------------------------------------------------------------------------------
		// Parse command line.
		// -----------------------------------------------------------------------------------------
		CmdLine cmd("stride", ' ', "1.0", false);
		SwitchArg index_case_Arg("r", "r0", "R0 only", cmd, false);
		ValueArg<string> config_file_Arg("c", "config", "Config File", false, "./config/run_default.xml",
						 "CONFIGURATION FILE", cmd);
		cmd.parse(argc, argv);

		// -----------------------------------------------------------------------------------------
		// Run the Stride simulator.
		// -----------------------------------------------------------------------------------------
		run_stride(index_case_Arg.getValue(), config_file_Arg.getValue());

	} catch (exception& e) {
		exit_status = EXIT_FAILURE;
		cerr << "\nEXCEPION THROWN: " << e.what() << endl;
	} catch (...) {
		exit_status = EXIT_FAILURE;
		cerr << "\nEXCEPION THROWN: "
		     << "Unknown exception." << endl;
	}
	return exit_status;
}
 *
 */

/**
 * @file
 * Header for the Simulator class.


#include "output/AdoptedFile.h"
#include "output/CasesFile.h"
#include "output/PersonFile.h"
#include "output/SummaryFile.h"
#include "sim/SimulatorBuilder.h"
#include "util/Stopwatch.h"
#include "util/TimeStamp.h"


#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace stride {

using namespace output;
using namespace std;
using namespace std::chrono;
using namespace util;


/**
 * Run the simulator with config information provided.

void run_stride(bool track_index_case, const std::string& config_file_name);

/**
 * Create the simulator.
 *
template <class global_information_policy, class local_information_policy, class belief_policy, class behaviour_policy>
shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> > create_simulator(
		const boost::property_tree::ptree& pt_config, unsigned int num_threads, bool track_index_case = false)
{

	cout << "Building the simulator. " << endl;

	auto sim = SimulatorBuilder<global_information_policy, local_information_policy, belief_policy, behaviour_policy>::Build(pt_config, num_threads, track_index_case);

	cout << "Done building the simulator. " << endl;

	return sim;
}


/**
 * Generate output files (at end of simulation).
 *
template <class global_information_policy, class local_information_policy, class belief_policy, class behaviour_policy>
void generate_output_files(const string output_prefix, const vector<unsigned int>& cases, const vector<unsigned int>& adopted,
		const boost::property_tree::ptree& pt_config,
		const shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> > sim,
		const unsigned int run_time, const unsigned int total_time)
{

	// Cases
	CasesFile cases_file(output_prefix);
	cases_file.Print(cases);

	// Adopted
	AdoptedFile adopted_file(output_prefix);
	adopted_file.Print(adopted);

	// Summary
	SummaryFile summary_file(output_prefix);
	summary_file.Print(pt_config, sim->GetPopulation()->size(), sim->GetPopulation()->GetInfectedCount(),
			sim->GetDiseaseProfile().GetTransmissionRate(), run_time, total_time);

	// Persons
	if (pt_config.get<double>("run.generate_person_file") == 1) {
		PersonFile person_file(output_prefix);
		person_file.Print(sim->GetPopulation());
	}
}

} // end_of_namespace

  */
