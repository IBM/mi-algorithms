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
 * \file MNISTMatrixImporter.hpp
 * \brief 
 * \author tkornut
 * \date Dec 23, 2015
 */


#ifndef SRC_importers_MNISTMATRIXIMPORTER_HPP_
#define SRC_importers_MNISTMATRIXIMPORTER_HPP_

#include <importers/Importer.hpp>
#include <types/MNISTTypes.hpp>
#include <fstream>

namespace mic {
namespace importers {

template<typename T=float>
class MNISTMatrixImporter: public mic::importers::Importer< mic::types::Matrix<T>, unsigned int > {
public:
	/*!
	 * Constructor. Sets MNIST image default properties. Registers properties.
	 * @param node_name_ Name of the node in configuration file.
	 * @param data_filename_ File (with path) containing MNIST images.
	 * @param labels_filename_ File (with path) containing MNIST labels.
	 */
    MNISTMatrixImporter(std::string node_name_ = "mnist_matrix_importer", std::string data_filename_ = "", std::string labels_filename_ = "") :
        Importer< mic::types::Matrix<T>, unsigned int > (node_name_),
        data_filename("data_filename", data_filename_),
        labels_filename("labels_filename", labels_filename_),
        samples_limit("samples_limit", -1)
    {
        // Register properties - so their values can be overridden (read from the configuration file).
        this->registerProperty(data_filename);
        this->registerProperty(labels_filename);
        this->registerProperty(samples_limit);

        // Set image properties.
        image_width = 28;
        image_height = 28;
    }
	/*!
	 * Virtual destructor. Empty.
	 */
    virtual ~MNISTMatrixImporter() {}

	/*!
	 * Set name and patch of the file containing MNIST images.
	 * @param data_filename_ File (with path) containing MNIST images.
	 */
    void setDataFilename(std::string data_filename_) {
        data_filename = data_filename_;
    }

	/*!
	 * Set name and patch of the file containing MNIST labels.
	 * @param labels_filename_ File (with path) containing MNIST labels.
	 */
    void setLabelsFilename(std::string labels_filename_){
        labels_filename = labels_filename_;
    }

	/*!
	 * Method responsible for importing/loading the MNIST dataset.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
    bool importData(){

        char buffer[28*28];
        int label_offset_bytes = 8;
        int data_offset_bytes = 16;
        size_t sample = 0;

        // Try to open file with labels.
        LOG(LSTATUS) << "Opening file containing MNIST labels: " << labels_filename;
        std::ifstream labels_file(labels_filename, std::ios::in | std::ios::binary);
        if (!labels_file.is_open()) {
            LOG(LFATAL) << "Oops! Couldn't find file: " << labels_filename;
            return false;
        }//: else

        // Read file containing images (binary format).
        LOG(LSTATUS) << "Opening file containing MNIST images: " << data_filename;
        std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);
        if (!data_file.is_open()) {
            LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
            return false;
        }

        // Label and image files ok - import digits.
        LOG(LSTATUS) << "Importing MNIST digits. This might take a while...";

        // Skip label header.
        labels_file.seekg (label_offset_bytes, std::ios::beg);
        // Skip data header.
        data_file.seekg (data_offset_bytes , std::ios::beg);

        // Import loop.
        while(true) {
            // Try to read the label.
            labels_file.read(buffer, 1);
            // If reached the EOF.
            if (labels_file.eof())
                break;
            // Else: get the label.
            unsigned int temp_label = (unsigned int)buffer[0];

            // Try to read the image into buffer.
            data_file.read(buffer, image_width*image_height);
            // If reached the EOF.
            if (data_file.eof())
                break;
            // Else: get image.

            // Create new matrix of MNIST image size.
            mic::types::MatrixPtr<T> image_ptr (new mic::types::Matrix<T>(image_height, image_width));

            // Parse and set image data.
            for (size_t i = 0; i < (size_t)(image_width*image_height); i++) {
                unsigned row = i / image_width;
                unsigned col = i % image_height;
                (*image_ptr)(row, col) = (T)((uint8_t)buffer[i])/(T)255.0f;
            }//: for

            // Got the image and label.
            LOG(LDEBUG) << "Loading MNIST sample: " << sample;

            sample_data.push_back(image_ptr);
            sample_labels.push_back(std::make_shared <unsigned int> (temp_label) );

            sample++;
            // Check limit.
            if ((samples_limit > 0) && (sample >= (size_t)samples_limit))
                break;
        }//: while !eof

        LOG(LINFO) << "Imported " << sample_labels.size() << " patches";

        // Close files
        labels_file.close();
        data_file.close();

        // Fill the indices table(!)
        for (size_t i=0; i < sample_data.size(); i++ )
            sample_indices.push_back(i);

        // Count the classes.
        //countClasses();
        number_of_classes = 10;

        LOG(LINFO) << "Data import finished";
        return true;
    }

	/*!
	 * Method responsible for initialization of all variables that are property-dependent - here not required, yet empty.
	 */
    virtual void initializePropertyDependentVariables() {}

private:
	/*!
	 * Width of MNIST image.
	 */
	int image_width;

	/*!
	 * Height of MNIST image.
	 */
	int image_height;

	/*!
	 * Property: directory/Name of file containing images (binary datafile).
	 */
	mic::configuration::Property<std::string> data_filename;

	/*!
	 * Property: directory/Name of file containing labels.
	 */
	mic::configuration::Property<std::string> labels_filename;

	/*!
	 * Property: maximum number of samples (limitation, from 1 to 60000). If <=0 then there is no limitation.
	 */
	mic::configuration::Property<int> samples_limit;

    using Importer< mic::types::Matrix<T>, unsigned int >::sample_data;
    using Importer< mic::types::Matrix<T>, unsigned int >::sample_labels;
    using Importer< mic::types::Matrix<T>, unsigned int >::number_of_classes;
    using Importer< mic::types::Matrix<T>, unsigned int >::sample_indices;
};


} /* namespace importers */
} /* namespace mic */


#endif /* SRC_importers_MNISTMATRIXIMPORTER_HPP_ */
