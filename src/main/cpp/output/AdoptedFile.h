/*
 * AdoptedFile.h
 *
 *  Created on: Jun 4, 2017
 *      Author: elise
 */

#ifndef SRC_MAIN_CPP_OUTPUT_ADOPTEDFILE_H_
#define SRC_MAIN_CPP_OUTPUT_ADOPTEDFILE_H_

#include <fstream>
#include <string>
#include <vector>

namespace stride {
namespace output {

class AdoptedFile {
public:
	/// Constructor
	AdoptedFile(const std::string& file = "stride_adopted");

	/// Destructor
	~AdoptedFile();

	/// Print the given cases with corresponding tag.
	void Print(const std::vector<unsigned int>& cases);
private:
	/// Generate file name and open the file stream.
	void Initialize(const std::string& file);

private:
	std::ofstream		m_fstream;	///< The file stream.
};

} /* namespace output */
} /* namespace stride */

#endif /* SRC_MAIN_CPP_OUTPUT_ADOPTEDFILE_H_ */
