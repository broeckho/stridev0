#pragma once
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
 *  Copyright 2017, Kuylen E, Willem L, Broeckhove J
 */

#include "pop/Person.h"

namespace stride {

/**
 * Class for local information policy 'LocalDiscussion'.
 * Upon contact, members of a cluster discuss their beliefs/health status/...
 * with each other with a certain probability.
 * TODO make this probability configurable
 */
class LocalDiscussion
{
//public:
//	static void Update(Person* p1, Person* p2)
//	{
//		if (RNG::GetInstance().NextDouble() < 1.0) {
//			p1->Update(p2);
//			p2->Update(p1);
//		}
//	}
};

} // end_of_namespace
