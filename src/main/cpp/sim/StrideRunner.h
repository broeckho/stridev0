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
 * Header for the StrideRunner class.
 */

#include "sim/Simulator.h"

#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <string>
#include <vector>

namespace stride {

/**
 * Class for setting up & managing a simulation.
 */
class StrideRunner
{
public:
	/// Constructor
	StrideRunner() {}

	/// Destructor
	virtual ~StrideRunner() {}

	/// Run the simulator with config information provided.
	static void Run(bool track_index_case, const std::string&  config_file_name);

private:
	/// Generate output files (at the end of the simulation).
	static void GenerateOutputFiles(const std::string& output_prefix, const std::vector<unsigned int>& cases,
			const std::vector<unsigned int>& adopted, const boost::property_tree::ptree& pt_config,
			std::shared_ptr<Simulator> sim, const unsigned int run_time, const unsigned int total_time);
};

} // end_of_namespace
