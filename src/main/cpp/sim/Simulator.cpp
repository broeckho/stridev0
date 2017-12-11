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
 * Implementation for the Simulator class.
 */

#include "Simulator.h"

#include "behaviour/information_policies/LocalDiscussion.h"
#include "behaviour/information_policies/NoLocalInformation.h"
#include "calendar/DaysOffStandard.h"
#include "core/Infector.h"

#include <omp.h>

namespace stride {

/// Default constructor for empty Simulator.
Simulator::Simulator()
	: m_pt_config(), m_num_threads(1U), m_log_level(LogMode::Null),
	  m_population(nullptr), m_disease_profile(), m_track_index_case(false)
{}

/// Run one time step, computing full simulation (default) or only index case.
void Simulator::TimeStep()
{
	std::shared_ptr<DaysOffInterface> days_off{nullptr};

	// Logic where you compute (on the basis of input/config for initial day
	// or on the basis of number of sick persons, duration of epidemic etc)
	// what kind of DaysOff scheme you apply. If we want to make this cluster
	// dependent then the days_off object has to be passed into the Update function.
	days_off = std::make_shared<DaysOffStandard>(m_calendar);
	const bool is_work_off{days_off->IsWorkOff()};
	const bool is_school_off{days_off->IsSchoolOff()};

	// Update individual's health status & presence in clusters.
	for (auto& p : *m_population) {
		p.Update(is_work_off, is_school_off);
	}

	if (m_local_information_policy == "NoLocalInformation") {
		if (m_track_index_case) {
			switch(m_log_level) {
			case LogMode::Contacts:
				UpdateClusters<LogMode::Contacts, NoLocalInformation, true>();
				break;
			case LogMode::Transmissions:
				UpdateClusters<LogMode::Transmissions, NoLocalInformation, true>();
				break;
			case LogMode::None:
				UpdateClusters<LogMode::None, NoLocalInformation, true>();
				break;
			default:
				throw std::runtime_error(std::string(__func__) + "Log mode screwed up!");
			}
		} else {
			switch (m_log_level) {
			case LogMode::Contacts:
				UpdateClusters<LogMode::Contacts, NoLocalInformation, false>();
				break;
			case LogMode::Transmissions:
				UpdateClusters<LogMode::Transmissions, NoLocalInformation, false>();
				break;
			case LogMode::None:
				UpdateClusters<LogMode::None, NoLocalInformation, false>();
				break;
			default:
				throw std::runtime_error(std::string(__func__) + "Log mode screwed up!");
			}
		}
	} else if (m_local_information_policy == "LocalDiscussion") {
		if (m_track_index_case) {
			switch (m_log_level) {
			case LogMode::Contacts:
				UpdateClusters<LogMode::Contacts, LocalDiscussion, true>();
				break;
			case LogMode::Transmissions:
				UpdateClusters<LogMode::Transmissions, LocalDiscussion, true>();
				break;
			case LogMode::None:
				UpdateClusters<LogMode::None, LocalDiscussion, true>();
				break;
			default:
				throw std::runtime_error(std::string(__func__) + "Log mode screwed up!");
			}
		} else {
			switch (m_log_level) {
			case LogMode::Contacts:
				UpdateClusters<LogMode::Contacts, LocalDiscussion, false>();
				break;
			case LogMode::Transmissions:
				UpdateClusters<LogMode::Transmissions, LocalDiscussion, false>();
				break;
			case LogMode::None:
				UpdateClusters<LogMode::None, LocalDiscussion, false>();
				break;
			default:
				throw std::runtime_error(std::string(__func__) + "Log mode screwed up!");
			}
		}
	} else {
		throw std::runtime_error(std::string(__func__) + "No valid local information policy!");
	}

	Notify(m_calendar->GetSimulationDay());

	m_calendar->AdvanceDay();
}

/// Update the contacts in the given clusters.
template <LogMode log_level, typename local_information_policy, bool track_index_case>
void Simulator::UpdateClusters()
{
	#pragma omp parallel num_threads(m_num_threads)
	{
		const unsigned int thread = omp_get_thread_num();

		#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_households.size(); i++) {
			Infector<log_level, track_index_case, local_information_policy>::Execute(
					m_households[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}

		#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_school_clusters.size(); i++) {
			Infector<log_level, track_index_case, local_information_policy>::Execute(
					m_school_clusters[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}

		#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_work_clusters.size(); i++) {
			Infector<log_level, track_index_case, local_information_policy>::Execute(
					m_work_clusters[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}

		#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_secondary_community.size(); i++) {
			Infector<log_level, track_index_case, local_information_policy>::Execute(
					m_secondary_community[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}
	}
}

} // end_of_namespace
