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

/**
 * @file
 * Header file for NoBelief class.
 */

#include "behaviour/belief_policies/Belief.h"

//#include "core/Health.h"

namespace stride {
//
//class Person;
//
class NoBelief: public Belief
{
public:
	NoBelief(const boost::property_tree::ptree& pt): Belief(pt) {}

//	static void Initialize(Data& belief_data, double risk_averseness) {}
//
//	static void Update(Data& belief_data, Health& health_data) {}
//
//	static void Update(const Person* p) {}
//
//	static bool HasAdopted(const Data& belief_data) { return false; }
};

} // end_of_namespace
