/*
 * ThresholdWithAwarenessData.cpp
 *
 *  Created on: May 31, 2017
 *      Author: elise
 */

#include "ThresholdWithAwarenessData.h"
#include "pop/Person.h"

namespace stride {

template <typename BehaviourPolicy, typename BeliefPolicy>
void ThresholdWithAwarenessData::Contact(const Person<BehaviourPolicy, BeliefPolicy>* p) {
	if (IsAware()) {
		// Update fraction infected and / or fraction adopted
		m_num_contacts++;
		if (p->GetHealth().IsSymptomatic()) {
			m_num_contacts_infected++;
		}
		const auto other_belief_data = p->GetBeliefData();
		if (BeliefPolicy::HasAdopted(other_belief_data)) {
			m_num_contacts_adopted++;
		}
	} else {
		// Set aware
		SetAware();
	}
}

template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<true, false> >, ThresholdWithAwareness<true, false> >
		(const Person<Vaccination<ThresholdWithAwareness<true, false> >, ThresholdWithAwareness<true, false> >* p);
template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<false, true> >, ThresholdWithAwareness<false, true> >
		(const Person<Vaccination<ThresholdWithAwareness<false, true> >, ThresholdWithAwareness<false, true> >* p);
template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<true, true> >, ThresholdWithAwareness<true, true> >
		(const Person<Vaccination<ThresholdWithAwareness<true, true> >, ThresholdWithAwareness<true, true> >* p);


} /* namespace stride */
