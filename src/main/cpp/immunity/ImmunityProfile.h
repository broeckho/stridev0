#ifndef IMMUNITYPROFILE_H_INCLUDED
#define IMMUNITYPROFILE_H_INCLUDED
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
 * Header for the ImmunityProfile class.
 */

#include "pop/Population.h"
#include "util/Random.h"

#include <boost/property_tree/ptree.hpp>
#include <memory>

namespace stride {

/**
 * To initialize the ImmunityProfile of the Population.
 */
class ImmunityProfile
{
public:
	/**
	 * Initializes the immunity profile of the population.
	 *
	 * @param pop             Pointer to the population.
	 * @param pt_config       Property_tree with general configuration settings.
	 * @param pt_disease      Property_tree with disease configuration settings.
	 * @param rng             Object to generate random numbers.
	 * @return                Pointer to the initialized population.
	 */
	static void Initialize(std::shared_ptr<Population> pop, const boost::property_tree::ptree& pt_config,
			       const boost::property_tree::ptree& pt_disease, util::Random& rng);
};

} // end_of_namespace

#endif // include guard
