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
 * Implementation of Infector algorithms.
 */

#include "core/Cluster.h"
#include "core/ContactHandler.h"
#include "core/Health.h"
#include "core/Infector.h"
#include "core/LogMode.h"
#include "core/Person.h"
#include "sim/WorldEnvironment.h"

#include "spdlog/spdlog.h"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace stride {

using namespace std;

//--------------------------------------------------------------------------
// Primary R0_POLICY: do nothing i.e. track all cases.
//--------------------------------------------------------------------------
template<bool track_index_case = false>
class R0_POLICY
{
public:
        static void Execute(Person* p) {}
};

//--------------------------------------------------------------------------
// Specialized R0_POLICY: track only the index case.
//--------------------------------------------------------------------------
template<>
class R0_POLICY<true>
{
public:
        static void Execute(Person* p) { p->GetHealth().StopInfection(); }
};

//--------------------------------------------------------------------------
// Primary LOG_POLICY policy, implements LogMode::None.
//--------------------------------------------------------------------------
template<LogMode log_level = LogMode::None>
class LOG_POLICY
{
public:
        static void Execute(shared_ptr<spdlog::logger> logger, Person* p1, Person* p2,
                ClusterType cluster_type, shared_ptr<const WorldEnvironment> environ)
        {}
};

//--------------------------------------------------------------------------
// Specialized LOG_POLICY policy LogMode::Transmissions.
//--------------------------------------------------------------------------
template<>
class LOG_POLICY<LogMode::Transmissions>
{
public:
        static void Execute(shared_ptr<spdlog::logger> logger, Person* p1, Person* p2,
                ClusterType cluster_type, shared_ptr<const WorldEnvironment> environ)
        {
                logger->info("[TRAN] {} {} {} {}",
                       p1->GetId(), p2->GetId(), ToString(cluster_type), environ->GetSimulationDay());
        }
};

//--------------------------------------------------------------------------
// Specialized LOG_POLICY policy LogMode::Contacts.
//--------------------------------------------------------------------------
template<>
class LOG_POLICY<LogMode::Contacts>
{
public:
        static void Execute(shared_ptr<spdlog::logger> logger, Person* p1, Person* p2,
                ClusterType cluster_type, shared_ptr<const WorldEnvironment> environ)
        {
                unsigned int home   = (cluster_type == ClusterType::Household);
                unsigned int work   = (cluster_type == ClusterType::Work);
                unsigned int school = (cluster_type == ClusterType::School);
                unsigned int other  = (cluster_type == ClusterType::HomeDistrict || cluster_type == ClusterType::DayDistrict);

                logger->info("[CONT] {} {} {} {} {} {} {} {}",
                        p1->GetId(), p1->GetAge(), p2->GetAge(), home, work, school, other, environ->GetSimulationDay());
        }
};

//--------------------------------------------------------------------------
// Declaration of partial specialization for LogMode::Contacts.
//--------------------------------------------------------------------------
template<bool track_index_case>
class Infector<LogMode::Contacts, track_index_case>
{
public:
        ///
        static void Execute(Cluster& cluster,
                std::shared_ptr<ContactHandler> contact_handler,
                std::shared_ptr<const WorldEnvironment> sim_state);
};

