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
 * Implementation of the Simulator class.
 */

#include "Simulator.h"

#include "calendar/Calendar.h"
#include "calendar/DaysOffStandard.h"
#include "core/Cluster.h"
#include "core/ClusterType.h"
#include "core/Infector.h"
#include "core/LogMode.h"
#include "pop/Population.h"

#include <boost/property_tree/ptree.hpp>
#include <memory>
#include <omp.h>

namespace stride {

using namespace std;
using namespace boost::property_tree;
using namespace stride::util;

Simulator::Simulator()
    : m_config_pt(), m_num_threads(1U), m_log_level(LogMode::Null), m_population(nullptr), m_disease_profile(),
      m_track_index_case(false)
{
}

const shared_ptr<const Population> Simulator::GetPopulation() const { return m_population; }

void Simulator::SetTrackIndexCase(bool track_index_case) { m_track_index_case = track_index_case; }

template <LogMode log_level, bool track_index_case>
void Simulator::UpdateClusters()
{
#pragma omp parallel num_threads(m_num_threads)
	{
		const unsigned int thread = omp_get_thread_num();

#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_households.size(); i++) {
			Infector<log_level, track_index_case, LocalInformationPolicy>::Execute(
			    m_households[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}
#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_school_clusters.size(); i++) {
			Infector<log_level, track_index_case, LocalInformationPolicy>::Execute(
			    m_school_clusters[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}
#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_work_clusters.size(); i++) {
			Infector<log_level, track_index_case, LocalInformationPolicy>::Execute(
			    m_work_clusters[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}
#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_primary_community.size(); i++) {
			Infector<log_level, track_index_case, LocalInformationPolicy>::Execute(
			    m_primary_community[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}
#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_secondary_community.size(); i++) {
			Infector<log_level, track_index_case, LocalInformationPolicy>::Execute(
			    m_secondary_community[i], m_disease_profile, m_rng_handler[thread], m_calendar);
		}
	}
}

void Simulator::TimeStep()
{
	shared_ptr<DaysOffInterface> days_off{nullptr};

	// Logic where you compute (on the basis of input/config for initial day
	// or on the basis of number of sick persons, duration of epidemic etc)
	// what kind of DaysOff scheme you apply. If we want to make this cluster
	// dependent then the days_off object has to be passed into the Update function.
	days_off = make_shared<DaysOffStandard>(m_calendar);
	const bool is_work_off{days_off->IsWorkOff()};
	const bool is_school_off{days_off->IsSchoolOff()};

	double fraction_infected = m_population->GetFractionInfected();

	for (auto& p : *m_population) {
		p.Update(is_work_off, is_school_off, fraction_infected);
	}

	if (m_track_index_case) {
		switch (m_log_level) {
		case LogMode::Contacts:
			UpdateClusters<LogMode::Contacts, true>();
			break;
		case LogMode::Transmissions:
			UpdateClusters<LogMode::Transmissions, true>();
			break;
		case LogMode::None:
			UpdateClusters<LogMode::None, true>();
			break;
		default:
			throw runtime_error(std::string(__func__) + "Log mode screwed up!");
		}
	} else {
		switch (m_log_level) {
		case LogMode::Contacts:
			UpdateClusters<LogMode::Contacts, false>();
			break;
		case LogMode::Transmissions:
			UpdateClusters<LogMode::Transmissions, false>();
			break;
		case LogMode::None:
			UpdateClusters<LogMode::None, false>();
			break;
		default:
			throw runtime_error(std::string(__func__) + "Log mode screwed up!");
		}
	}

	m_calendar->AdvanceDay();
}
} // end_of_namespace
