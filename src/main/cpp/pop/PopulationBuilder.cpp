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

#include "PopulationBuilder.h"

#include "core/Cluster.h"
#include "core/Health.h"
#include "pop/Person.h"
#include "pop/Population.h"
#include "util/InstallDirs.h"
#include "util/Random.h"
#include "util/StringUtils.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <spdlog/spdlog.h>

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace stride {

using namespace std;
using namespace boost::filesystem;
using namespace boost::property_tree;
using namespace stride::util;

shared_ptr<Population> PopulationBuilder::Build(const boost::property_tree::ptree& pt_config,
						const boost::property_tree::ptree& pt_disease, util::Random& rng)
{
	//------------------------------------------------
	// Setup.
	//------------------------------------------------
	const auto pop = make_shared<Population>();
	Population& population = *pop;
	const double seeding_rate = pt_config.get<double>("run.seeding_rate");
	const double immunity_rate = pt_config.get<double>("run.immunity_rate");
	const string disease_config_file = pt_config.get<string>("run.disease_config_file");

	//------------------------------------------------
	// Logger.
	//------------------------------------------------
	const shared_ptr<spdlog::logger> logger = spdlog::get("contact_logger");

	//------------------------------------------------
	// Check input.
	//------------------------------------------------
	bool status = (seeding_rate <= 1) && (immunity_rate <= 1); // && ((seeding_rate + immunity_rate) <= 1);
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
		population.emplace_back(Simulator::PersonType(
		    person_id, StringUtils::FromString<unsigned int>(values[0]),
		    StringUtils::FromString<unsigned int>(values[1]), StringUtils::FromString<unsigned int>(values[2]),
		    StringUtils::FromString<unsigned int>(values[3]), StringUtils::FromString<unsigned int>(values[4]),
		    StringUtils::FromString<unsigned int>(values[5]), start_infectiousness, start_symptomatic,
		    time_infectious, time_symptomatic, risk_averseness));
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
			Simulator::PersonType& p = population[rng(max_population_index)];
			if (!p.IsParticipatingInSurvey()) {
				p.ParticipateInSurvey();
				logger->info("[PART] {} {} {} {} {}", p.GetId(), p.GetAge(), p.GetGender(),
					     p.GetClusterId(ClusterType::School), p.GetClusterId(ClusterType::Work));
				num_samples++;
			}
		}
	}

	//------------------------------------------------
	// Set population immunity.
	//------------------------------------------------
	const string immunity_profile = pt_config.get<string>("run.immunity_profile");
	std::stringstream ss;
	ss << "disease.immunity." << immunity_profile;
	std::string xml_immunity_profile = ss.str();

	std::cout << "using immunity profile: " << xml_immunity_profile << std::endl;

	if (!pt_disease.get_child_optional(xml_immunity_profile)) {

		std::cout << "using average immunity: " << immunity_rate << std::endl;
		unsigned int num_immune = floor(static_cast<double>(population.size()) * immunity_rate);
		while (num_immune > 0) {
			Simulator::PersonType& p = population[rng(max_population_index)];
			if (p.GetHealth().IsSusceptible()) {
				p.GetHealth().SetImmune();
				num_immune--;
			}
		}
	} else { // with age-specific immunity
		std::cout << "using age-specific immunity: " << xml_immunity_profile << std::endl;

		const auto distrib_immunity = GetDistribution(pt_disease, xml_immunity_profile);

		vector<double> population_count_age;
		for (unsigned int index_age = 0; index_age < 100; index_age++) {
			population_count_age.push_back(0);
		}

		// set all individuals to "immune"
		for (unsigned int i = 0; i < population.size(); i++) {
			Simulator::PersonType& p = population[i];
			p.GetHealth().SetImmune();
			population_count_age[p.GetAge()]++;
		}

		// select some individuals to "susceptible", given age specific probabilities
		//        	std::cout << "population immune... start selecting susceptibles" << std::endl;
		//        	std::cout << "population size " << population.size() << std::endl;

		for (unsigned int index_age = 0; index_age < distrib_immunity.size(); index_age++) {

			unsigned int num_susceptible =
			    floor(population_count_age[index_age] * (1 - distrib_immunity[index_age]));
			// std::cout << "age " << index_age << std::endl;
			//        		std::cout << "num_susceptible " << num_susceptible << std::endl;
			//        		std::cout << "(1-distrib_immunity[index_age]) " <<
			//        (1-distrib_immunity[index_age]) << std::endl;

			while (num_susceptible > 0) {
				Simulator::PersonType& p = population[rng(max_population_index)];
				if (p.GetAge() == index_age && p.GetHealth().IsImmune()) {
					p.GetHealth().SetSusceptible();
					num_susceptible--;
					// std::cout << "** num_susceptible " << num_susceptible << std::endl;
				}
			} // end num_susceptible while loop
		}         // end index_age for loop

		if (xml_immunity_profile == "disease.immunity.cocoon") {
			std::cout << "** COCOON " << std::endl;
			// target individuals aged 20-38 years with children <1 year
			for (unsigned int i = 0; i < population.size(); i++) {
				Simulator::PersonType& p = population[i];

				if (p.GetHealth().IsSusceptible() && p.GetAge() > 20 && p.GetAge() < 38) {

					unsigned int household_id = p.GetClusterId(ClusterType::Household);
					bool has_infant = false;

					for (unsigned int i2 = 0; i2 < population.size() && !has_infant; i2++) {
						Simulator::PersonType& p2 = population[i2];
						if (p2.GetAge() < 1 &&
						    p2.GetClusterId(ClusterType::Household) == household_id) {
							has_infant = true;
						}
					}
					// std::cout << "** has_infant " << has_infant << "		age: "<<
					// p.GetAge()<<  std::endl;

					if (has_infant == true && rng.NextDouble() < 0.8) {
						p.GetHealth().SetImmune();
						std::cout << "** set immune " << i << "		age: " << p.GetAge()
							  << std::endl;
					}
				}
			}
		}

	} // end age-specific immunity ELSE

	//------------------------------------------------
	// Seed infected persons.
	//------------------------------------------------
	unsigned int num_infected = floor(static_cast<double>(population.size()) * seeding_rate);
	while (num_infected > 0) {
		Simulator::PersonType& p = population[rng(max_population_index)];
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

vector<double> PopulationBuilder::GetDistribution(const boost::property_tree::ptree& pt_root, const string& xml_tag)
{
	vector<double> values;
	boost::property_tree::ptree subtree = pt_root.get_child(xml_tag);
	for (const auto& tree : subtree) {
		values.push_back(tree.second.get<double>(""));
	}
	return values;
}

unsigned int PopulationBuilder::Sample(Random& rng, const vector<double>& distribution)
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

} // end_of_namespace
