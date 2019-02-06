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
 * @file: CIFARImporter.hpp
 * @Author: Tomasz Kornuta <tkornut@us.ibm.com>
 * @Date:   Jun 12, 2017
 *
 * Copyright (c) 2017, Tomasz Kornuta, IBM Corporation. All rights reserved.
 *
 */

#ifndef CIFARIMPORTER_HPP_
#define CIFARIMPORTER_HPP_


#include <importers/Importer.hpp>
#include <types/TensorTypes.hpp>

namespace mic {
namespace importers {

/*!
 * \brief Class responsible for importing CIFAR images.
 * Returns a batch of Tensors.
 * \author tkornuta
 */
template <typename eT>
class CIFARImporter: public mic::importers::Importer< mic::types::Tensor<eT>, unsigned int > {
public:
	/*!
	 * Constructor. Sets CIFAR image default properties. Registers properties.
	 * @param node_name_ Name of the node in configuration file.
	 * @param data_filename_ File(s) (with path) containing images. If data is supposed to be loaded from more than one files, they should be separated by a semicolon (;).
	 * @param min_sample_ Index of first sample to be added to batch (samples < min_sample_ will be skipped) (DEFAULT=-1 - all samples).
	 * @param max_sample_ Index of last sample to be added to batch (samples > max_sample_ will be skipped) (DEFAULT=-1 - all samples).
	 */
	CIFARImporter(std::string node_name_ = "cifar_importer", std::string data_filename_ = "", int min_sample_ = -1, int max_sample_ = -1)
		: Importer< mic::types::Tensor<eT>, unsigned int >::Importer (node_name_),
			data_filename("data_filename",data_filename_),
			min_sample("min_sample", min_sample_),
			max_sample("max_sample", max_sample_)
	{
		// Register properties - so their values can be overridden (read from the configuration file).
		registerProperty(data_filename);
		registerProperty(min_sample);
		registerProperty(max_sample);

		// Set image properties.
		image_height = 32;
		image_width = 32;
		image_depth = 3;
	}



	/*!
	 * Virtual destructor. Empty.
	 */
	virtual ~CIFARImporter() { };

	/*!
	 * Set name and patch of the file/files containing CIFAR images.
	 * @param data_filename_ File (with path) containing CIFAR images. If data is supposed to be loaded from more than one files, they should be separated by a semicolon (;).
	 */
	void setDataFilename(std::string data_filename_) {
		data_filename = data_filename_;
	}

	/*!
	 * Method responsible for importing/loading the CIFAR dataset.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	bool importData() {
		// Split filename using a semicolon (;) separator.
	    std::vector<std::string> names_array;
	    std::size_t pos = 0, found;
	    while((found = std::string(data_filename).find_first_of(';', pos)) != std::string::npos) {
	    	// Get filename from pos to found.
	    	names_array.push_back(std::string(data_filename).substr(pos, found - pos));
	    	// Move marker.
	        pos = found+1;
	    }//: white
	    // Add last name.
	    names_array.push_back(std::string(data_filename).substr(pos));

	    // Buffer.
		char buffer[image_height*image_width*image_depth];

	    // Read data from files.
	    for (size_t fi = 0; fi < names_array.size(); ++fi) {

    		// Try to open file with labels.
    		LOG(LSTATUS) << "Opening file containing CIFAR file: " << names_array[fi];
    		std::ifstream cifar_file(names_array[fi], std::ios::in | std::ios::binary);
    		if (!cifar_file.is_open()) {
    			LOG(LFATAL) << "Oops! Couldn't find file: " << names_array[fi];
    			return false;
    		}//: else

    		size_t sample = 0;

	    	// Import data from file.
	    	while(true) {

				// <1 x label><3072 x pixel>
				// * the first byte is the label of the first image, which is a number in the range 0-9.
				// The next 3072 bytes are the values of the pixels of the image.
				// The first 1024 bytes are the red channel values, the next 1024 the green, and the final 1024 the blue.
				// The values are stored in row-major order, so the first 32 bytes are the red channel values of the first row of the image.

	    		// Try to read the label.
	    		cifar_file.read(buffer, 1);
	    		// If reached the EOF.
	    		if (cifar_file.eof())
	    			break;
	    		// Else: get the label.
	    		unsigned int temp_label = (unsigned int)buffer[0];

	    		// Got the image and label.
	    		LOG(LDEBUG) << "Loading sample: " << sample;

	    		// Try to read the image into buffer.
	    		cifar_file.read(buffer, image_width*image_height*image_depth);
	    		// If reached the EOF.
	    		if (cifar_file.eof())
	    			break;
	    		// Else: get image.

	    		// Ok, the sample was loaded. Now check whether we should add it to image.
	    		sample++;
	    		if ((min_sample > 0) && (sample < (size_t)min_sample))
	    			continue;

	    		// Create new tensor of CIFAR image size.
	    		mic::types::TensorPtr<eT> ptr = MAKE_TENSOR_PTR(eT, image_height, image_width, image_depth);
	    		eT* data = ptr->data();

	    		// Copy image.
				for (size_t id = 0; id < image_depth; ++id) {
					for (size_t ih = 0; ih < image_height; ++ih) {
						for (size_t iw = 0; iw < image_width; ++iw) {
							// Calculate indices.
							// BI - row-major.
							size_t bi = id*image_height*image_width + ih*image_width + iw;
							// DI - col-major.
							//size_t di = id*image_height*image_width + ih + iw*image_height;

							// Copy value.
			    			data[bi] = (eT)((uint8_t)buffer[bi])/255.0f;
						}
					}//: width
				}//: depth
	    		/*for (size_t i = 0; i < (size_t)(image_width*image_height*image_depth); i++) {
	    			data[i] = (eT)((uint8_t)buffer[i])/255.0f;
	    		}//: for*/

	    		sample_data.push_back(ptr);
	    		sample_labels.push_back(std::make_shared <unsigned int> (temp_label) );

	    		// Check limit.
	    		if ((max_sample > 0) && (sample >= (size_t)max_sample))
	    			break;
	    	}//: while !eof

	    	LOG(LINFO) << "Imported " << sample_data.size() << " samples";

			// Close files
			cifar_file.close();

		}//: for files.

		// Fill the indices table(!)
		for (size_t i=0; i < sample_data.size(); i++ )
			sample_indices.push_back(i);

		// Count (and set) number of classes.
		countClasses();

		LOG(LINFO) << "Data import finished";
		return true;
	}

	/*!
	 * Method responsible for initialization of all variables that are property-dependent - here not required, yet empty.
	 */
	virtual void initializePropertyDependentVariables() { };

protected:
	// Unhide the fields inherited from the template class Layer via "using" statement.
    using Importer< mic::types::Tensor<eT>, unsigned int >::registerProperty;
    using Importer< mic::types::Tensor<eT>, unsigned int >::sample_data;
    using Importer< mic::types::Tensor<eT>, unsigned int >::sample_labels;
    using Importer< mic::types::Tensor<eT>, unsigned int >::sample_indices;
    using Importer< mic::types::Tensor<eT>, unsigned int >::number_of_classes;
    using Importer< mic::types::Tensor<eT>, unsigned int >::countClasses;

private:
	/*!
	 * Height of CIFAR image.
	 */
    size_t image_height;

	/*!
	 * Width of CIFAR image.
	 */
    size_t image_width;

	/*!
	 * Depth of CIFAR image (number of channels).
	 */
    size_t image_depth;


	/*!
	 * Property: directory/Name of file containing images (binary files).
	 */
	mic::configuration::Property<std::string> data_filename;

	/*!
	 * Property: index of the first sample to be added to batch (samples < min_sample_ will be skipped) (DEFAULT=-1 - all samples).
	 */
	mic::configuration::Property<int> min_sample;

	/*!
	 * Property: index of the last sample to be added to batch (samples > max_sample_ will be skipped) (DEFAULT=-1 - all samples).
	 */
	mic::configuration::Property<int> max_sample;

};


} /* namespace importers */
} /* namespace mic */



#endif /* CIFARIMPORTER_HPP_ */
