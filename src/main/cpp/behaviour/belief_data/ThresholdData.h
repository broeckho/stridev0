/*
 * ThresholdData.h
 *
 *  Created on: May 7, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDDATA_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDDATA_H_


/*
 * Possible variants:
 * 		+ fraction infected over entire simulation
 * 		+ fraction adopted over entire simulation
 * 		+ with awareness
 * 		+ with history
 * 		+ and off course all combinations
 */
namespace stride {

class ThresholdData {
public:
	/// Default constructor
	ThresholdData(): m_num_contacts(0U), m_num_contacts_infected(0U), m_threshold_infected(1) {}

	void SetThresholdInfected(double threshold) {
		m_threshold_infected = threshold;
	}

private:
	unsigned int		m_num_contacts;				///<
	unsigned int		m_num_contacts_infected;	///<

	double 				m_threshold_infected;		///< Fraction of contacts that needs to be infected before person adopts belief.

};

} /* namespace stride */


#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDDATA_H_ */
