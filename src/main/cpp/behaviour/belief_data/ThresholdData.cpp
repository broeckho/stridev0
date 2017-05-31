/*
 * ThresholdData.cpp
 *
 *  Created on: May 8, 2017
 *      Author: elise
 */

#include "ThresholdData.h"
#include "pop/Person.h"

namespace stride {

template <typename BehaviourPolicy, typename BeliefPolicy>
void ThresholdData::Contact(const Person<BehaviourPolicy, BeliefPolicy>* p) {
	m_num_contacts++;
	if (p->GetHealth().IsSymptomatic()) {
		m_num_contacts_infected++;
	}
	const auto other_belief_data = p->GetBeliefData();
	if (BeliefPolicy::HasAdopted(other_belief_data)) {
		m_num_contacts_adopted++;
	}
}

template void ThresholdData::Contact<Vaccination<Threshold<true, false> >, Threshold<true, false> >
		(const Person<Vaccination<Threshold<true, false> >, Threshold<true, false> >* p);
template void ThresholdData::Contact<Vaccination<Threshold<false, true> >, Threshold<false, true> >
		(const Person<Vaccination<Threshold<false, true> >, Threshold<false, true> >* p);
template void ThresholdData::Contact<Vaccination<Threshold<true, true> >, Threshold<true, true> >
		(const Person<Vaccination<Threshold<true, true> >, Threshold<true, true> >* p);

} /* namespace stride */
