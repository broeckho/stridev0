#pragma once
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

#include "behaviour/belief_policies/Belief.h"

//#include "core/Health.h"
//#include "util/RNG.h"
//
//#include <cmath>

namespace stride {
//
//class Person;
//
//// p(behaviour) = OR0 * (OR1^x1 * OR2^x2 * OR3^x3 * OR4^x4)/ (1 + OR0 * (prod ORi^xi))

class HBM: public Belief
{
public:
	HBM(const boost::property_tree::ptree& pt): Belief(pt) {}

};

//	static void Update(Data& belief_data, Health& health_data) {}
//
//	template <typename BehaviourPolicy>
//	static void Update(Data& belief_data, const Person* p)
//	{
//	}
//
//	static bool HasAdopted(const Data& belief_data)
//	{
//		int perceived_severity = belief_data.GetPerceivedSeverity();
//		int perceived_susceptibility = belief_data.GetPerceivedSusceptibility();
//		int perceived_benefit = belief_data.GetPerceivedBenefit();
//		int perceived_barriers = belief_data.GetPerceivedBarriers();
//		double odds = m_odds_ratio_0 * std::pow(m_odds_ratio_severity, perceived_severity) *
//			      std::pow(m_odds_ratio_susceptibility, perceived_susceptibility) *
//			      std::pow(m_odds_ratio_benefit, perceived_benefit) *
//			      std::pow(m_odds_ratio_benefit, perceived_barriers);
//		double p_behaviour = odds / (1 + odds);
//		return p_behaviour >= 0.5;
//	}
//
//private:
//	static const double m_odds_ratio_0 = -1;
//	static const double m_odds_ratio_severity = 0.626;
//	static const double m_odds_ratio_susceptibility = 1.07;
//	static const double m_odds_ratio_benefit = 0;
//	static const double m_odds_ratio_barriers = -0.4;

} // end_of_namespace


//enum class VariableState
//{
//	Low = 0U,
//	High = 1U
//};
//
//class HBMData
//{
//public:
//	HBMData()
//	    : m_perceived_severity(0), m_perceived_susceptibility(0), m_perceived_benefit(1), m_perceived_barriers(1)
//	{
//	}
//
//	VariableState GetPerceivedSeverity() const { return m_perceived_severity; }
//
//	VariableState GetPerceivedSusceptibility() const { return m_perceived_susceptibility; }
//
//	VariableState GetPerceivedBenefit() const { return m_perceived_benefit; }
//
//	VariableState GetPerceivedBarriers() const { return m_perceived_barriers; }
//
//	void MeetAdopted()
//	{
//		m_num_contacts_adopted++;
//		m_num_contacts++;
//	}
//
//	void MeetInfected()
//	{
//		m_num_contacts_infected++;
//		m_num_contacts++;
//	}
//
//	void MeetInfectedAndAdopted()
//	{
//		m_num_contacts_adopted++;
//		m_num_contacts_infected++;
//		m_num_contacts++;
//	}
//
//private:
//	int m_num_contacts;
//	int m_num_contacts_infected;
//	int m_num_contacts_adopted;
//	VariableState m_perceived_severity;
//	VariableState m_perceived_susceptibility;
//	VariableState m_perceived_benefit;
//	VariableState m_perceived_barriers;
//};
