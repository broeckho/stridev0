#ifndef SIM_UTILS_H_INCLUDED
#define SIM_UTILS_H_INCLUDED
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

/*
 * @file Header file for SimUtils class.
 */

#include "behaviour/behaviour_policies/NoBehaviour.h"
#include "behaviour/belief_policies/NoBelief.h"
#include "behaviour/information_policies/NoGlobalInformation.h"
#include "behaviour/information_policies/NoLocalInformation.h"
#include "output/AdoptedFile.h"
#include "output/CasesFile.h"
#include "output/PersonFile.h"
#include "output/SummaryFile.h"
#include "sim/SimulatorBuilder.h"
#include "util/ConfigInfo.h"
#include "util/Stopwatch.h"
#include "util/TimeStamp.h"

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iostream>
#include <memory>
#include <omp.h>
#include <spdlog/spdlog.h>
#include <vector>

namespace stride {

using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace output;
using namespace std;
using namespace util;

class SimUtils {
public:
	///
	SimUtils() {}

	///
	virtual ~SimUtils() {}

	/// Setup the simulator configuration, execution environment, ...
	void Setup(std::string config_file_name, bool track_index_case) {
		// -----------------------------------------------------------------------------------------
		// Print output to command line.
		// -----------------------------------------------------------------------------------------
		cout << "\n*************************************************************" << endl;
		cout << "Starting up at:      " << TimeStamp().ToString() << endl;

		/*
		// -----------------------------------------------------------------------------------------
		// Check execution environment.
		// -----------------------------------------------------------------------------------------
		if (InstallDirs::GetCurrentDir().compare(InstallDirs::GetRootDir()) != 0) {
			throw runtime_error(string(__func__) + "> Current directory is not install root! Aborting.");
		}
		if (InstallDirs::GetDataDir().empty()) {
			throw runtime_error(string(__func__) + "> Data directory not present! Aborting.");
		}
		*/

		// -----------------------------------------------------------------------------------------
		// Configuration.
		// -----------------------------------------------------------------------------------------
		const auto file_path = canonical(system_complete(config_file_name));

		if (!is_regular_file(file_path)) {
			throw runtime_error(string(__func__) + ">Config file " + file_path.string() +
					    " not present. Aborting.");
		}

		read_xml(file_path.string(), m_pt_config);
		cout << "Configuration file:  " << file_path.string() << endl;

		// -----------------------------------------------------------------------------------------
		// OpenMP.
		// -----------------------------------------------------------------------------------------

#pragma omp parallel
		{
			m_num_threads = omp_get_num_threads();
		}

		if (ConfigInfo::HaveOpenMP()) {
			cout << "Using OpenMP threads:  " << m_num_threads << endl;
		} else {
			cout << "Not using OpenMP threads." << endl;
		}

		// -----------------------------------------------------------------------------------------
		// Set output path prefix.
		// -----------------------------------------------------------------------------------------
		m_output_prefix = m_pt_config.get<string>("run.output_prefix", "");
		if (m_output_prefix.length() == 0) {
			output_prefix = TimeStamp().ToTag();
		}
		cout << "Project output tag:  " << m_output_prefix << endl << endl;

		// -----------------------------------------------------------------------------------------
		// Additional run configurations.
		// -----------------------------------------------------------------------------------------
		if (m_pt_config.get_optional<bool>("run.num_participants_survey") == false) {
			m_pt_config.put("run.num_participants_survey", 1);
		}

		// -----------------------------------------------------------------------------------------
		// Track index case setting.
		// -----------------------------------------------------------------------------------------
		m_track_index_case = track_index_case;
		cout << "Setting for track_index_case:  " << boolalpha << track_index_case << endl;

		// -----------------------------------------------------------------------------------------
		// Create logger
		// Transmissions:     [TRANSMISSION] <infecterID> <infectedID> <clusterID> <day>
		// General contacts:  [CNT] <person1ID> <person1AGE> <person2AGE>  <at_home> <at_work> <at_school> <at_other>
		// -----------------------------------------------------------------------------------------
		spdlog::set_async_mode(1048576);
		auto file_logger =
		    spdlog::rotating_logger_mt("contact_logger", m_output_prefix + "/logfile", std::numeric_limits<size_t>::max(),
					       std::numeric_limits<size_t>::max());
		file_logger->set_pattern("%v"); // Remove meta data from log => time-stamp of logging
	}

	/// Create the simulator
	template <class global_information_policy, class local_information_policy, class belief_policy, class behaviour_policy>
	shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> > CreateSimulator()
	{
		cout << "Building the simulator. " << endl;
		auto sim = SimulatorBuilder<global_information_policy, local_information_policy, belief_policy, behaviour_policy>
			::Build(m_pt_config, m_num_threads, m_track_index_case);
		cout << "Done building the simulator. " << endl;

		return sim;
	}

