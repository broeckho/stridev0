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
		// TODO set aware when individual is infected
	}

	template<typename BehaviourPolicy>
	static void Update(Data& belief_data, const Person<BehaviourPolicy, ThresholdWithAwareness<threshold_infected, threshold_adopted> >* p) {
		// TODO update
	}

	static bool HasAdopted(const Data& belief_data) {
		// TODO check
		return false;
	}
};

} /* namespace stride */

#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLDWITHAWARENESS_H_ */
