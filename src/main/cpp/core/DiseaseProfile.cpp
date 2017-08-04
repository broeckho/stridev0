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
 *  Copyright 2015, Willem L, Kuylen E, Stijven S & Broeckhove J
 */

/**
 * @file
 * Disease profile.
 */

#include "DiseaseProfile.h"

#include <iostream>
#include <math.h> /* sqrt */

namespace stride {

using namespace std;
using namespace boost::property_tree;

void DiseaseProfile::Initialize(const ptree& pt_config, const ptree& pt_disease)
{

	// Use a quadratic model, fitted to simulation data: Expected(R0) = (0 + b1*transm_rate + b2*transm_rate^2).
	const double r0 = pt_config.get<double>("run.r0");
	const double b0 = pt_disease.get<double>("disease.transmission.b0");
	const double b1 = pt_disease.get<double>("disease.transmission.b1");
	const double b2 = pt_disease.get<double>("disease.transmission.b2");

	// find root
	const double a = b2;
	const double b = b1;
	const double c = b0 - r0;

	// to obtain a real values (instead of complex)
	if (r0 < (-(b * b) / (4 * a))) {
		const double determ = (b * b) - 4 * a * c;
		m_transmission_rate = (-b + sqrt(determ)) / (2 * a);
	} else {
		std::cout << "ILLIGAL R0 VALUE" << std::endl;
		std::cout << "MAX R0 VALUE: " << (-(b * b) / (4 * a)) << std::endl;
		m_transmission_rate = 0;
	}
}

} // namespace
