#ifndef SIMULATOR_INTERFACE_H_INCLUDED
#define SIMULATOR_INTERFACE_H_INCLUDED
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
 * Interface for the Simulator class, without templates
 */

#include "core/DiseaseProfile.h"
#include "sim/SimulatorObserver.h"
#include "util/Subject.h"

namespace stride {

class SimulatorInterface: public util::Subject<unsigned int, SimulatorObserver>
{
public:
	SimulatorInterface() {}

	virtual ~SimulatorInterface() {}

	/// Get the disease profile.
	virtual const DiseaseProfile GetDiseaseProfile() const = 0;

	//	const std::shared_ptr<const Population<PersonType> > GetPopulation() const = 0;

	/// Check if the simulator is operational.
	virtual bool IsOperational() const = 0;

	/// Change track_index_case setting.
	virtual void SetTrackIndexCase(bool track_index_case) = 0;

	/// Run one time step, computing full simulation (default) or only index case.
	virtual void TimeStep() = 0;
};

} /* end of namespace stride */

#endif // end-of-include-guard
