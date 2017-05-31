/*
 * ThresholdWithAwarenessData.h
 *
 *  Created on: May 31, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDWITHAWARENESSDATA_H_
#define SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDWITHAWARENESSDATA_H_

#include "ThresholdData.h"

namespace stride {

class ThresholdWithAwarenessData: public ThresholdData {
public:
	bool IsAware() {
		return m_is_aware;
	}

	void SetAware() {
		m_is_aware = true;
	}

private:
	bool		m_is_aware; ///< Is the individual aware of the threat of the infection?
};

} /* namespace stride */

#endif /* SRC_MAIN_CPP_BEHAVIOUR_BELIEF_DATA_THRESHOLDWITHAWARENESSDATA_H_ */