	/// Build & run the simulator
	void Run() {
		// Get logger
		const shared_ptr<spdlog::logger> logger = spdlog::get("contact_logger");

		// -----------------------------------------------------------------------------------------
		// Get information, belief and behaviour policies to be used
		// -----------------------------------------------------------------------------------------
		string belief_policy = m_pt_config.get<string>("run.belief_policy", "NoBelief");
		string behaviour_policy = m_pt_config.get<string>("run.behaviour_policy", "NoBehaviour");
		string local_information_policy = m_pt_config.get<string>("run.local_information_policy", "NoLocalInformation");
		string global_information_policy = m_pt_config.get<string>("run.global_information_policy", "NoGlobalInformation");

		// Get number of days to simulate
		const unsigned int num_days = m_pt_config.get<unsigned int>("run.num_days");

		m_total_clock = Stopwatch<>("total_clock", true);

		if (global_information_policy == "NoGlobalInformation") {
			using GlobalInformationPolicy = NoGlobalInformation;

			if (local_information_policy == "NoLocalInformation") {
				using LocalInformationPolicy = NoLocalInformation;

				if (belief_policy == "NoBelief") {
					using BeliefPolicy = NoBelief;

					if (behaviour_policy == "NoBehaviour") {
						using BehaviourPolicy = NoBehaviour<BeliefPolicy>;

						// Create the simulator
						auto sim = CreateSimulator<GlobalInformationPolicy, LocalInformationPolicy, BeliefPolicy, BehaviourPolicy>();

						// -----------------------------------------------------------------------------------------
						// Check the simulator.
						// -----------------------------------------------------------------------------------------
						bool simulator_is_operational = sim->IsOperational();
						if (simulator_is_operational) {
							cout << "Done checking the simulator. " << endl << endl;
						} else {
							logger->info("[ERROR] Invalid configuration");
							cout << "Invalid configuration => terminate without output" << endl << endl;
						}

						// -----------------------------------------------------------------------------------------
						// Run the simulation (if operational).
						// -----------------------------------------------------------------------------------------
						if (simulator_is_operational) {
							Run<GlobalInformationPolicy, LocalInformationPolicy, BeliefPolicy, BehaviourPolicy>(sim, num_days);
						}
					} else {
						throw std::runtime_error(std::string(__func__) + "No valid behaviour policy!");
					}
				} else {
					throw std::runtime_error(std::string(__func__) + "No valid belief policy!");
				}
			} else {
				throw std::runtime_error(std::string(__func__) + "No valid local information policy!");
			}
		} else {
			throw std::runtime_error(std::string(__func__) + "No valid global information policy!");
		}
	}

	template <class global_information_policy, class local_information_policy, class belief_policy, class behaviour_policy>
	void Run(shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> > sim, unsigned int num_days) {
		m_run_clock = Stopwatch<>("run_clock");
		vector<unsigned int> cases(num_days);
		vector<unsigned int> adopted(num_days);

		for (unsigned int i = 0; i < num_days; i++) {
			cout << "Simulating day: " << setw(5) << i;

			m_run_clock.Start();
			sim->TimeStep();
			m_run_clock.Stop();

			cout << "     Done, infected count: ";
			cases[i] = sim->GetPopulation()->GetInfectedCount();
			//adopted[i] = sim->GetPopulation()->GetAdoptedCount<belief_policy>();
			cout << setw(7) << cases[i];
			//cout << "     Adopters count: " << setw(7) << adopted[i];
			cout << endl;
		}

		// -----------------------------------------------------------------------------------------
		// Generate output files
		// -----------------------------------------------------------------------------------------
		generate_output_files<global_information_policy, local_information_policy, belief_policy, behaviour_policy>(cases, adopted, sim);

		// -----------------------------------------------------------------------------------------
		// Print final message to command line.
		// -----------------------------------------------------------------------------------------
		cout << endl << endl;

		cout << "  run_time: " << m_run_clock.ToString() << "  -- total time: " << m_total_clock.ToString() << endl << endl;
		cout << "Exiting at:         " << TimeStamp().ToString() << endl << endl;
	}

	/// Stop

	/// Callbacks & observers?

	/// Generate output files (at end of simulation).
	template <class global_information_policy, class local_information_policy, class belief_policy, class behaviour_policy>
	void generate_output_files(const vector<unsigned int>& cases, const vector<unsigned int>& adopted,
			const shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> > sim)
	{
		unsigned int run_time = chrono::duration_cast<chrono::milliseconds>(m_run_clock.Get()).count();
		unsigned int total_time = chrono::duration_cast<chrono::milliseconds>(m_total_clock.Get()).count();
		// Cases
		CasesFile cases_file(m_output_prefix);
		cases_file.Print(cases);

		// Adopted
		AdoptedFile adopted_file(m_output_prefix);
		adopted_file.Print(adopted);

		// Summary
		SummaryFile summary_file(m_output_prefix);
		summary_file.Print(m_pt_config, sim->GetPopulation()->size(), sim->GetPopulation()->GetInfectedCount(),
				sim->GetDiseaseProfile().GetTransmissionRate(), run_time, total_time);

		// Persons
		if (m_pt_config.get<double>("run.generate_person_file") == 1) {
			PersonFile person_file(m_output_prefix);
			person_file.Print(sim->GetPopulation());
		}
	}

private:
	Stopwatch<> m_total_clock;
	Stopwatch<> m_run_clock;

	ptree m_pt_config;
	std::string m_output_prefix;
	unsigned int m_num_threads;
	bool m_track_index_case;

};

} /* end of namespace stride */

#endif // end of include-guard

/*
#include <string>

#include "util/InstallDirs.h"

#include <boost/property_tree/xml_parser.hpp>
 */
