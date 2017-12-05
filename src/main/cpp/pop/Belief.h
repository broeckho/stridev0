#pragma once
/*
 * Belief.h
 *
 *  Created on: 05 Dec 2017
 *      Author: broeckho
 */

#include <boost/property_tree/ptree.hpp>

class Belief
{
public:
        Belief(const boost::property_tree::ptree& pt) : m_pt(pt) {}
        boost::property_tree::ptree Get() {return m_pt;}
        void Set(const boost::property_tree::ptree& pt) { m_pt = pt;}
private:
        boost::property_tree::ptree  m_pt;
};


