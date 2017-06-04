/*
 * ThresholdWithAwarenessData.h
 *
 *  Created on: May 31, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDWITHAWARENESSDATA_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDWITHAWARENESSDATA_H_

#include "behaviour/behaviour_policies/Vaccination.h"


namespace stride {

template <typename BehaviourPolicy, typename BeliefPolicy>
class Person;

template <bool threshold_infected, bool threshold_adopted>
class ThresholdWithAwareness;

class ThresholdWithAwarenessData {
public:
	/// Default constructor
	ThresholdWithAwarenessData():
		m_num_contacts(0U), m_num_contacts_infected(0U), m_num_contacts_adopted(0U),
		m_threshold_infected(1), m_threshold_adopted(1), m_is_aware(false) {}

	void SetThresholdInfected(double threshold) {
		m_threshold_infected = threshold;
	}

	double GetThresholdInfected() const {
		return m_threshold_infected;
	}

	void SetThresholdAdopted(double threshold) {
		m_threshold_adopted = threshold;
	}

	double GetThresholdAdopted() const {
		return m_threshold_adopted;
	}

	double GetFractionInfected() const {
		if (m_num_contacts == 0) {
			return 0;
		}
		return (double)m_num_contacts_infected / m_num_contacts;
	}

	double GetFractionAdopted() const {
		if (m_num_contacts == 0) {
			return 0;
		}
		return (double)m_num_contacts_adopted / m_num_contacts;
	}

	bool IsAware() const {
		return m_is_aware;
	}

	void SetAware() {
		m_is_aware = true;
	}

	template <typename BehaviourPolicy, typename BeliefPolicy>
	void Contact(const Person<BehaviourPolicy, BeliefPolicy>* p);

private:
	unsigned int		m_num_contacts;				///<
	unsigned int		m_num_contacts_infected;	///<
	unsigned int		m_num_contacts_adopted; 	///<

	double 				m_threshold_infected;		///< Fraction of contacts that needs to be infected before person adopts belief.
	double				m_threshold_adopted;		///< Fraction of contacts that needs to have adopted the belief for person to also adopt.

	bool		m_is_aware; ///< Is the individual aware of the threat of the infection?
};


extern template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<true, false> >, ThresholdWithAwareness<true, false> >
		(const Person<Vaccination<ThresholdWithAwareness<true, false> >, ThresholdWithAwareness<true, false> >* p);
extern template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<false, true> >, ThresholdWithAwareness<false, true> >
		(const Person<Vaccination<ThresholdWithAwareness<false, true> >, ThresholdWithAwareness<false, true> >* p);
extern template void ThresholdWithAwarenessData::Contact<Vaccination<ThresholdWithAwareness<true, true> >, ThresholdWithAwareness<true, true> >
		(const Person<Vaccination<ThresholdWithAwareness<true, true> >, ThresholdWithAwareness<true, true> >* p);


} /* namespace stride */

#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDWITHAWARENESSDATA_H_ */
