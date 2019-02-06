/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*!
 * \file IBMFontMatrixImporter.cpp
 * \brief Contains definition of methods of importer responsible for importing/loading characters from IBM VGA font dataset.
 * \author tkornut
 * \date Dec 22, 2015
 */

#include <importers/IBMFontMatrixImporter.hpp>

#include <importers/IBMFonts.hpp>

namespace mic {
namespace importers {

IBMFontMatrixImporter::IBMFontMatrixImporter(std::string node_name_) : Importer (node_name_),
		font_type("font_type",font16x16_type)
{
	// Register properties - so their values can be overridden (read from the configuration file).
	registerProperty(font_type);
}

bool IBMFontMatrixImporter::importData(){

	LOG(LSTATUS) << "Importing IBM VGA fonts of size " << ( font_type == font8x8_type ? "8x8" : "16x16");

	// Select adequate font table.
//	const uint8_t* font_table = ( font_type == font8x8_type ? font8x8_table : font16x16_table );

//	const uint16_t*  mask = ( font_type == font8x8_type ? mask8x8 : mask16x16 );

	// "Load font dataset with labels.
	for(size_t i=1; i < 94; i++ ) {

		// Create new matrix of size font_type x font_type (8x8 v 16x16).
		mic::types::MatrixXfPtr mat (new mic::types::MatrixXf((int)font_type, (int)font_type));

		if (font_type == font8x8_type) {
			// Generate 8x8 characters - font_type = 8.

			for (size_t r = 0; r < 8; r++) {
				// Get line.
				uint8_t line = font8x8_table[8*i+r];
				// Iterate through characters in given line.
				for (size_t c = 0; c < 8; c++) {
					(*mat)(r, c) = ((line & mask8x8[8-c]) != 0);
				}//: for
			}//: for
		} else {
			// Generate 16x16 characters - font_type = 16.

			for (size_t r = 0; r < 16; r++) {
		        uint16_t line = ((uint16_t*)font16x16_table)[16 *i+r];
		        for (size_t c = 0; c < 16; c++) {
		            (*mat)(r, c) = ((line & mask16x16[c]) != 0);
		        }//: for
		    }//: for

	    }//: else

		// Add character and label to vectors.
		sample_data.push_back(mat);
		sample_labels.push_back(std::make_shared <char> (font_labels_table[i]) );

	}//: for
	LOG(LINFO) << "Imported " << sample_data.size() << " fonts";

	// Fill the indices table(!)
	for (size_t i=0; i < sample_data.size(); i++ )
		sample_indices.push_back(i);

	// Count the classes.
	countClasses();

	LOG(LINFO) << "Data import finished";

	return true;
}


} /* namespace importers */
} /* namespace mic */
