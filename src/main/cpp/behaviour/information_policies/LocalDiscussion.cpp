/*
 * LocalDiscussion.cpp
 *
 *  Created on: May 26, 2017
 *      Author: elise
 */

#include "LocalDiscussion.h"
#include "util/RNG.h"

namespace stride {

template <typename PersonType>
double LocalDiscussion<PersonType>::m_discussion_probability = 1.0;

template <typename PersonType>
void LocalDiscussion<PersonType>::Update(PersonType* p1, PersonType* p2) {
	if (RNG::GetInstance().NextDouble() < m_discussion_probability) {
		p1->Update(p2);
		p2->Update(p1);
	}
}

} /* namespace stride */
