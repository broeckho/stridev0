#ifndef INFECTOR_H_INCLUDED
#define INFECTOR_H_INCLUDED
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
 * Header for the Infector class.
 */

#include "behaviour/information_policies/LocalDiscussion.h"
#include "behaviour/information_policies/NoLocalInformation.h"
#include "calendar/Calendar.h"
#include "core/DiseaseProfile.h"
#include "core/LogMode.h"
#include "core/RngHandler.h"
#include "pop/Person.h"

#include <memory>

namespace stride {

template <typename person_type>
class Cluster;

/**
 * Actual contacts and transmission in cluster (primary template).
 */
template <LogMode log_level, bool track_index_case, typename local_information_policy, typename person_type>
class Infector
{
public:
	///
	static void Execute(Cluster<person_type>& cluster, DiseaseProfile disease_profile, RngHandler& contact_handler,
			std::shared_ptr<const Calendar> calendar);
};

/**
 * Actual contacts and transmissions in cluster (specialization for NoLocalInformation policy)
 */
template <LogMode log_level, bool track_index_case, typename person_type>
class Infector<log_level, track_index_case, NoLocalInformation, person_type>
{
public:
	///
	static void Execute(Cluster<person_type>& cluster, DiseaseProfile disease_profile, RngHandler& contact_handler,
				std::shared_ptr<const Calendar> calendar);
};

/**
 * Actual contacts and transmission in cluster (specialization for logging all contacts, and with NoLocalInformation
 * policy).
 */
template <bool track_index_case, typename person_type>
class Infector<LogMode::Contacts, track_index_case, NoLocalInformation, person_type>
{
public:
	///
	static void Execute(Cluster<person_type>& cluster, DiseaseProfile disease_profile, RngHandler& contact_handler,
			    std::shared_ptr<const Calendar> calendar);
};

typedef Person<NoBehaviour<NoBelief>, NoBelief> PersonNoBehaviourNoBelief;


/// Explicit instantiations in cpp file.
extern template class Infector<LogMode::None, false, NoLocalInformation, PersonNoBehaviourNoBelief>;
extern template class Infector<LogMode::None, false, LocalDiscussion<PersonNoBehaviourNoBelief>, PersonNoBehaviourNoBelief>;

extern template class Infector<LogMode::None, true, NoLocalInformation, PersonNoBehaviourNoBelief>;
extern template class Infector<LogMode::None, true, LocalDiscussion<PersonNoBehaviourNoBelief>, PersonNoBehaviourNoBelief>;

extern template class Infector<LogMode::Transmissions, false, NoLocalInformation, PersonNoBehaviourNoBelief>;
extern template class Infector<LogMode::Transmissions, false, LocalDiscussion<PersonNoBehaviourNoBelief>, PersonNoBehaviourNoBelief>;

extern template class Infector<LogMode::Transmissions, true, NoLocalInformation, PersonNoBehaviourNoBelief>;
extern template class Infector<LogMode::Transmissions, true, LocalDiscussion<PersonNoBehaviourNoBelief>, PersonNoBehaviourNoBelief>;

extern template class Infector<LogMode::Contacts, false, NoLocalInformation, PersonNoBehaviourNoBelief>;
extern template class Infector<LogMode::Contacts, false, LocalDiscussion<PersonNoBehaviourNoBelief>, PersonNoBehaviourNoBelief>;

extern template class Infector<LogMode::Contacts, true, NoLocalInformation, PersonNoBehaviourNoBelief>;
extern template class Infector<LogMode::Contacts, true, LocalDiscussion<PersonNoBehaviourNoBelief>, PersonNoBehaviourNoBelief>;

} // end_of_namespace

#endif // include-guard
