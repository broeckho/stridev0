#ifndef SIMULATOR_H_INCLUDED
#define SIMULATOR_H_INCLUDED
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
 * Header for the Simulator class.
 */

#include "calendar/Calendar.h"
#include "calendar/DaysOffStandard.h"
#include "core/Cluster.h"
#include "core/DiseaseProfile.h"
#include "core/Infector.h"
#include "core/LogMode.h"
#include "core/RngHandler.h"
#include "pop/Person.h"
#include "pop/Population.h"
#include "sim/SimulatorInterface.h"
#include "util/Subject.h"

#include <boost/property_tree/ptree.hpp>
#include <omp.h>
#include <string>

namespace stride {

/**
 * Main class that contains and direct the virtual world.
 */
template <class global_information_policy, class local_information_policy, class belief_policy, class behaviour_policy>
class Simulator: public SimulatorInterface
{
public:
	/// Information policies
	using GlobalInformationPolicy = global_information_policy;
	using LocalInformationPolicy = local_information_policy;

	/// Belief policy
	using BeliefPolicy = belief_policy;

	/// Behaviour policy
	using BehaviourPolicy = behaviour_policy;

	/// Person type
	using PersonType = Person<BehaviourPolicy, BeliefPolicy>;

public:
	/// Default constructor for empty Simulator.
	Simulator(): m_config_pt(), m_num_threads(1U), m_log_level(LogMode::Null), m_population(nullptr), m_disease_profile(),
		m_track_index_case(false)
	{}

	/// Get the population.
	const std::shared_ptr<const Population<PersonType> > GetPopulation() const { return m_population; }

	/// Get the disease profile.
	const DiseaseProfile GetDiseaseProfile() const { return m_disease_profile; }

	/// Check if the simulator is operational.
	bool IsOperational() const
	{
		return GetDiseaseProfile().IsOperational();
	}

	/// Change track_index_case setting.
	void SetTrackIndexCase(bool track_index_case) { m_track_index_case = track_index_case; }

	/// Run one time step, computing full simulation (default) or only index case.
	void TimeStep()
	{
		std::shared_ptr<DaysOffInterface> days_off{nullptr};

		// Logic where you compute (on the basis of input/config for initial day
		// or on the basis of number of sick persons, duration of epidemic etc)
		// what kind of DaysOff scheme you apply. If we want to make this cluster
		// dependent then the days_off object has to be passed into the Update function.
		days_off = std::make_shared<DaysOffStandard>(m_calendar);
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
				throw std::runtime_error(std::string(__func__) + "Log mode screwed up!");
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
				throw std::runtime_error(std::string(__func__) + "Log mode screwed up!");
			}
		}

		m_calendar->AdvanceDay();

		// Notify observers
		Notify(m_calendar->GetSimulationDay());
	}

private:
	/// Update the contacts in the given clusters.
	template <LogMode log_level, bool track_index_case = false>
	void UpdateClusters()
	{
#pragma omp parallel num_threads(m_num_threads)
		{
			const unsigned int thread = omp_get_thread_num();

#pragma omp for schedule(runtime)
			for (size_t i = 0; i < m_households.size(); i++) {
				Infector<log_level, track_index_case, LocalInformationPolicy, PersonType>::Execute(
						m_households[i], m_disease_profile, m_rng_handler[thread], m_calendar);
			}

#pragma omp for schedule(runtime)
			for (size_t i = 0; i < m_school_clusters.size(); i++) {
				Infector<log_level, track_index_case, LocalInformationPolicy, PersonType>::Execute(
						m_school_clusters[i], m_disease_profile, m_rng_handler[thread], m_calendar);
			}

#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_work_clusters.size(); i++) {
			Infector<log_level, track_index_case, LocalInformationPolicy, PersonType>::Execute(
			    m_work_clusters[i], m_disease_profile, m_rng_handler[thread], m_calendar);
			}

#pragma omp for schedule(runtime)
		for (size_t i = 0; i < m_secondary_community.size(); i++) {
			Infector<log_level, track_index_case, LocalInformationPolicy, PersonType>::Execute(
			    m_secondary_community[i], m_disease_profile, m_rng_handler[thread], m_calendar);
			}
		}
	}

private:
	boost::property_tree::ptree m_config_pt; ///< Configuration property tree

private:
	unsigned int m_num_threads;            ///< The number of (OpenMP) threads.
	std::vector<RngHandler> m_rng_handler; ///< Pointer to the RngHandlers.
	LogMode m_log_level;                   ///< Specifies logging mode.
	std::shared_ptr<Calendar> m_calendar;  ///< Management of calendar.

private:
	std::shared_ptr<Population<PersonType> > m_population; ///< Pointer to the Population.

	std::vector<Cluster<PersonType> > m_households;          ///< Container with household Clusters.
	std::vector<Cluster<PersonType> > m_school_clusters;     ///< Container with school Clusters.
	std::vector<Cluster<PersonType> > m_work_clusters;       ///< Container with work Clusters.
	std::vector<Cluster<PersonType> > m_primary_community;   ///< Container with primary community Clusters.
	std::vector<Cluster<PersonType> > m_secondary_community; ///< Container with secondary community  Clusters.

	DiseaseProfile m_disease_profile; ///< Profile of disease.
	bool m_track_index_case; ///< General simulation or tracking index case.


private:
	template <class b_global_information_policy, class b_local_information_policy, class b_belief_policy, class b_behaviour_policy>
	friend class SimulatorBuilder;

	template <class c_global_information_policy, class c_local_information_policy, class c_belief_policy, class c_behaviour_policy>
	friend class Vaccinator;
};

} // end_of_namespace

#endif // end-of-include-guard
