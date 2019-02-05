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
 * @file: BMPImporter.hpp
 * @Author: Tomasz Kornuta <tkornut@us.ibm.com>
 * @Date:   Jun 14, 2017
 *
 * Copyright (c) 2017, Tomasz Kornuta, IBM Corporation. All rights reserved.
 *
 */

#ifndef BMPIMPORTER_HPP_
#define BMPIMPORTER_HPP_

#include <importers/Importer.hpp>
#include <types/TensorTypes.hpp>

namespace mic {
namespace importers {

/*!
 * \brief Class responsible for importing CIFAR images.
 * Returns a batch of Tensors.
 * Assumes that every image can have different dimensions and belongs to a separate class.
 * \author tkornuta
 */
template <typename eT>
class BMPImporter: public mic::importers::Importer< mic::types::Tensor<eT>, unsigned int > {
public:
	/*!
	 * Constructor. Sets default properties. Registers properties.
	 * @param node_name_ Name of the node in configuration file.
	 * @param data_filename_ File(s) (with path) containing images. If data is supposed to be loaded from more than one files, they should be separated by a semicolon (;).
	 */
	BMPImporter(std::string node_name_ = "bmp_importer", std::string data_filename_ = "")
		: Importer< mic::types::Tensor<eT>, unsigned int >::Importer (node_name_),
			data_filename("data_filename",data_filename_)
	{
		// Register properties - so their values can be overridden (read from the configuration file).
		registerProperty(data_filename);

	}



	/*!
	 * Virtual destructor. Empty.
	 */
	virtual ~BMPImporter() { };

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

	    // Read data from files.
	    for (size_t fi = 0; fi < names_array.size(); ++fi) {

    		// Try to open file with labels.
    		LOG(LSTATUS) << "Opening file: " << names_array[fi];
    		std::ifstream file(names_array[fi], std::ios::in | std::ios::binary);
    		if (!file.is_open()) {
    			LOG(LFATAL) << "Oops! Couldn't find file: " << names_array[fi];
    			return false;
    		}//: else


			// Got the image and label.
			LOG(LDEBUG) << "Loading sample: " << names_array[fi];
    		mic::types::TensorPtr<float> ptr = loadBMP(file);

    		// Add sample to batch.
			sample_data.push_back(ptr);

			// Close file
	    	file.close();

		}//: for files.

    	LOG(LINFO) << "Imported " << sample_data.size() << " samples";


		// Fill labels and indices tables(!)
		for (size_t i=0; i < sample_data.size(); i++ ){
			sample_labels.push_back(std::make_shared <unsigned int> (i) );
			sample_indices.push_back(i);
		}

		// Count the classes.
		number_of_classes = sample_data.size();

		LOG(LINFO) << "Data import finished";

		return true;
	}

	mic::types::TensorPtr<float> loadBMP(std::ifstream& file_)
	{
		   static constexpr size_t HEADER_SIZE = 54;

		    // Read the header.
		    std::array<char, HEADER_SIZE> header;
		    file_.read(header.data(), header.size());

		    // Get parameters.
		    // https://www.gamedev.net/resources/_/technical/game-programming/how-to-load-a-bitmap-r1966
		    // From File information header.
		    uint32_t bfSize = *reinterpret_cast<uint32_t *>(&header[2]);
		    uint32_t bfOffBits = *reinterpret_cast<uint32_t *>(&header[10]);
		    // From Bitmap information header.
		    uint32_t biWidth = *reinterpret_cast<uint32_t *>(&header[18]);
		    uint32_t biHeight = *reinterpret_cast<uint32_t *>(&header[22]);
		    // Number of bits per pixel for colors: 1,4,8 or 24.
		    uint16_t biBitCount = *reinterpret_cast<uint16_t *>(&header[28]);

		    LOG(LDEBUG) << "fileSize: " << bfSize;
		    LOG(LDEBUG) << "dataOffset: " << bfOffBits;
		    LOG(LDEBUG) << "width: " << biWidth;
		    LOG(LDEBUG) << "height: " << biHeight;
		    LOG(LDEBUG) << "depth: " << biBitCount << "-bit";

		    // Set values depending on BitCount.
		    // Number of image channels (3 or 4).
		    size_t img_channels = biBitCount/8;
		    size_t roff, goff, boff;
		    if (biBitCount == 24) {
		    	roff = 2;
		    	goff = 1;
		    	boff = 0;
		    } else if (biBitCount == 32) {
		    	roff = 3;
		    	goff = 2;
		    	boff = 1;
		    } else {
		    	LOG(LERROR) << "Unhandel number of bits per pixel: " << biBitCount << "-bit";
		    }

		    // Skip the rest of header.
		    std::vector<char> rest(bfOffBits - HEADER_SIZE);
		    file_.read(rest.data(), rest.size());

		    // Read the "pixels".
		    std::vector<char> img(bfSize - bfOffBits);
		    file_.read(img.data(), img.size());

		    // padWidth is the width of the image plus the extra padding.
		    // Initially set both to the width of the image.
		    size_t padWidth= (size_t)(img_channels*biWidth);

		    // And add any extra space to bring each line to a DWORD boundary
		    while(padWidth%4!=0) {
		       padWidth++;
		    }
		    LOG(LDEBUG) << "padWidth: " << padWidth;

		    // Prepare output tensoroff, goff, boffr.
		    mic::types::TensorPtr<float> ptr = MAKE_TENSOR_PTR(float, biHeight, biWidth, 3 );
		    ptr->zeros();
		    // Get data.
		    float* data_ptr = ptr->data();

		    // Iterate through rows...
		    for (size_t h =0; h < biHeight; h++){
		    	// ... and cols.
		        for (size_t w =0; w < padWidth; w+=img_channels) {
		        	 // Skip the padding.
		        	 if (w >= img_channels*biWidth) {
		        		 continue;
		        	 }
		        	size_t i = h*padWidth + w;
		 	    	/*std::cout << "h = " << h << "w = " << w << "i = " << i ;
		 	        std::cout << " R: " << int(img[i+2] & 0xff) << " G: " << int(img[i+1] & 0xff) << " B: " << int(img[i] & 0xff)
		 	        	<< std::endl;*/

		        	// Red
		           	//(*ptr)({biHeight-1 - h,w/3,0}) = (int(img[i + 2] & 0xff)) / (255.0);
		        	data_ptr[(biHeight-1 - h)*biWidth + w/img_channels] = (int(img[i + roff] & 0xff)) / (255.0);
		           	// Green
		           	//(*ptr)({biHeight-1 - h,w/3,1}) = (int(img[i + 1] & 0xff)) / (255.0);
		        	data_ptr[(biHeight-1 - h)*biWidth + w/img_channels + 1*biWidth*biHeight] = (int(img[i + goff] & 0xff)) / (255.0);
		           	// Blue
		           	//(*ptr)({biHeight-1 - h,w/3,2}) = (int(img[i] & 0xff)) / (255.0);
		        	data_ptr[(biHeight-1 - h)*biWidth + w/img_channels + 2*biWidth*biHeight] = (int(img[i + boff] & 0xff)) / (255.0);

		        }
		    }

	    //std::cout << " ptr = " << (*ptr) << std::endl;
	    return ptr;
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

private:

	/*!
	 * Property: directory/Name of file containing images (or a list of files separated by a semicolon).
	 */
	mic::configuration::Property<std::string> data_filename;

};


} /* namespace importers */
} /* namespace mic */





#endif /* BMPIMPORTER_HPP_ */
