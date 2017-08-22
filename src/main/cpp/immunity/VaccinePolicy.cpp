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

/**
 * @file
 * Implementation of VaccinePolicy.
 */

#include "VaccinePolicy.h"

#include <boost/algorithm/string.hpp>
#include <map>
#include <string>

namespace {

using stride::VaccinePolicy;
using boost::to_upper;
using namespace std;

map<VaccinePolicy, string> g_vaccine_policy_name{
    make_pair(VaccinePolicy::Random, "Random"), make_pair(VaccinePolicy::Cocoon, "Cocoon"),
    make_pair(VaccinePolicy::None, "None"), make_pair(VaccinePolicy::Null, "Null")};

map<string, VaccinePolicy> g_name_vaccine_policy{
    make_pair("RANDOM", VaccinePolicy::Random), make_pair("COCOON", VaccinePolicy::Cocoon),
    make_pair("NONE", VaccinePolicy::None), make_pair("NULL", VaccinePolicy::Null)};
}

namespace stride {

string ToString(VaccinePolicy v) { return (g_vaccine_policy_name.count(v) == 1) ? g_vaccine_policy_name[v] : "Null"; }

bool IsVaccinePolicy(const string& s)
{
	std::string t{s};
	to_upper(t);
	return (g_name_vaccine_policy.count(t) == 1);
}

const VaccinePolicy ToVaccinePolicy(const string& s)
{
	std::string t{s};
	to_upper(t);
	return (g_name_vaccine_policy.count(t) == 1) ? g_name_vaccine_policy[t] : VaccinePolicy::Null;
}

} // namespace
