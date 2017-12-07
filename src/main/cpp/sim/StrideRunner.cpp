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
 * Implementation for StrideRunner.
 */

#include "StrideRunner.h"

#include "output/AdoptedFile.h"
#include "output/CasesFile.h"
#include "output/PersonFile.h"
#include "output/SummaryFile.h"
#include "sim/SimulatorBuilder.h"
#include "util/ConfigInfo.h"
#include "util/InstallDirs.h"
#include "util/Stopwatch.h"
#include "util/StringUtils.h"
#include "util/TimeStamp.h"
#include "util/TimeToString.h"

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <omp.h>
#include <spdlog/spdlog.h>

namespace stride {

using namespace output;
using namespace util;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace std;
using namespace std::chrono;


/// Run the simulator with config information provided.
void StrideRunner::Run(bool track_index_case, const std::string&  config_file_name)
{
	// -----------------------------------------------------------------------------------------
	// Print output to command line.
	// -----------------------------------------------------------------------------------------
	cout << "\n*************************************************************" << endl;
	cout << "Starting up at:      " << TimeStamp().ToString() << endl;
	cout << "Executing:           " << InstallDirs::GetExecPath().string() << endl;
	cout << "Current directory:   " << InstallDirs::GetCurrentDir().string() << endl;
	cout << "Install directory:   " << InstallDirs::GetRootDir().string() << endl;
	cout << "Data    directory:   " << InstallDirs::GetDataDir().string() << endl;

	// -----------------------------------------------------------------------------------------
	// Check execution environment.
	// -----------------------------------------------------------------------------------------
	if (InstallDirs::GetCurrentDir().compare(InstallDirs::GetRootDir()) != 0) {
		throw runtime_error(string(__func__) + "> Current directory is not install root! Aborting.");
	}
	if (InstallDirs::GetDataDir().empty()) {
		throw runtime_error(string(__func__) + "> Data directory not present! Aborting.");
	}

	// -----------------------------------------------------------------------------------------
	// Configuration.
	// -----------------------------------------------------------------------------------------
	ptree pt_config;
	const auto file_path = canonical(system_complete(config_file_name));

	if (!is_regular_file(file_path)) {
		throw runtime_error(string(__func__) + ">Config file " + file_path.string() +
				" not present. Aborting.");
	}
	read_xml(file_path.string(), pt_config);
	cout << "Configuration file:  " << file_path.string() << endl;

	// -----------------------------------------------------------------------------------------
	// OpenMP.
	// -----------------------------------------------------------------------------------------
	unsigned int num_threads;

	#pragma omp parallel
	{
		num_threads = omp_get_num_threads();
	}
	if (ConfigInfo::HaveOpenMP()) {
		cout << "Using OpenMP threads:  " << num_threads << endl;
	} else {
		cout << "Not using OpenMP threads." << endl;
	}

	// -----------------------------------------------------------------------------------------
	// Set output path prefix.
	// -----------------------------------------------------------------------------------------
	auto output_prefix = pt_config.get<string>("run.output_prefix", "");
	if (output_prefix.length() == 0) {
		output_prefix = TimeStamp().ToTag();
	}
	cout << "Project output tag:  " << output_prefix << endl << endl;

	// -----------------------------------------------------------------------------------------
	// Additional run configurations.
	// -----------------------------------------------------------------------------------------
	if (pt_config.get_optional<bool>("run.num_participants_survey") == false) {
		pt_config.put("run.num_participants_survey", 1);
	}

	// -----------------------------------------------------------------------------------------
	// Track index case setting.
	// -----------------------------------------------------------------------------------------
	cout << "Setting for track_index_case:  " << boolalpha << track_index_case << endl;

	// -----------------------------------------------------------------------------------------
	// Create logger
	// Transmissions:     [TRANSMISSION] <infecterID> <infectedID> <clusterID> <day>
	// General contacts:  [CNT] <person1ID> <person1AGE> <person2AGE>  <at_home> <at_work> <at_school> <at_other>
	// -----------------------------------------------------------------------------------------
	spdlog::set_async_mode(1048576);
	auto file_logger = spdlog::rotating_logger_mt("contact_logger", output_prefix + "_logfile",
			std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max());
	file_logger->set_pattern("%v"); // Remove meta data from log => time-stamp of logging

	// ------------------------------------------------------------------------------
	// Create the simulator.
	//------------------------------------------------------------------------------
	Stopwatch<> total_clock("total_clock", true);

	cout << "Building the simulator. " << endl;
	auto sim = SimulatorBuilder::Build(pt_config, num_threads, track_index_case);
	cout << "Done building the simulator. " << endl;

	// -----------------------------------------------------------------------------------------
	// Check the simulator.
	// -----------------------------------------------------------------------------------------
	bool simulator_is_operational = sim->IsOperational();
	if (simulator_is_operational) {
		cout << "Done checking the simulator. " << endl << endl;
	} else {
		file_logger->info("[ERROR] Invalid configuration");
		cout << "Invalid configuration => terminate without output" << endl << endl;
	}

	// -----------------------------------------------------------------------------------------
	// Run the simulation (if operational).
	// -----------------------------------------------------------------------------------------
	Stopwatch<> run_clock("run_clock");
	if (simulator_is_operational) {
		const unsigned int num_days = pt_config.get<unsigned int>("run.num_days");
		vector<unsigned int> cases(num_days);
		vector<unsigned int> adopted(num_days);
		for (unsigned int i = 0; i < num_days; i++) {
			cout << "Simulating day: " << setw(5) << i;

			run_clock.Start();
			sim->TimeStep();
			run_clock.Stop();

			cout << "     Done, infected count: ";

			cases[i] = sim->GetPopulation()->GetInfectedCount();
			adopted[i] = sim->GetPopulation()->GetAdoptedCount();

			cout << setw(7) << cases[i] << "     Adopters count: " << setw(7) << adopted[i];
			cout << endl;
		}

	// -----------------------------------------------------------------------------------------
	// Generate output files
	// -----------------------------------------------------------------------------------------
	GenerateOutputFiles(output_prefix, cases, adopted, pt_config, sim,
			duration_cast<milliseconds>(run_clock.Get()).count(),
			duration_cast<milliseconds>(total_clock.Get()).count());
	}

	// -----------------------------------------------------------------------------------------
	// Print final message to command line.
	// -----------------------------------------------------------------------------------------
	cout << endl << endl;
	cout << "  run_time: " << run_clock.ToString() << "  -- total time: "
			<< total_clock.ToString() << endl << endl;
	cout << "Exiting at:         " << TimeStamp().ToString() << endl << endl;
}

/// Generate output files (at the end of the simulation).
void StrideRunner::GenerateOutputFiles(const std::string& output_prefix, const std::vector<unsigned int>& cases,
		const std::vector<unsigned int>& adopted, const boost::property_tree::ptree& pt_config,
		std::shared_ptr<Simulator> sim, const unsigned int run_time, const unsigned int total_time)
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