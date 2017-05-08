/*
 * Threshold.h
 *
 *  Created on: May 7, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLD_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLD_H_

#include "behaviour/belief_data/ThresholdData.h"
#include "core/Health.h"

namespace stride {

/// Forward declaration of class Person
template<typename BehaviourPolicy, typename BeliefPolicy>
class Person;

template <bool threshold_infected, bool threshold_adopted>
class Threshold {
public:
	using Data = ThresholdData;

	static void Initialize(Data& belief_data, double risk_averseness) {
		belief_data.SetThresholdInfected(1 - risk_averseness);
	}

	static void Update(Data& belief_data, Health& health_data) {

	}

	template<typename BehaviourPolicy>
	static void Update(Data& belief_data, const Person<BehaviourPolicy, Threshold<threshold_infected, threshold_adopted> >* p) {
		belief_data.Contact<BehaviourPolicy, Threshold<threshold_infected, threshold_adopted> >(p);
	}

	static bool HasAdopted(Data& belief_data) {
		if (belief_data.GetFractionInfected() > belief_data.GetThresholdInfected()) {
			return true;
		} else {
			return false;
		}
	}

};


/// Explicit instantiations in .cpp file
extern template class Threshold<true, false>;
extern template class Threshold<false, true>;
extern template class Threshold<true, true>;


#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLD_H_ */

} /* namespace stride */
