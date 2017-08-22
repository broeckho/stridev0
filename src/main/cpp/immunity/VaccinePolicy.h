#ifndef VACCINEPOLICY_H_INCLUDED
#define VACCINEPOLICY_H_INCLUDED
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
 * Header for the ImmunityProfile class.
 */

#include <cstddef>
#include <string>

namespace stride {

/// Enumerates the vaccine Policies.
enum class VaccinePolicy
{
	Random,
	Cocoon,
	None,
	Null
};

/// Number of Vaccine Policies (not including Null type).
inline constexpr unsigned int NumOfVaccinePolicy() { return 3U; }

/// Cast for array access.
inline std::size_t ToSizeType(VaccinePolicy v) { return static_cast<std::size_t>(v); }

/// Converts a VaccinePolicy value to corresponding name.
std::string ToString(VaccinePolicy v);

/// Check whether string is name of a VaccinePolicy value.
bool IsVaccinePolicy(const std::string& s);

/// Converts a string with name to VaccinePolicy value.
const VaccinePolicy ToVaccinePolicy(const std::string& s);

} // namespace

#endif // end-of-include-guard
