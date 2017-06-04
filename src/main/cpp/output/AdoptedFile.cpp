/*
 * AdoptedFile.cpp
 *
 *  Created on: Jun 4, 2017
 *      Author: elise
 */

#include "AdoptedFile.h"

#include <iostream>

namespace stride {
namespace output {

AdoptedFile::AdoptedFile(const std::string& file) {
	Initialize(file);
}

AdoptedFile::~AdoptedFile() {
	m_fstream.close();
}

void AdoptedFile::Print(const std::vector<unsigned int>& adopted) {
	for (unsigned int i = 0; i < (adopted.size() - 1); i++) {
		m_fstream << adopted[i] << ",";
	}
	m_fstream << adopted[adopted.size() - 1] << std::endl;
}

void AdoptedFile::Initialize(const std::string& file) {
	m_fstream.open((file + "_adopted.csv").c_str());
}



} /* namespace output */
} /* namespace stride */