//--------------------------------------------------------------------------
// Definition for primary template covers the situation for
// LogMode::None & LogMode::Transmissions, both with
// track_index_case false and true..
//--------------------------------------------------------------------------
template<LogMode log_level, bool track_index_case>
void Infector<log_level, track_index_case>::Execute(Cluster& cluster,
                                shared_ptr<ContactHandler> contact_handler,
                                shared_ptr<const WorldEnvironment> sim_state)
{
        // check if the cluster has infected members and sort
        bool infectious_cases;
        size_t num_cases;
        tie(infectious_cases, num_cases) = cluster.SortMembers();

        if (infectious_cases) {
                cluster.UpdateMemberPresence();

                // set up some stuff
                auto logger            = spdlog::get("contact_logger");
                const auto c_type      = cluster.m_cluster_type;
                const auto c_immune    = cluster.m_index_immune;
                const auto& c_members  = cluster.m_members;
                const auto c_size      = cluster.GetSize();

                // match infectious in first part with susceptible in second part, skip last part (immune)
                for (size_t i_infected = 0; i_infected < num_cases; i_infected++) {
                        // check if member is present today
                        if (c_members[i_infected].second) {
                                const auto p1 = c_members[i_infected].first;
                                if (p1->GetHealth().IsInfectious()) {
                                        const auto age1 = p1->GetAge();
                                        for (size_t i_contact = num_cases; i_contact < c_immune; i_contact++) {
                                                // check if member is present today
                                                if (c_members[i_contact].second) {
                                                        auto p2 = c_members[i_contact].first;
                                                        if ((*contact_handler)(age1, c_type, c_size)) {
                                                                LOG_POLICY<log_level>::Execute(logger, p1, p2, c_type, sim_state);
                                                                p2->GetHealth().StartInfection();
                                                                R0_POLICY<track_index_case>::Execute(p2);
                                                        }
                                                }
                                        }
                                }
                        }
                }
        }
}

//--------------------------------------------------------------------------
// Definition of partial specialization for LogMode::Contacts.
//--------------------------------------------------------------------------
template<bool track_index_case>
void Infector<LogMode::Contacts, track_index_case>::Execute(Cluster& cluster,
                                shared_ptr<ContactHandler> contact_handler,
                                shared_ptr<const WorldEnvironment> sim_state)
{
        cluster.UpdateMemberPresence();

        // set up some stuff
        auto logger            = spdlog::get("contact_logger");
        const auto c_type      = cluster.m_cluster_type;
        const auto& c_members  = cluster.m_members;
        const auto c_size      = cluster.GetSize();

        // check all contacts
        for (size_t i_person1 = 0; i_person1 < cluster.m_members.size(); i_person1++) {
                // check if member participates in the social contact survey && member is present today
                if (c_members[i_person1].second && c_members[i_person1].first->IsParticipatingInSurvey()) {
                        auto p1 = c_members[i_person1].first;
                        const auto age1 = p1->GetAge();
                        for (size_t i_person2 = 0; i_person2 < c_members.size(); i_person2++) {
                                // check if member is present today
                                if ((i_person1 != i_person2) && c_members[i_person2].second) {
                                        auto p2 = c_members[i_person2].first;
                                        // check for contact
                                        if (contact_handler->Contact(age1, c_type, c_size)) {
                                                // TODO ContactHandler doesn't have a separate transmission function anymore to
                                                // check for transmission when contact has already been checked.
                                                // check for transmission
                                                /*bool transmission = contact_handler->transmission(age1, p2->GetAge());
                                                unsigned int infecter = 0;
                                                if (transmission) {
                                                        if (p1->IsInfectious() && p2->IsSusceptible()) {
                                                                infecter = 1;
                                                                p2->StartInfection();
                                                                R0_POLICY<track_index_case>::Execute(p2);
                                                        }
                                                        else if (p2->IsInfectious() && p1->IsSusceptible()) {
                                                                infecter = 2;
                                                                p1->StartInfection();
                                                                R0_POLICY<track_index_case>::Execute(p1);
                                                        }
                                                        //TODO log transmission?
                                                }*/
                                                LOG_POLICY<LogMode::Contacts>::Execute(logger, p1, p2, c_type, sim_state);

                                        }
                                }
                        }
                }
        }
}

//--------------------------------------------------------------------------
// All explicit instantiations.
//--------------------------------------------------------------------------
template class Infector<LogMode::None, false>;

template class Infector<LogMode::None, true>;

template class Infector<LogMode::Transmissions, false>;

template class Infector<LogMode::Transmissions, true>;

template class Infector<LogMode::Contacts, false>;

template class Infector<LogMode::Contacts, true>;

} // end_of_namespace