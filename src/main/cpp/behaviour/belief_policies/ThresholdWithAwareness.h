/*
 * ThresholdWithAwareness.h
 *
 *  Created on: May 31, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLDWITHAWARENESS_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLDWITHAWARENESS_H_

#include "behaviour/belief_data/ThresholdWithAwarenessData.h"
#include "core/Health.h"

namespace stride {

/// Forward declaration of class Person
template<typename BehaviourPolicy, typename BeliefPolicy>
class Person;

template <bool threshold_infected, bool threshold_adopted>
class ThresholdWithAwareness {
public:
	using Data = ThresholdWithAwarenessData;

	static void Initialize(Data& belief_data, double risk_averseness) {
		if (threshold_infected) {
			belief_data.SetThresholdInfected(1 - risk_averseness);
		}
		if (threshold_adopted) {
			belief_data.SetThresholdAdopted(1 - risk_averseness);
		}
	}

	static void Update(Data& belief_data, Health& health_data) {
		if ((! belief_data.IsAware()) && health_data.IsSymptomatic()) {
			belief_data.SetAware();
		}
	}

	template<typename BehaviourPolicy>
	static void Update(Data& belief_data, const Person<BehaviourPolicy, ThresholdWithAwareness<threshold_infected, threshold_adopted> >* p) {
		belief_data.Contact(p);
	}

	static bool HasAdopted(const Data& belief_data) {
		if (belief_data.IsAware()) {
			bool infected_threshold_reached = threshold_infected && (belief_data.GetFractionInfected() >= belief_data.GetThresholdInfected());
			bool adopted_threshold_reached = threshold_adopted && (belief_data.GetFractionAdopted() >= belief_data.GetThresholdAdopted());

			if (infected_threshold_reached || adopted_threshold_reached) {
				return true;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
};

} /* namespace stride */

#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLDWITHAWARENESS_H_ */
