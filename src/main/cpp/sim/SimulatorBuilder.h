#ifndef SIMULATOR_BUILDER_H_INCLUDED
#define SIMULATOR_BUILDER_H_INCLUDED
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
 * Header for the SimulatorBuilder class.
 */

/*
#include "core/Cluster.h"
#include "core/ClusterType.h"
#include "core/ContactProfile.h"
#include "core/Infector.h"
#include "core/LogMode.h"
#include "pop/Population.h"

#include <iostream>
#include <omp.h>
 */

#include "calendar/Calendar.h"
#include "core/RngHandler.h"
#include "immunity/Vaccinator.h"

#include "pop/PopulationBuilder.h"
#include "sim/Simulator.h"
#include "util/InstallDirs.h"
#include "util/Random.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <stdexcept>

namespace stride {

using namespace::std;
// using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace stride::util;

/**
 * Class to build the Simulator.
 */
template <class global_information_policy, class local_information_policy, class belief_policy, class behaviour_policy>
class SimulatorBuilder {
public:
	static std::shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> >
	Build(const string& config_file_name, unsigned int num_threads,
						      bool track_index_case)
	{
		// Configuration file.
		ptree pt_config;
		const auto file_path = InstallDirs::GetCurrentDir() /= config_file_name;
		if (!is_regular_file(file_path)) {
			throw runtime_error(string(__func__) + ">Config file " + file_path.string() +
					    " not present. Aborting.");
		}
		read_xml(file_path.string(), pt_config);

		// Done.
		return Build(pt_config, num_threads, track_index_case);
	}

	/// Build simulator
	static std::shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> >
	Build(const boost::property_tree::ptree& pt_config,
			unsigned int num_threads, bool track_index_case = false)
	{
		std::cout << "Start the building" << std::endl;

		// Disease file.
		ptree pt_disease;

		std::cout << "ptree disease initialized" << std::endl;

		const auto file_name_d{pt_config.get<string>("run.disease_config_file")};

		InstallDirs::GetDataDir();

		std::cout << "file name obtained for disease file" << std::endl;

		const auto file_path_d{InstallDirs::GetDataDir() /= file_name_d};

		std::cout << "file path obtained for disease file" << std::endl;

		if (!is_regular_file(file_path_d)) {
			throw runtime_error(std::string(__func__) + "> No file " + file_path_d.string());
		}

		std::cout << "disease file found" << std::endl;
		read_xml(file_path_d.string(), pt_disease);

		std::cout << "Disease config file OK" << std::endl;

		// Contact file.
		ptree pt_contact;
		const auto file_name_c{pt_config.get("run.age_contact_matrix_file", "contact_matrix.xml")};
		const auto file_path_c{InstallDirs::GetDataDir() /= file_name_c};
		if (!is_regular_file(file_path_c)) {
			throw runtime_error(string(__func__) + "> No file " + file_path_c.string());
		}
		read_xml(file_path_c.string(), pt_contact);

		std::cout << "Contact matrix file OK" << std::endl;

		// Done.
		return Build(pt_config, pt_disease, pt_contact, num_threads, track_index_case);
	}

