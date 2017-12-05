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
 * Header for the PersonFile class.
 */

#include "pop/Population.h"

#include <fstream>
#include <memory>
#include <string>

namespace stride {
namespace output {

/**
 * Produces a file with daily cases count.
 */
class PersonFile
{
public:
	/// Constructor: initialize.
	PersonFile(const std::string& file = "stride_person");

	/// Destructor: close the file stream.
	~PersonFile();

	/// Print the given cases with corresponding tag.
	void Print(const std::shared_ptr<const Population> population)
	{
		for (const auto& p : *population) {
			const auto& h = p.GetHealth();
			m_fstream << p.GetId() << "," << p.GetAge() << "," << h.IsRecovered() << "," << h.IsImmune()
				  << "," << h.GetStartInfectiousness() << "," << h.GetEndInfectiousness() << ","
				  << h.GetStartSymptomatic() << "," << h.GetEndSymptomatic() << std::endl;
		}
	}

private:
	/// Generate file name and open the file stream.
	void Initialize(const std::string& file);

private:
	std::ofstream m_fstream; ///< The file stream.
};

} // end_of_namespace
} // end_of_namespace
