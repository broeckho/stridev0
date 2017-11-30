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
namespace stride {

enum class VariableState
{
	Low = 0U,
	High = 1U
};

class HBMData
{
public:
	HBMData()
	    : m_perceived_severity(0), m_perceived_susceptibility(0), m_perceived_benefit(1), m_perceived_barriers(1)
	{
	}

	VariableState GetPerceivedSeverity() const { return m_perceived_severity; }

	VariableState GetPerceivedSusceptibility() const { return m_perceived_susceptibility; }

	VariableState GetPerceivedBenefit() const { return m_perceived_benefit; }

	VariableState GetPerceivedBarriers() const { return m_perceived_barriers; }

	void MeetAdopted()
	{
		m_num_contacts_adopted++;
		m_num_contacts++;
	}

	void MeetInfected()
	{
		m_num_contacts_infected++;
		m_num_contacts++;
	}

	void MeetInfectedAndAdopted()
	{
		m_num_contacts_adopted++;
		m_num_contacts_infected++;
		m_num_contacts++;
	}

private:
	int m_num_contacts;
	int m_num_contacts_infected;
	int m_num_contacts_adopted;
	VariableState m_perceived_severity;
	VariableState m_perceived_susceptibility;
	VariableState m_perceived_benefit;
	VariableState m_perceived_barriers;
};

} // end of namespace
