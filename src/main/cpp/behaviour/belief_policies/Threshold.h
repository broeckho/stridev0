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

	static void Update(Data& belief_data, Health& health_data) {

			// if fraction infected > threshold : adopt belief
			// or if fraction adopted > threshold : adopt belief
			/*if (belief_data.GetFractionContactsAdopted() > belief_data.GetThresholdAdopted()
					|| belief_data.GetFractionContactsInfected() > belief_data.GetThresholdInfected()) {
				belief_data.SetAdopted();
			}*/
	}

	template<typename BehaviourPolicy>
	static void Update(Data& belief_data, const Person<BehaviourPolicy, Threshold<threshold_infected, threshold_adopted> >* p) {

	}

	/*
	template<typename BehaviourPolicy>
	static void Update(Data& belief_data, const Person<BehaviourPolicy, BeliefTransmission>* p) {
			bool other_infected = p->GetHealth().IsSymptomatic();
			bool other_adopted = p->GetBeliefData().HasAdopted();

			if (other_infected && other_adopted) {
				belief_data.MeetInfectedAndAdopted();
			} else if (other_infected) {
				belief_data.MeetInfected();
			} else if (other_adopted) {
				belief_data.MeetAdopted();
			}

	}
	 */

};

template<>
class Threshold<true, false> {
public:
	using Data = ThresholdData;

	static void Update(Data& belief_data, Health& health_data) {

	}

	template<typename BehaviourPolicy>
	static void Update(Data& belief_data, const Person<BehaviourPolicy, Threshold<true, false> >* p) {

	}
};


/// Explicit instantiations in .cpp file
extern template class Threshold<true, false>;
extern template class Threshold<false, true>;
extern template class Threshold<true, true>;


#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_THRESHOLD_H_ */

} /* namespace stride */
