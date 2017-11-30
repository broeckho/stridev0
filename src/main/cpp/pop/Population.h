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
 * Header file for the core Population class
 */

#include <vector>

#include "pop/Person.h"

namespace stride {

/**
 * Container for persons in population.
 */
template <typename person_type>
class Population : public std::vector<person_type>
{
public:
	/// Get the cumulative number of cases.
	unsigned int GetInfectedCount() const
	{
		unsigned int total{0U};
		for (const auto& p : *this) {
			const auto& h = p.GetHealth();
			total += h.IsInfected() || h.IsRecovered();
		}
		return total;
	}

	double GetFractionInfected() const { return GetInfectedCount() / this->size(); }

	template <typename BeliefPolicy>
	unsigned int GetAdoptedCount() const
	{
		unsigned int total{0U};
		for (const auto& p : *this) {
			auto belief_data = p.GetBeliefData();
			bool adopted = BeliefPolicy::HasAdopted(belief_data);
			if (adopted) {
				total++;
			}
		}

		return total;
	}
};

} // end_of_namespace
