/*!
 * \file MNISTMatrixImporter.cpp
 * \brief 
 * \author tkornut
 * \date Dec 23, 2015
 */


#include <data_io/MNISTMatrixImporter.hpp>

#include <fstream>

namespace mic {
namespace data_io {

MNISTMatrixImporter::MNISTMatrixImporter(std::string node_name_, std::string data_filename_, std::string labels_filename_) : Importer (node_name_),
		data_filename("data_filename",data_filename_),
		labels_filename("labels_filename", labels_filename_),
		samples_limit("samples_limit",-1)
{
	// Register properties - so their values can be overridden (read from the configuration file).
	registerProperty(data_filename);
	registerProperty(labels_filename);
	registerProperty(samples_limit);

	// Set image properties.
	image_width = 28;
	image_height = 28;
}

void MNISTMatrixImporter::setDataFilename(std::string data_filename_) {
	data_filename = data_filename_;
}

void MNISTMatrixImporter::setLabelsFilename(std::string labels_filename_) {
	labels_filename = labels_filename_;
}


bool MNISTMatrixImporter::importData(){

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
		mic::types::MatrixXfPtr image_ptr (new mic::types::MatrixXf(image_height, image_width));

		// Parse and set image data.
		for (size_t i = 0; i < (size_t)(image_width*image_height); i++) {
			unsigned row = i / image_width;
			unsigned col = i % image_height;
			(*image_ptr)(row, col) = (float)((uint8_t)buffer[i])/255.0f;
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




} /* namespace data_io */
} /* namespace mic */
