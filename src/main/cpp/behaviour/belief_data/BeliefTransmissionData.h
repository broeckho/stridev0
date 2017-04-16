/*
 * BeliefTransmission.h
 *
 *  Created on: Apr 16, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_BELIEFTRANSMISSIONDATA_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_BELIEFTRANSMISSIONDATA_H_

namespace stride {

class BeliefTransmissionData {
public:
	BeliefTransmissionData(): m_num_contacts_adopted(0), m_num_contacts_infected(0), m_num_contacts(0),
				m_threshold_adopted(100), m_threshold_infected(100), m_aware(false), m_adopted(false)
	{}

	virtual ~BeliefTransmissionData() {}

	bool IsAware() const {
		return m_aware;
	}

	void SetAware() {
		m_aware = true;
	}

	bool HasAdopted() const {
		return m_adopted;
	}

	void MeetAdopted() {
		m_num_contacts_adopted ++;
		m_num_contacts ++;
	}

	void MeetInfected() {
		m_num_contacts_infected ++;
		m_num_contacts ++;
	}

	void MeetInfectedAndAdopted() {
		m_num_contacts_adopted ++;
		m_num_contacts_infected ++;
		m_num_contacts++;
	}

private:
	unsigned int		m_num_contacts_adopted;  	///< Number of this person's contacts that has adopted behaviour in question.
	unsigned int		m_num_contacts_infected;	///< Number of this person's contacts that is infected.
	unsigned int		m_num_contacts;

	double				m_threshold_adopted;
	double 				m_threshold_infected;

	bool				m_aware;
	bool 				m_adopted;
};

} /* namespace stride */

#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_BELIEFTRANSMISSIONDATA_H_ */
