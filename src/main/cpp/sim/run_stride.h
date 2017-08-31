#ifndef RUN_STRIDE_H_INCLUDED
#define RUN_STRIDE_H_INCLUDED
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
 * Header for the Simulator class.
 */

#include "output/AdoptedFile.h"
#include "output/CasesFile.h"
#include "output/PersonFile.h"
#include "output/SummaryFile.h"
#include "sim/SimulatorBuilder.h"
#include "util/Stopwatch.h"
#include "util/TimeStamp.h"

#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <memory>
#include <string>

namespace stride {

using namespace output;
using namespace std;
using namespace std::chrono;
using namespace util;


/**
 * Run the simulator with config information provided.
 */
void run_stride(bool track_index_case, const std::string& config_file_name);

/**
 * Create the simulator
 */
template <class global_information_policy, class local_information_policy, class belief_policy, class behaviour_policy>
shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> > create_simulator(
		const boost::property_tree::ptree& pt_config, unsigned int num_threads, bool track_index_case = false) {

	cout << "Building the simulator. " << endl;

	auto sim = SimulatorBuilder<global_information_policy, local_information_policy, belief_policy, behaviour_policy>::Build(pt_config, num_threads, track_index_case);

	cout << "Done building the simulator. " << endl;

	return sim;
}

} // end_of_namespace

#endif // end-of-include-guard