	/// Build simulator.
	static std::shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> >
	Build(const boost::property_tree::ptree& pt_config,
						const boost::property_tree::ptree& pt_disease,
						const boost::property_tree::ptree& pt_contact,
						unsigned int number_of_threads = 1U, bool track_index_case = false)
	{
		auto sim = make_shared<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> >();

		std::cout << "Empty simulator created" << std::endl;

		const shared_ptr<spdlog::logger> logger = spdlog::get("contact_logger");

		std::cout << "Logger created" << std::endl;

		// Initialize config ptree.
		sim->m_config_pt = pt_config;

		std::cout << "Config ptree initialized" << std::endl;

		// Initialize track_index_case policy
		sim->m_track_index_case = track_index_case;

		std::cout << "Track index case policy initialized" << std::endl;

		// Initialize number of threads.
		sim->m_num_threads = number_of_threads;

		std::cout << "Num threads initialized" << std::endl;

		// Initialize calendar.
		sim->m_calendar = make_shared<Calendar>(pt_config);

		std::cout << "Calendar initialized" << std::endl;

		// Get log level.
		const string l = pt_config.get<string>("run.log_level", "None");
		sim->m_log_level =
			IsLogMode(l) ? ToLogMode(l) : throw runtime_error(string(__func__) + "> Invalid input for LogMode.");

		std::cout << "Log mode determined" << std::endl;

		// Rng's.
		const auto seed = pt_config.get<double>("run.rng_seed");
		Random rng(seed);

		std::cout << "Rngs initialized" << std::endl;

		// Build population.
		sim->m_population = PopulationBuilder<Person<behaviour_policy, belief_policy> >::Build(pt_config, pt_disease, rng);

		std::cout << "Population built" << std::endl;

		// Initialize clusters.
		InitializeClusters(sim);

		std::cout << "Clusters initialized" << std::endl;

		// Initialize population immunity
		Vaccinator<global_information_policy, local_information_policy, belief_policy, behaviour_policy>::Apply("immunity", sim, pt_config, pt_disease, rng);

		// Additional vaccine administration
		Vaccinator<global_information_policy, local_information_policy, belief_policy, behaviour_policy>::Apply("vaccine", sim, pt_config, pt_disease, rng);

		std::cout << "Vaccinations done" << std::endl;

		// Initialize disease profile.
		sim->m_disease_profile.Initialize(pt_config, pt_disease);

		std::cout << "Disease profile initialized" << std::endl;

		// --------------------------------------------------------------
		// Seed infected persons.
		// --------------------------------------------------------------
		const double seeding_rate = pt_config.get<double>("run.seeding_rate");
		double seeding_age_min = pt_config.get<double>("run.seeding_age_min");
		double seeding_age_max = pt_config.get<double>("run.seeding_age_max");
		const unsigned int max_population_index = sim->m_population->size() - 1;
		unsigned int num_infected = floor(static_cast<double>(sim->m_population->size()) * seeding_rate);
		while (num_infected > 0) {
				Person<behaviour_policy, belief_policy>& p = sim->m_population->at(rng(max_population_index));
				if (p.GetHealth().IsSusceptible() && (p.GetAge() >= seeding_age_min) && (p.GetAge() <= seeding_age_max)) {
					p.GetHealth().StartInfection();
					num_infected--;

					logger->info("[PRIM] {} {} {} {}", -1, p.GetId(), -1, 0);
				}
		}

		std::cout << "Seeding done" << std::endl;

		// Initialize Rng handlers
		unsigned int new_seed = rng(numeric_limits<unsigned int>::max());
		for (size_t i = 0; i < sim->m_num_threads; i++) {
			sim->m_rng_handler.emplace_back(RngHandler(new_seed, sim->m_num_threads, i));
		}

		std::cout << "RNG handlers initialized" << std::endl;

		// Initialize contact profiles.
		Cluster<Person<behaviour_policy, belief_policy> >::AddContactProfile(ClusterType::Household, ContactProfile(ClusterType::Household, pt_contact));
		Cluster<Person<behaviour_policy, belief_policy> >::AddContactProfile(ClusterType::School, ContactProfile(ClusterType::School, pt_contact));
		Cluster<Person<behaviour_policy, belief_policy> >::AddContactProfile(ClusterType::Work, ContactProfile(ClusterType::Work, pt_contact));
		Cluster<Person<behaviour_policy, belief_policy> >::AddContactProfile(ClusterType::PrimaryCommunity,
					   ContactProfile(ClusterType::PrimaryCommunity, pt_contact));
		Cluster<Person<behaviour_policy, belief_policy> >::AddContactProfile(ClusterType::SecondaryCommunity,
					   ContactProfile(ClusterType::SecondaryCommunity, pt_contact));

		std::cout << "Contact profiles initialized" << std::endl;

		// Done.
		return sim;
	}

private:
	/// Initialize the clusters.
	static void InitializeClusters(std::shared_ptr<Simulator<global_information_policy, local_information_policy, belief_policy, behaviour_policy> > sim)
	{
		// Determine the number of clusters.
		// Determine number of clusters.
		unsigned int max_id_households = 0U;
		unsigned int max_id_school_clusters = 0U;
		unsigned int max_id_work_clusters = 0U;
		unsigned int max_id_primary_community = 0U;
		unsigned int max_id_secondary_community = 0U;
		Population<Person<behaviour_policy, belief_policy> >& population = *sim->m_population;

		for (const auto& p : population) {
			max_id_households = std::max(max_id_households, p.GetClusterId(ClusterType::Household));
			max_id_school_clusters = std::max(max_id_school_clusters, p.GetClusterId(ClusterType::School));
			max_id_work_clusters = std::max(max_id_work_clusters, p.GetClusterId(ClusterType::Work));
			max_id_primary_community =
			    std::max(max_id_primary_community, p.GetClusterId(ClusterType::PrimaryCommunity));
			max_id_secondary_community =
			    std::max(max_id_secondary_community, p.GetClusterId(ClusterType::SecondaryCommunity));
		}

		// Keep separate id counter to provide a unique id for every cluster.
		unsigned int cluster_id = 1;

		for (size_t i = 0; i <= max_id_households; i++) {
			sim->m_households.emplace_back(Cluster<Person<behaviour_policy, belief_policy> >(cluster_id, ClusterType::Household));
			cluster_id++;
		}
		for (size_t i = 0; i <= max_id_school_clusters; i++) {
			sim->m_school_clusters.emplace_back(Cluster<Person<behaviour_policy, belief_policy> >(cluster_id, ClusterType::School));
			cluster_id++;
		}
		for (size_t i = 0; i <= max_id_work_clusters; i++) {
			sim->m_work_clusters.emplace_back(Cluster<Person<behaviour_policy, belief_policy> >(cluster_id, ClusterType::Work));
			cluster_id++;
		}
		for (size_t i = 0; i <= max_id_primary_community; i++) {
			sim->m_primary_community.emplace_back(Cluster<Person<behaviour_policy, belief_policy> >(cluster_id, ClusterType::PrimaryCommunity));
			cluster_id++;
		}
		for (size_t i = 0; i <= max_id_secondary_community; i++) {
			sim->m_secondary_community.emplace_back(Cluster<Person<behaviour_policy, belief_policy> >(cluster_id, ClusterType::SecondaryCommunity));
			cluster_id++;
		}

		// Cluster id '0' means "not present in any cluster of that type".
		for (auto& p : population) {
			const auto hh_id = p.GetClusterId(ClusterType::Household);
			if (hh_id > 0) {
				sim->m_households[hh_id].AddPerson(&p);
			}
			const auto sc_id = p.GetClusterId(ClusterType::School);
			if (sc_id > 0) {
				sim->m_school_clusters[sc_id].AddPerson(&p);
			}
			const auto wo_id = p.GetClusterId(ClusterType::Work);
			if (wo_id > 0) {
				sim->m_work_clusters[wo_id].AddPerson(&p);
			}
			const auto primCom_id = p.GetClusterId(ClusterType::PrimaryCommunity);
			if (primCom_id > 0) {
				sim->m_primary_community[primCom_id].AddPerson(&p);
			}
			const auto secCom_id = p.GetClusterId(ClusterType::SecondaryCommunity);
			if (secCom_id > 0) {
				sim->m_secondary_community[secCom_id].AddPerson(&p);
			}
		}
	}
};

} // end_of_namespace

#endif // end-of-include-guard
