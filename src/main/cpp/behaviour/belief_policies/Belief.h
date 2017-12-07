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
 * Header file for Belief.
 */

#include <boost/property_tree/ptree.hpp>

namespace stride {

/**
 * Base class for all belief policies.
 */
class Belief
{
public:
	///
    Belief(const boost::property_tree::ptree& pt) : m_pt(pt) {}

    ///
    boost::property_tree::ptree Get() {return m_pt;}

    ///
    void Set(const boost::property_tree::ptree& pt) { m_pt = pt;}

    bool HasAdopted() const { return false; } //TODO make virtual to override

private:
    boost::property_tree::ptree  m_pt;			///<
};

} // end_of_namespace

