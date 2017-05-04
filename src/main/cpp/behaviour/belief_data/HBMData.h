/*
 * HBMData.h
 *
 *  Created on: Apr 19, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_HBMDATA_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_HBMDATA_H_

namespace stride {

enum class VariableState {Low = 0U, High = 1U};

class HBMData {

public:
	HBMData():
		m_perceived_severity(0), m_perceived_susceptibility(0),
		m_perceived_benefit(0), m_perceived_barriers(0) {}

	VariableState GetPerceivedSeverity() const {
		return m_perceived_severity;
	}

	VariableState GetPerceivedSusceptibility() const {
		return m_perceived_susceptibility;
	}

	VariableState GetPerceivedBenefit() const {
		return m_perceived_benefit;
	}

	VariableState GetPerceivedBarriers() const {
		return m_perceived_barriers;
	}

private:
	VariableState m_perceived_severity;
	VariableState m_perceived_susceptibility;
	VariableState m_perceived_benefit;
	VariableState m_perceived_barriers;
};

}

#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_HBMDATA_H_ */
