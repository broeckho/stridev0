/*
 * BehaviorInterface.h
 *
 *  Created on: Feb 18, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_POP_BEHAVIORINTERFACE_H_
#define SRC_MAIN_CPP_POP_BEHAVIORINTERFACE_H_

#include <memory>

namespace stride {

class Person;

class BehaviorInterface {
public:
	virtual ~BehaviorInterface() {}

	/// Check whether a person practices a certain behavior
	virtual bool PracticesBehavior() const = 0;

	/// Check whether a person talks about certain behavior
	virtual bool PreachesBehavior() const = 0;

	/// Update behavior based on global information
	virtual void Update(int num_infected) = 0; //TODO more general 'global' info?

	/// Update behavior based on local information
	virtual void Update(std::shared_ptr<Person>& other_person) = 0;

};

} /* namespace stride */

#endif /* SRC_MAIN_CPP_POP_BEHAVIORINTERFACE_H_ */
