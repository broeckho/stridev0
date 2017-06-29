#include "ThresholdData.h"
#include "pop/Person.h"
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

namespace stride {

template <typename BehaviourPolicy, typename BeliefPolicy>
void ThresholdData::Contact(const Person<BehaviourPolicy, BeliefPolicy>* p)
{
	m_num_contacts++;
	if (p->GetHealth().IsSymptomatic()) {
		m_num_contacts_infected++;
	}
	const auto other_belief_data = p->GetBeliefData();
	if (BeliefPolicy::HasAdopted(other_belief_data)) {
		m_num_contacts_adopted++;
	}
}

template void ThresholdData::Contact<Vaccination<Threshold<true, false>>, Threshold<true, false>>(
    const Person<Vaccination<Threshold<true, false>>, Threshold<true, false>>* p);
template void ThresholdData::Contact<Vaccination<Threshold<false, true>>, Threshold<false, true>>(
    const Person<Vaccination<Threshold<false, true>>, Threshold<false, true>>* p);
template void ThresholdData::Contact<Vaccination<Threshold<true, true>>, Threshold<true, true>>(
    const Person<Vaccination<Threshold<true, true>>, Threshold<true, true>>* p);

} /* namespace stride */
