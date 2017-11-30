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

#include <trng/mrg2.hpp>
#include <trng/uniform01_dist.hpp>

namespace stride {

class RNG
{
private:
	RNG() { m_uniform_dist = trng::uniform01_dist<double>(); }
	RNG(const RNG&);
	RNG& operator=(const RNG&);

public:
	static RNG& GetInstance()
	{
		static RNG instance;
		return instance;
	}

	double NextDouble() { return m_uniform_dist(m_engine); }

private:
	trng::mrg2 m_engine;                         ///< The random number engine.
	trng::uniform01_dist<double> m_uniform_dist; ///< The random distribution.
};

} // end of namespace

