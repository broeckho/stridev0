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
 * Implementation of ClusterType.
 */

#include "ImmunityProfile.h"

#include "core/Cluster.h"
#include "core/Health.h"
#include "pop/Person.h"
#include "pop/Population.h"
#include "util/PtreeUtils.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

namespace stride {

using namespace util;

void ImmunityProfile::Initialize(std::shared_ptr<Population> pop, const boost::property_tree::ptree& pt_config,
				 const boost::property_tree::ptree& pt_disease, util::Random& rng)
{

	Population& population = *pop;
	const unsigned int max_population_index = population.size() - 1;

	const double immunity_rate = pt_config.get<double>("run.immunity_rate");

	std::stringstream ss;
	ss << "disease.immunity_profile." << pt_config.get<string>("run.immunity_profile");
	std::string xml_immunity_profile = ss.str();

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
		const auto distrib_immunity = PtreeUtils::GetDistribution(pt_disease, xml_immunity_profile);

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

		for (unsigned int index_age = 0; index_age < distrib_immunity.size(); index_age++) {

			unsigned int num_susceptible =
			    floor(population_count_age[index_age] * (1 - distrib_immunity[index_age]));
//			std::cout << index_age << "** num_susceptible " << num_susceptible << std::endl;

			while (num_susceptible > 0) {
				Simulator::PersonType& p = population[rng(max_population_index)];
				if (p.GetAge() == index_age && p.GetHealth().IsImmune()) {
					p.GetHealth().SetSusceptible();
					num_susceptible--;

				}
			} // end num_susceptible while loop
		}  // end index_age for loop

		if (xml_immunity_profile == "disease.immunity_profile.cocoon") {
	//		                         disease.immunity_profile.cocoon
			std::cout << "** COCOON " << std::endl;

			const double adult_age_min = pt_disease.get<double>(xml_immunity_profile + "_ages.adult_age_min");
			const double adult_age_max = pt_disease.get<double>(xml_immunity_profile + "_ages.adult_age_max");
			const double child_age_min = pt_disease.get<double>(xml_immunity_profile + "_ages.child_age_max");
			const double child_age_max = pt_disease.get<double>(xml_immunity_profile + "_ages.child_age_max");

			std::cout << adult_age_min << " " << adult_age_max << " " << child_age_min << " " << child_age_max << std::endl;

			// target individuals aged 20-38 years with children <1 year
			for (unsigned int i = 0; i < population.size(); i++) {
				Simulator::PersonType& p = population[i];

				if (p.GetHealth().IsSusceptible() && p.GetAge() >= adult_age_min && p.GetAge() <= adult_age_max) {

					unsigned int household_id = p.GetClusterId(ClusterType::Household);
					bool has_infant = false;

					for (unsigned int i2 = 0; i2 < population.size() && !has_infant; i2++) {
						Simulator::PersonType& p2 = population[i2];
						if (p2.GetAge() >= child_age_min && p2.GetAge() <= child_age_max &&
						    p2.GetClusterId(ClusterType::Household) == household_id) {
							has_infant = true;
						}
					}
					// std::cout << "** has_infant " << has_infant << "		age: "<<
					// p.GetAge()<<  std::endl;

					if (has_infant == true && rng.NextDouble() < 0.8) {
						p.GetHealth().SetImmune();
//						std::cout << "** set immune " << i << "		age: " << p.GetAge()
//							  << std::endl;
					}
				}
			}
		}

	} // end age-specific immunity ELSE
}

} // namespace stride
