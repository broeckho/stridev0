#ifndef SRC_MAIN_CPP_BEHAVIOUR_INFORMATION_POLICIES_LOCALDISCUSSION_H_
#define SRC_MAIN_CPP_BEHAVIOUR_INFORMATION_POLICIES_LOCALDISCUSSION_H_
/*
 *  This is free software: you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  any later version.
 *  The software is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License
 *  along with the software. If not, see <http://www.gnu.org/licenses/>.
 *
 *  Copyright 2017, Willem L, Kuylen E, Stijven S & Broeckhove J
 */

#include "util/RNG.h"

namespace stride {

template <typename PersonType>
class LocalDiscussion
{
public:
	static void Update(PersonType* p1, PersonType* p2)
	{
		if (RNG::GetInstance().NextDouble() < 1.0) {
			p1->Update(p2);
			p2->Update(p1);
		}
	}
};

} /* namespace stride */

#endif /* SRC_MAIN_CPP_BEHAVIOUR_INFORMATION_POLICIES_LOCALDISCUSSION_H_ */
