/*
 * LocalDiscussion.h
 *
 *  Created on: May 26, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_BEHAVIOUR_INFORMATION_POLICIES_LOCALDISCUSSION_H_
#define SRC_MAIN_CPP_BEHAVIOUR_INFORMATION_POLICIES_LOCALDISCUSSION_H_

namespace stride {

template <typename PersonType>
class LocalDiscussion {
public:
	static void Update(PersonType* p1, PersonType* p2);
private:
	static double  m_discussion_probability;
};

} /* namespace stride */


#endif /* SRC_MAIN_CPP_BEHAVIOUR_INFORMATION_POLICIES_LOCALDISCUSSION_H_ */
