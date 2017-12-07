//#pragma once
///*
// *  This is free software: you can redistribute it and/or modify it
// *  under the terms of the GNU General Public License as published by
// *  the Free Software Foundation, either version 3 of the License, or
// *  any later version.
// *  The software is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// *  GNU General Public License for more details.
// *  You should have received a copy of the GNU General Public License
// *  along with the software. If not, see <http://www.gnu.org/licenses/>.
// *
// *  Copyright 2017, Willem L, Kuylen E, Stijven S & Broeckhove J
// */
//
//#include "behaviour/belief_data/ThresholdWithAwarenessData.h"
//#include "core/Health.h"
//
//namespace stride {
//
///// Forward declaration of class Person
//class Person;
//
//template <bool threshold_infected, bool threshold_adopted>
//class ThresholdWithAwareness
//{
//public:
//	using Data = ThresholdWithAwarenessData;
//
//	static void Initialize(Data& belief_data, double risk_averseness)
//	{
//		if (threshold_infected) {
//			belief_data.SetThresholdInfected(1 - risk_averseness);
//		}
//		if (threshold_adopted) {
//			belief_data.SetThresholdAdopted(1 - risk_averseness);
//		}
//	}
//
//	static void Update(Data& belief_data, Health& health_data)
//	{
//		if ((!belief_data.IsAware()) && health_data.IsSymptomatic()) {
//			belief_data.SetAware();
//		}
//	}
//
//	static void Update(const Person* p)
//	{
//		belief_data.Contact(p);
//	}
//
//	static bool HasAdopted(const Person* p)
//	{/*
//		if (belief_data.IsAware()) {
//			bool infected_threshold_reached = threshold_infected && (belief_data.GetFractionInfected() >=
//										 belief_data.GetThresholdInfected());
//			bool adopted_threshold_reached = threshold_adopted && (belief_data.GetFractionAdopted() >=
//									       belief_data.GetThresholdAdopted());
//
//			if (infected_threshold_reached || adopted_threshold_reached) {
//				return true;
//			} else {
//				return false;
//			}
//		} else {
//			return false;
//		}
//		*/
//	}
//};
//
//} // end of namespace

//#pragma once
///*
// *  This is free software: you can redistribute it and/or modify it
// *  under the terms of the GNU General Public License as published by
// *  the Free Software Foundation, either version 3 of the License, or
// *  any later version.
// *  The software is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// *  GNU General Public License for more details.
// *  You should have received a copy of the GNU General Public License
// *  along with the software. If not, see <http://www.gnu.org/licenses/>.
// *
// *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
// */
//
//#include "behaviour/behaviour_policies/Vaccination.h"
//
//namespace stride {
//
//template <typename BehaviourPolicy, typename BeliefPolicy>
//class Person;
//
//template <bool threshold_infected, bool threshold_adopted>
//class ThresholdWithAwareness;
//
//class ThresholdWithAwarenessData
//{
//public:
//	/// Default constructor
//	ThresholdWithAwarenessData()
//	    : m_num_contacts(0U), m_num_contacts_infected(0U), m_num_contacts_adopted(0U), m_threshold_infected(1),
//	      m_threshold_adopted(1), m_is_aware(false)
//	{
//	}
//
//	void SetThresholdInfected(double threshold) { m_threshold_infected = threshold; }
//
//	double GetThresholdInfected() const { return m_threshold_infected; }
//
//	void SetThresholdAdopted(double threshold) { m_threshold_adopted = threshold; }
//
//	double GetThresholdAdopted() const { return m_threshold_adopted; }
//
//	double GetFractionInfected() const
//	{
//		if (m_num_contacts == 0) {
//			return 0;
//		}
//		return (double)m_num_contacts_infected / m_num_contacts;
//	}
//
//	double GetFractionAdopted() const
//	{
//		if (m_num_contacts == 0) {
//			return 0;
//		}
//		return (double)m_num_contacts_adopted / m_num_contacts;
//	}
//
//	bool IsAware() const { return m_is_aware; }
//
//	void SetAware() { m_is_aware = true; }
//
//	template <typename BehaviourPolicy, typename BeliefPolicy>
//	void Contact(const Person<BehaviourPolicy, BeliefPolicy>* p);
//
//private:
//	unsigned int m_num_contacts;          ///<
//	unsigned int m_num_contacts_infected; ///<
//	unsigned int m_num_contacts_adopted;  ///<
//
//	double m_threshold_infected; ///< Fraction of contacts that needs to be infected before person adopts belief.
//	double m_threshold_adopted;  ///< Fraction of contacts that needs to have adopted the belief for person to also
//	/// adopt.
//
//	bool m_is_aware; ///< Is the individual aware of the threat of the infection?
//};
//
//extern template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<true, false>>,
//							 ThresholdWithAwareness<true, false>>(
//    const Person<Vaccination<ThresholdWithAwareness<true, false>>, ThresholdWithAwareness<true, false>>* p);
//extern template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<false, true>>,
//							 ThresholdWithAwareness<false, true>>(
//    const Person<Vaccination<ThresholdWithAwareness<false, true>>, ThresholdWithAwareness<false, true>>* p);
//extern template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<true, true>>,
//							 ThresholdWithAwareness<true, true>>(
//    const Person<Vaccination<ThresholdWithAwareness<true, true>>, ThresholdWithAwareness<true, true>>* p);
//
//} // end of namespace


//#include "ThresholdWithAwarenessData.h"
//#include "pop/Person.h"
//
//namespace stride {
//
//template <typename BehaviourPolicy, typename BeliefPolicy>
//void ThresholdWithAwarenessData::Contact(const Person<BehaviourPolicy, BeliefPolicy>* p)
//{
//	if (IsAware()) {
//		// Update fraction infected and / or fraction adopted
//		m_num_contacts++;
//		if (p->GetHealth().IsSymptomatic()) {
//			m_num_contacts_infected++;
//		}
//		const auto other_belief_data = p->GetBeliefData();
//		if (BeliefPolicy::HasAdopted(other_belief_data)) {
//			m_num_contacts_adopted++;
//		}
//	} else {
//		// Set aware
//		SetAware();
//	}
//}
//
//template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<true, false>>,
//						  ThresholdWithAwareness<true, false>>(
//    const Person<Vaccination<ThresholdWithAwareness<true, false>>, ThresholdWithAwareness<true, false>>* p);
//template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<false, true>>,
//						  ThresholdWithAwareness<false, true>>(
//    const Person<Vaccination<ThresholdWithAwareness<false, true>>, ThresholdWithAwareness<false, true>>* p);
//template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<true, true>>,
//						  ThresholdWithAwareness<true, true>>(
//    const Person<Vaccination<ThresholdWithAwareness<true, true>>, ThresholdWithAwareness<true, true>>* p);
//
//} // end of namespace stride

