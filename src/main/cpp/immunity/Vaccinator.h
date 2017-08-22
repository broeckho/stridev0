#ifndef VACCINATOR_H_INCLUDED
#define VACCINATOR_H_INCLUDED
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
 * Header for the Vaccinator class.
 */

#include "immunity/VaccinePolicy.h"
#include "sim/Simulator.h"
#include "util/Random.h"

#include <boost/property_tree/ptree.hpp>
#include <memory>

#include <iostream>

namespace stride {

class Vaccinator
{
public:
	template <VaccinePolicy vaccin_policy>
	static void ApplyPolicy(std::shared_ptr<Simulator> sim, const boost::property_tree::ptree& pt_config,
				const boost::property_tree::ptree& pt_disease, util::Random& rng);

	static void Apply(const std::string vaccine_policy, std::shared_ptr<Simulator> sim,
			  const boost::property_tree::ptree& pt_config, const boost::property_tree::ptree& pt_disease,
			  util::Random& rng);
};

} // end_of_namespace

#endif // include guard
