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
 *  Copyright 2017, Willem L, Kuylen E, Stijven S & Broeckhove J
 */

#include "behaviour/belief_data/ThresholdWithAwarenessData.h"
#include "core/Health.h"

namespace stride {

/// Forward declaration of class Person
class Person;

template <bool threshold_infected, bool threshold_adopted>
class ThresholdWithAwareness
{
public:
	using Data = ThresholdWithAwarenessData;

	static void Initialize(Data& belief_data, double risk_averseness)
	{
		if (threshold_infected) {
			belief_data.SetThresholdInfected(1 - risk_averseness);
		}
		if (threshold_adopted) {
			belief_data.SetThresholdAdopted(1 - risk_averseness);
		}
	}

	static void Update(Data& belief_data, Health& health_data)
	{
		if ((!belief_data.IsAware()) && health_data.IsSymptomatic()) {
			belief_data.SetAware();
		}
	}

	static void Update(const Person* p)
	{
		belief_data.Contact(p);
	}

	static bool HasAdopted(const Person* p)
	{/*
		if (belief_data.IsAware()) {
			bool infected_threshold_reached = threshold_infected && (belief_data.GetFractionInfected() >=
										 belief_data.GetThresholdInfected());
			bool adopted_threshold_reached = threshold_adopted && (belief_data.GetFractionAdopted() >=
									       belief_data.GetThresholdAdopted());

			if (infected_threshold_reached || adopted_threshold_reached) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
		*/
	}
};

} // end of namespace
