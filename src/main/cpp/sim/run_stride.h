#pragma once
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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
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
#include <vector>

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
 * Generate output files (at end of simulation).
 */
void generate_output_files(
    const string& output_prefix, const vector<unsigned int>& cases, const vector<unsigned int>& adopted,
    const boost::property_tree::ptree& pt_config,
    const shared_ptr<Simulator>
	sim,
    const unsigned int run_time, const unsigned int total_time);

} // end_of_namespace
