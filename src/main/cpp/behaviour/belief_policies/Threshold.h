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
//#include "behaviour/belief_data/ThresholdData.h"
//
//namespace stride {
//
//template <bool threshold_infected, bool threshold_adopted>
//class Threshold
//{
//public:
//	using Data = ThresholdData;
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
//	static void Update(Data& belief_data, Health& health_data) {}
//
//	static void Update(const Person* p)
//	{
//		// belief_data.Contact<BehaviourPolicy, Threshold<threshold_infected, threshold_adopted>>(p);
//	}
//
//	static bool HasAdopted(const Person* p)
//	{/*
//		if (threshold_infected) {
//			if (belief_data.GetFractionInfected() > belief_data.GetThresholdInfected()) {
//				return true;
//			}
//		}
//		if (threshold_adopted) {
//			if (belief_data.GetFractionAdopted() > belief_data.GetThresholdAdopted()) {
//				return true;
//			}
//		}
//
//		return false;
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
///*
// * Possible variants:
// * 		+ fraction adopted over entire simulation
// * 		+ with awareness
// * 		+ with history
// * 		+ and off course all combinations
// */
//namespace stride {
//
//class Person;
//
//template <bool threshold_infected, bool threshold_adopted>
//class Threshold;
//
///**
// *
// */
//class ThresholdData
//{
//public:
//	/// Default constructor
//	ThresholdData()
//	    : m_num_contacts(0U), m_num_contacts_infected(0U), m_num_contacts_adopted(0U), m_threshold_infected(1),
//	      m_threshold_adopted(1)
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
//	void Contact(const Person* p);
//
//private:
//	unsigned int m_num_contacts;          ///<
//	unsigned int m_num_contacts_infected; ///<
//	unsigned int m_num_contacts_adopted;  ///<
//
//	double m_threshold_infected; ///< Fraction of contacts that needs to be infected before person adopts belief.
//	double m_threshold_adopted;  ///< Fraction of contacts that needs to have adopted the belief for person to also
//				     /// adopt.
//};
//
//} // end of namespace

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
//#include "ThresholdData.h"
//#include "pop/Person.h"
//
//namespace stride {
//
//void ThresholdData::Contact(const Person* p)
//{
//        /*
//	m_num_contacts++;
//	if (p->GetHealth().IsSymptomatic()) {
//		m_num_contacts_infected++;
//	}
//	const auto other_belief_data = p->GetBeliefData();
//	if (BeliefPolicy::HasAdopted(other_belief_data)) {
//		m_num_contacts_adopted++;
//	}
//	*/
//}
//
//} // end of namespace

