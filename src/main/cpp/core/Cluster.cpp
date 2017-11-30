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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

/**
 * @file
 * Header for the core Cluster class.
 */

#include "Cluster.h"

namespace stride {

using namespace std;

template <typename person_type>
array<ContactProfile, NumOfClusterTypes()> Cluster<person_type>::g_profiles;

template <typename person_type>
Cluster<person_type>::Cluster(std::size_t cluster_id, ClusterType cluster_type)
    : m_cluster_id(cluster_id), m_cluster_type(cluster_type), m_index_immune(0),
      m_profile(g_profiles.at(ToSizeType(m_cluster_type)))
{
}

template <typename person_type>
void Cluster<person_type>::AddContactProfile(ClusterType cluster_type, const ContactProfile& profile)
{
	g_profiles.at(ToSizeType(cluster_type)) = profile;
}

template <typename person_type>
void Cluster<person_type>::AddPerson(person_type* p)
{
	m_members.emplace_back(std::make_pair(p, true));
	m_index_immune++;
}

template <typename person_type>
tuple<bool, size_t> Cluster<person_type>::SortMembers()
{
	bool infectious_cases = false;
	size_t num_cases = 0;

	for (size_t i_member = 0; i_member < m_index_immune; i_member++) {
		// if immune, move to back
		if (m_members[i_member].first->GetHealth().IsImmune()) {
			bool swapped = false;
			size_t new_place = m_index_immune - 1;
			m_index_immune--;
			while (!swapped && new_place > i_member) {
				if (m_members[new_place].first->GetHealth().IsImmune()) {
					m_index_immune--;
					new_place--;
				} else {
					swap(m_members[i_member], m_members[new_place]);
					swapped = true;
				}
			}
		}
		// else, if not susceptible, move to front
		else if (!m_members[i_member].first->GetHealth().IsSusceptible()) {
			if (!infectious_cases && m_members[i_member].first->GetHealth().IsInfectious()) {
				infectious_cases = true;
			}
			if (i_member > num_cases) {
				swap(m_members[i_member], m_members[num_cases]);
			}
			num_cases++;
		}
	}

	return std::make_tuple(infectious_cases, num_cases);
}

template <typename person_type>
void Cluster<person_type>::UpdateMemberPresence()
{
	for (auto& member : m_members) {
		member.second = member.first->IsInCluster(m_cluster_type);
	}
}

//--------------------------------------------------------------------------
// All explicit instantiations.
//--------------------------------------------------------------------------
template class Cluster<Person<NoBehaviour<NoBelief>, NoBelief>>;
template class Cluster<Person<Vaccination<Threshold<true, false>>, Threshold<true, false>>>;
template class Cluster<Person<Vaccination<Threshold<false, true>>, Threshold<false, true>>>;
template class Cluster<Person<Vaccination<Threshold<true, true>>, Threshold<true, true>>>;

} // end_of_namespace
