/*
 * BeliefTransmission.h
 *
 *  Created on: Apr 16, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_BELIEFTRANSMISSION_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_BELIEFTRANSMISSION_H_

#include "behaviour/belief_data/BeliefTransmissionData.h"

namespace stride {

class BeliefTransmission {
public:

	using Data = BeliefTransmissionData;

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

	static void Update(Data& belief_data, const Data& p2_beliefs, const Health& p2_health) {
		if (belief_data.IsAware()) {
			bool other_infected = p2_health.IsSymptomatic();
			bool other_adopted = p2_beliefs.HasAdopted();
			if (other_infected && other_adopted) {
				belief_data.MeetInfectedAndAdopted();
			} else if (other_infected) {
				belief_data.MeetInfected();
			} else if (other_adopted) {
				belief_data.MeetAdopted();
			}
		} else {
			if (p2_beliefs.IsAware()) {
				belief_data.SetAware();
			}
		}
	}
};

} /* namespace stride */


#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_BELIEFTRANSMISSION_H_ */
