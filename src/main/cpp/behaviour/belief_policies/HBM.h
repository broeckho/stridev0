/*
 * HBM.h
 *
 *  Created on: Apr 20, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_HBM_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_HBM_H_

#include "core/Health.h"
#include "util/RNG.h"
#include "behaviour/belief_data/HBMData.h"

#include <cmath>

namespace stride {

template<typename BehaviourPolicy, typename BeliefPolicy>
class Person;

/*
 * p(behaviour) = OR0 * (OR1^x1 * OR2^x2 * OR3^x3 * OR4^x4)/ (1 + OR0 * (prod ORi^xi))
 */

class HBM {
public:
	using Data = HBMData;

	static void Update(Data& belief_data, Health& health_data) {

	}

	template<typename BehaviourPolicy>
	static void Update(Data& belief_data, const Person<BehaviourPolicy, HBM>* p) {

	}

	static bool HasAdopted(const Data& belief_data) {
		int perceived_severity = belief_data.GetPerceivedSeverity();
		int perceived_susceptibility = belief_data.GetPerceivedSusceptibility();
		int perceived_benefit = belief_data.GetPerceivedBenefit();
		int perceived_barriers = belief_data.GetPerceivedBarriers();
		double odds = m_odds_ratio_0 * std::pow(m_odds_ratio_severity, perceived_severity)
									 * std::pow(m_odds_ratio_susceptibility, perceived_susceptibility)
									 * std::pow(m_odds_ratio_benefit, perceived_benefit)
									 * std::pow(m_odds_ratio_benefit, perceived_barriers);
		double p_behaviour = odds / (1 + odds);
		return p_behaviour >= 0.5;
	}

private:
	static const double m_odds_ratio_0;
	static const double m_odds_ratio_severity;
	static const double m_odds_ratio_susceptibility;
	static const double m_odds_ratio_benefit;
	static const double m_odds_ratio_barriers;
};

} /* namespace stride */


#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_POLICIES_HBM_H_ */
