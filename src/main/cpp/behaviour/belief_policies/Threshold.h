#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLD_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLD_H_
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

#include "behaviour/belief_data/ThresholdData.h"
//#include "core/Health.h"

namespace stride {

template <bool threshold_infected, bool threshold_adopted>
class Threshold
{
public:
	using Data = ThresholdData;

	static void Initialize(Data& belief_data, double risk_averseness) {
		if (threshold_infected) {
			belief_data.SetThresholdInfected(1 - risk_averseness);
		}
		if (threshold_adopted) {
			belief_data.SetThresholdAdopted(1 - risk_averseness);
		}
	}

	static void Update(Data& belief_data, Health& health_data) {}

	template <typename BehaviourPolicy>
	static void Update(Data& belief_data,
			const Person<BehaviourPolicy, Threshold<threshold_infected, threshold_adopted> >* p)
	{
		belief_data.Contact<BehaviourPolicy, Threshold<threshold_infected, threshold_adopted> >(p);
	}

	static bool HasAdopted(const Data& belief_data) {
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
	}
};

} /* end of namespace stride */

#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLD_H_ */
