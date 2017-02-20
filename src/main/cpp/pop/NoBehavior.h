/*
 * NoBehavior.h
 *
 *  Created on: Feb 20, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_POP_NOBEHAVIOR_H_
#define SRC_MAIN_CPP_POP_NOBEHAVIOR_H_

#include "BehaviorInterface.h"

namespace stride {

class NoBehavior: public BehaviorInterface {
public:
	NoBehavior() {}
	virtual ~NoBehavior() {}

	/// Check whether a person practices a certain behavior
	virtual bool PracticesBehavior() const { return false; }

	/// Check whether a person talks about certain behavior
	virtual bool PreachesBehavior() const { return false; }

	/// Update behavior based on global information
	virtual void Update(int num_infected) {}  //TODO more general 'global' info?

	/// Update behavior based on local information
	virtual void Update(std::shared_ptr<Person>& other_person) {}
};

} /* namespace stride */

#endif /* SRC_MAIN_CPP_POP_NOBEHAVIOR_H_ */
