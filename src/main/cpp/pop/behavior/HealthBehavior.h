/*
 * HealthBehavior.h
 *
 *  Created on: Feb 21, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_POP_BEHAVIOR_HEALTHBEHAVIOR_H_
#define SRC_MAIN_CPP_POP_BEHAVIOR_HEALTHBEHAVIOR_H_

namespace stride {

template <class InformationPolicy, class BeliefPolicy, class BehaviorPolicy>
class HealthBehavior {
public:
	HealthBehavior();
	virtual ~HealthBehavior();
};

} /* namespace stride */

#endif /* SRC_MAIN_CPP_POP_BEHAVIOR_HEALTHBEHAVIOR_H_ */
