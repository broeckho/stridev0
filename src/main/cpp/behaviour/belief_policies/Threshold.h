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

#include "belief.h"
#include "behaviour/belief_data/ThresholdData.h"

namespace stride {

template <bool threshold_infected, bool threshold_adopted>
class Threshold : public Belief
{
public:
	using Data = ThresholdData;

	static void Initialize(Data& belief_data, double risk_averseness)
	{
		if (threshold_infected) {
			belief_data.SetThresholdInfected(1 - risk_averseness);
		}
		if (threshold_adopted) {
			belief_data.SetThresholdAdopted(1 - risk_averseness);
		}
	}

	static void Update(Data& belief_data, Health& health_data) {}

	static void Update(const Person* p)
	{
		// belief_data.Contact<BehaviourPolicy, Threshold<threshold_infected, threshold_adopted>>(p);
	}

	static bool HasAdopted(const Person* p)
	{/*
		if (threshold_infected) {
			if (belief_data.GetFractionInfected() > belief_data.GetThresholdInfected()) {
				return true;
			}
		}
		if (threshold_adopted) {
			if (belief_data.GetFractionAdopted() > belief_data.GetThresholdAdopted()) {
				return true;
			}
		}

		return false;
		*/
	}
};

} // end of namespace
