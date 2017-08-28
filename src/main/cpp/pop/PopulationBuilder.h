#ifndef POPULATION_BUILDER_H_INCLUDED
#define POPULATION_BUILDER_H_INCLUDED
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
 * Initialize populations.
 */

#include "core/ClusterType.h"
#include "pop/Population.h"
#include "util/InstallDirs.h"
#include "util/Random.h"
#include "util/StringUtils.h"

#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace stride {

using namespace std;
using namespace stride::util;

/**
 * Initializes Population objects.
 */
template <typename person_type>
class PopulationBuilder
{
public:
	/**
	 * Initializes a Population: add persons, set immunity, seed infection.
	 *
	 * @param pt_config			Property_tree with general configuration settings.
	 * @param pt_disease		Property_tree with disease configuration settings.
	 * @return					Pointer to the initialized population.
	 */
	static std::shared_ptr<Population<person_type> > Build(const boost::property_tree::ptree& pt_config,
			const boost::property_tree::ptree& pt_disease, util::Random& rng) {
		// ------------------------------------------------
		// Setup.
		// ------------------------------------------------
		const auto pop = make_shared<Population<person_type> >();
		Population<person_type>& population = *pop;

		const double seeding_rate = pt_config.get<double>("run.seeding_rate");
		const double immunity_rate = pt_config.get<double>("run.immunity_rate");
		const string disease_config_file = pt_config.get<string>("run.disease_config_file");

		// ------------------------------------------------
		// Logger.
		// ------------------------------------------------
		const shared_ptr<spdlog::logger> logger = spdlog::get("contact_logger");

		//------------------------------------------------
		// Check input.
		//------------------------------------------------
		bool status = (seeding_rate <= 1) && (immunity_rate <= 1) && ((seeding_rate + immunity_rate) <= 1);
		if (!status) {
			throw runtime_error(string(__func__) + "> Bad input data.");
		}

		//------------------------------------------------
		// Add persons to population.
		//------------------------------------------------
		const auto file_name = pt_config.get<string>("run.population_file");
		;
		const auto file_path = InstallDirs::GetDataDir() /= file_name;
		if (!is_regular_file(file_path)) {
			throw runtime_error(string(__func__) + "> Population file " + file_path.string() + " not present.");
		}

		boost::filesystem::ifstream pop_file;
		pop_file.open(file_path.string());
		if (!pop_file.is_open()) {
			throw runtime_error(string(__func__) + "> Error opening population file " + file_path.string());
		}

		const auto distrib_start_infectiousness = GetDistribution(pt_disease, "disease.start_infectiousness");
		const auto distrib_start_symptomatic = GetDistribution(pt_disease, "disease.start_symptomatic");
		const auto distrib_time_infectious = GetDistribution(pt_disease, "disease.time_infectious");
		const auto distrib_time_symptomatic = GetDistribution(pt_disease, "disease.time_symptomatic");

		string line;
		getline(pop_file, line); // step over file header
		unsigned int person_id = 0U;

		while (getline(pop_file, line)) {
			// Make use of stochastic disease characteristics.
			const auto start_infectiousness = Sample(rng, distrib_start_infectiousness);
			const auto start_symptomatic = Sample(rng, distrib_start_symptomatic);
			const auto time_infectious = Sample(rng, distrib_time_infectious);
			const auto time_symptomatic = Sample(rng, distrib_time_symptomatic);

			const auto values = StringUtils::Split(line, ",");
			auto risk_averseness = 0.0;

			if (values.size() > 6) {
				risk_averseness = StringUtils::FromString<double>(values[6]);
			}

			unsigned int age = StringUtils::FromString<unsigned int>(values[0]);
			unsigned int household_id = StringUtils::FromString<unsigned int>(values[1]);
			unsigned int school_id = StringUtils::FromString<unsigned int>(values[2]);
			unsigned int work_id = StringUtils::FromString<unsigned int>(values[3]);
			unsigned int primary_community_id = StringUtils::FromString<unsigned int>(values[4]);
			unsigned int secondary_community_id = StringUtils::FromString<unsigned int>(values[5]);

			population.emplace_back(person_type(person_id, age, household_id, school_id, work_id, primary_community_id,
					secondary_community_id, start_infectiousness, start_symptomatic, time_infectious, time_symptomatic,
					risk_averseness));

			++person_id;
		}

		pop_file.close();

		//------------------------------------------------
		// Customize the population.
		//------------------------------------------------
		const unsigned int max_population_index = population.size() - 1;
		if (max_population_index <= 1U) {
			throw runtime_error(string(__func__) + "> Problem with population size.");
		}

		//------------------------------------------------
		// Set participants in social contact survey.
		//------------------------------------------------
		const string log_level = pt_config.get<string>("run.log_level", "None");
		if (log_level == "Contacts") {
			const unsigned int num_participants = pt_config.get<double>("run.num_participants_survey");

			// use a while-loop to obtain 'num_participant' unique participants (default sampling is with
			// replacement)
			// A for loop will not do because we might draw the same person twice.
			unsigned int num_samples = 0;
			while (num_samples < num_participants) {
				person_type& p = population[rng(max_population_index)];
				if (!p.IsParticipatingInSurvey()) {
					p.ParticipateInSurvey();
					logger->info("[PART] {}", p.GetId());
					logger->info("[PART] {} {} {} {} {}", p.GetId(), p.GetAge(), p.GetGender(),
						     p.GetClusterId(ClusterType::School), p.GetClusterId(ClusterType::Work));
					num_samples++;
				}
			}
		}

		// ------------------------------------------------
		// Set population immunity.
		// ------------------------------------------------
		unsigned int num_immune = floor(static_cast<double>(population.size()) * immunity_rate);
		while (num_immune > 0) {
			person_type& p = population[rng(max_population_index)];
			if (p.GetHealth().IsSusceptible()) {
				p.GetHealth().SetImmune();
				num_immune--;
			}
		}

		//------------------------------------------------
		// Seed infected persons.
		//------------------------------------------------
		unsigned int num_infected = floor(static_cast<double>(population.size()) * seeding_rate);
		while (num_infected > 0) {
			person_type& p = population[rng(max_population_index)];
			if (p.GetHealth().IsSusceptible()) {
				p.GetHealth().StartInfection();
				num_infected--;

				logger->info("[PRIM] {} {} {} {}", p.GetId(), -1, -1, 0);
			}
		}

		//------------------------------------------------
		// Done
		//------------------------------------------------
		return pop;
	}

private:
	/// Get distribution associated with tag values.
	static std::vector<double> GetDistribution(const boost::property_tree::ptree& pt_root,
			const std::string& xml_tag)
	{
		vector<double> values;
		boost::property_tree::ptree subtree = pt_root.get_child(xml_tag);
		for (const auto& tree : subtree) {
			values.push_back(tree.second.get<double>(""));
		}
		return values;
	}


	/// Sample from the distribution.
	static unsigned int Sample(util::Random& rng, const std::vector<double>& distribution)
	{
		double random_value = rng.NextDouble();
		for (unsigned int i = 0; i < distribution.size(); i++) {
			if (random_value <= distribution[i]) {
				return i;
			}
		}
		cerr << "WARNING: PROBLEM WITH DISEASE DISTRIBUTION [PopulationBuilder]" << endl;
		return distribution.size();
	}
};

} // end_of_namespace

#endif // include guard
