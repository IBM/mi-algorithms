/*!
 * \file RawTextImporter.cpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#include <data_io/RawTextImporter.hpp>

#include <fstream>

namespace mic {
namespace data_io {

RawTextImporter::RawTextImporter(std::string node_name_) : Importer (node_name_),
		data_filename("data_filename","data_filename")
{
	// Register properties - so their values can be overridden (read from the configuration file).
	registerProperty(data_filename);
}

void RawTextImporter::setDataFilename(std::string data_filename_) {
	data_filename = data_filename_;
}


bool RawTextImporter::importData(){
	char character;
	// Open file.
	std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);

	LOG(LSTATUS) << "Importing raw data from file: " << data_filename;

	// Check if file is open.
	if (data_file.is_open()) {
		// Iterate through the characters.
		while(!data_file.eof()) {
			// Read the character.
			data_file.get(character);
			LOG(LDEBUG) << character;

			// Add character to both data and labels.
			sample_data.push_back(std::make_shared <char> (character) );
			sample_labels.push_back(std::make_shared <char> (character) );

		}//: while ! eof

		// Close the file.
		data_file.close();

	} else {
		LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
		return false;
	}//: else

	LOG(LINFO) << "Imported " << sample_data.size() << " characters";

	// Fill the indices table(!)
	for (size_t i=0; i < sample_data.size(); i++ )
		sample_indices.push_back(i);

	LOG(LINFO) << "Data import finished";

	return true;
}

} /* namespace data_io */
} /* namespace mic */
