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
 * Implementation of Vaccinator class.
 */

#include "immunity/Vaccinator.h"
#include "immunity/VaccinePolicy.h"

#include "core/Cluster.h"
#include "core/Health.h"
#include "pop/Person.h"
#include "pop/Population.h"
#include "sim/Simulator.h"
#include "util/PtreeUtils.h"
#include "util/StringUtils.h"

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

template <VaccinePolicy V>
void Vaccinator::ApplyPolicy(std::shared_ptr<Simulator> sim, const boost::property_tree::ptree& pt_config,
			     const boost::property_tree::ptree& pt_disease, util::Random& rng)
{
	std::cout << "NO ADDITIONAL VACCINATION" << endl;
}

template <>
void Vaccinator::ApplyPolicy<VaccinePolicy::Random>(std::shared_ptr<Simulator> sim,
						    const boost::property_tree::ptree& pt_config,
						    const boost::property_tree::ptree& pt_disease, util::Random& rng)
{
	std::cout << "RANDOM ADDITIONAL VACCINATION" << endl;

	Population& population = *sim->m_population;
	const double vaccine_coverage = pt_config.get<double>("run.vaccine_coverage");

	for (unsigned int i = 0; i < population.size(); i++) {
		Simulator::PersonType& p = population[i];

		if (p.GetHealth().IsSusceptible() && rng.NextDouble() < vaccine_coverage) {
			p.GetHealth().SetImmune();
		}
	}
}

template <>
void Vaccinator::ApplyPolicy<VaccinePolicy::Cocoon>(std::shared_ptr<Simulator> sim,
						    const boost::property_tree::ptree& pt_config,
						    const boost::property_tree::ptree& pt_disease, util::Random& rng)
{
	std::cout << "COCOON VACCINATION" << endl;

	Population& population = *sim->m_population;
	const double vaccine_coverage = pt_config.get<double>("run.vaccine_coverage");

	std::stringstream ss;
	ss << "disease.immunity_profile." << StringUtils::ToLower(pt_config.get<string>("run.vaccine_policy"));
	std::string xml_immunity_profile = ss.str();
	const double adult_age_min = pt_disease.get<double>(xml_immunity_profile + "_settings.adult_age_min");
	const double adult_age_max = pt_disease.get<double>(xml_immunity_profile + "_settings.adult_age_max");
	const double child_age_min = pt_disease.get<double>(xml_immunity_profile + "_settings.child_age_max");
	const double child_age_max = pt_disease.get<double>(xml_immunity_profile + "_settings.child_age_max");

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

			if (has_infant == true && rng.NextDouble() < vaccine_coverage) {
				p.GetHealth().SetImmune();
			}
		}
	}
}

void Vaccinator::Apply(const std::string s, std::shared_ptr<Simulator> sim,
		       const boost::property_tree::ptree& pt_config, const boost::property_tree::ptree& pt_disease,
		       util::Random& rng)
{

	VaccinePolicy vaccine_policy =
	    IsVaccinePolicy(s) ? ToVaccinePolicy(s)
			       : throw runtime_error(string(__func__) + "> Invalid input for VaccinePolicy.");

	switch (vaccine_policy) {
	case VaccinePolicy::Random:
		ApplyPolicy<VaccinePolicy::Random>(sim, pt_config, pt_disease, rng);
		break;
	case VaccinePolicy::Cocoon:
		ApplyPolicy<VaccinePolicy::Cocoon>(sim, pt_config, pt_disease, rng);
		break;
	case VaccinePolicy::None:
		ApplyPolicy<VaccinePolicy::None>(sim, pt_config, pt_disease, rng);
		break;
	default:
		throw runtime_error(std::string(__func__) + "Vaccine Policy screwed up!");
	}
}

} // end_of_namespace
