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
 * \file STL10MatrixImporter.cpp
 * \brief
 * \author tkornut, Alexis-Asseman
 * \date Dec 23, 2015
 */


#include <importers/STL10MatrixImporter.hpp>

#include <fstream>

namespace mic {
namespace importers {

STL10MatrixImporter::STL10MatrixImporter(std::string node_name_, std::string data_filename_, std::string labels_filename_) : Importer (node_name_),
    data_filename("data_filename",data_filename_),
    labels_filename("labels_filename", labels_filename_),
    samples_limit("samples_limit",-1)
{
    // Register properties - so their values can be overridden (read from the configuration file).
    registerProperty(data_filename);
    registerProperty(labels_filename);
    registerProperty(samples_limit);

    // Set image properties.
    image_width = 96;
    image_height = 96;
}

void STL10MatrixImporter::setDataFilename(std::string data_filename_) {
    data_filename = data_filename_;
}

void STL10MatrixImporter::setLabelsFilename(std::string labels_filename_) {
    labels_filename = labels_filename_;
}


bool STL10MatrixImporter::importData(){

    char buffer[96*96*3];
    size_t sample = 0;

    // RGB to grayascale conversion coefficents, using CCIR 601 norm
    float rgb_grayscale_coeffs[] = {0.299, 0.587, 0.114};

    // set true if providing labels
    labeled = labels_filename != "";

    std::ifstream labels_file(labels_filename, std::ios::in | std::ios::binary);
    if(labeled) {
        // Try to open file with labels.
        LOG(LSTATUS) << "Opening file containing STL-10 labels: " << labels_filename;
        if (!labels_file.is_open()) {
            LOG(LFATAL) << "Oops! Couldn't find file: " << labels_filename;
            return false;
        }//: else
    }

    // Read file containing images (binary format).
    LOG(LSTATUS) << "Opening file containing STL-10 images: " << data_filename;
    std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);
    if (!data_file.is_open()) {
        LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
        return false;
    }

    // Label and image files ok - import digits.
    LOG(LSTATUS) << "Importing STL-10 images. This might take a while...";


    // Import loop.
    while(true) {

        unsigned int temp_label;
        if(labeled) {
            // Try to read the label.
            labels_file.read(buffer, 1);
            // If reached the EOF.
            if (labels_file.eof())
                break;
            // Else: get the label.
            temp_label = (unsigned int)buffer[0];
        }
        else {
            temp_label = 0;
        }

        // Try to read the image into buffer.
        data_file.read(buffer, image_width*image_height*3);
        // If reached the EOF.
        if (data_file.eof())
            break;
        // Else: get image.

        // Create new matrix of STL-10 image size.
        mic::types::MatrixXfPtr image_ptr (new mic::types::MatrixXf(image_height, image_width));
        image_ptr->zeros();

        // Parse and set image data.
        for (size_t j = 0 ; j < 3 ; j++) {
            for (size_t i = 0 ; i < (size_t)(image_width*image_height) ; i++) {
                unsigned col = i / image_width;
                unsigned row = i % image_height;
                //ch(row, col) = (uint8_t)buffer[i];
                (*image_ptr)(row, col) += rgb_grayscale_coeffs[j] * (float)((uint8_t)buffer[i + ((image_height*image_width)*j)])/255.0f;
            }//: for
        }

        // Got the image and label.
        LOG(LDEBUG) << "Loading STL-10 sample: " << sample;

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




} /* namespace importers */
} /* namespace mic */
