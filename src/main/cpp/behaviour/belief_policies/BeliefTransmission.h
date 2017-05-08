/*
 * BeliefTransmission.h
 *
 *  Created on: Apr 16, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_BELIEFTRANSMISSION_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_BELIEFTRANSMISSION_H_

#include "behaviour/belief_data/BeliefTransmissionData.h"
#include "core/Health.h"
#include "util/RNG.h"


namespace stride {

template<typename BehaviourPolicy, typename BeliefPolicy>
class Person;

class BeliefTransmission {
public:

	using Data = BeliefTransmissionData;

	static void Initialize(Data& belief_data, double risk_averseness) {}

	static void Update(Data& belief_data, Health& health_data) {
		if (belief_data.IsAware()) {
			// if fraction infected > threshold : adopt belief
			// or if fraction adopted > threshold : adopt belief
			if (belief_data.GetFractionContactsAdopted() > belief_data.GetThresholdAdopted()
					|| belief_data.GetFractionContactsInfected() > belief_data.GetThresholdInfected()) {
				belief_data.SetAdopted();
			}
		} else {
			// If symptomatic, person is aware
			if (health_data.IsSymptomatic()) {
				belief_data.SetAware();
			}
		}
	}

	template<typename BehaviourPolicy>
	static void Update(Data& belief_data, const Person<BehaviourPolicy, BeliefTransmission>* p) {
		if (belief_data.IsAware()) {
			bool other_infected = p->GetHealth().IsSymptomatic();
			bool other_adopted = p->GetBeliefData().HasAdopted();

			if (other_infected && other_adopted) {
				belief_data.MeetInfectedAndAdopted();
			} else if (other_infected) {
				belief_data.MeetInfected();
			} else if (other_adopted) {
				belief_data.MeetAdopted();
			}

		} else {
			if (p->GetBeliefData().IsAware()) {
				if (RNG::GetInstance().NextDouble() < m_p_discussion) {
					belief_data.SetAware();
				}
			}

		}
	}

	static bool HasAdopted(const Data& belief_data) {
		return belief_data.HasAdopted();
	}

private:
	static double 					 m_p_discussion;			///< The probability that an aware individual discusses disease
};

} /* namespace stride */


#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_BELIEFTRANSMISSION_H_ */
