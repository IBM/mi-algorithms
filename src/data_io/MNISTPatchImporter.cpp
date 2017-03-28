/*!
 * \file MNISTPatchImporter.cpp
 * \brief 
 * \author tkornut
 * \date Mar 27, 2017
 */

#include <data_io/MNISTPatchImporter.hpp>
#include <logger/Log.hpp>


#include <fstream>

namespace mic {
namespace data_io {

MNISTPatchImporter::MNISTPatchImporter(std::string node_name_) : Importer (node_name_),
		data_filename("data_filename","images-idx3-ubyte"),
		labels_filename("labels_filename", "labels-idx1-ubyte"),
		patch_size("patch_size", 5),
		samples_limit("samples_limit",-1)
{
	// Register properties - so their values can be overridden (read from the configuration file).
	registerProperty(data_filename);
	registerProperty(labels_filename);
	registerProperty(patch_size);
	registerProperty(samples_limit);
	// Set image properties.
	image_width = 28;
	image_height = 28;
}

bool MNISTPatchImporter::importData(){

	char buffer[28*28];
	int label_offset_bytes = 8;
	int data_offset_bytes = 16;
	size_t sample = 0;

	// Limit patch size.
	if (patch_size < 1)
		patch_size = 1;
	else if (patch_size >= 28)
		patch_size = 28;

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

	// Label and image files ok - import patches.
	LOG(LSTATUS) << "Importing MNIST patches of size " << patch_size << " by " << patch_size << ". This might take a while...";

	// Skip label header.
	labels_file.seekg (label_offset_bytes, std::ios::beg);
	// Skip header.
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
		mic::types::MatrixXf image(image_height, image_width);

		// Parse and set image data.
		for (size_t i = 0; i < (size_t)(image_width*image_height); i++) {
			unsigned row = i / image_width;
			unsigned col = i % image_height;
			image(row, col) = (float)((uint8_t)buffer[i])/255.0f;
		}//: for

		// Got the image and label - now add the patches...
		LOG(LDEBUG) << "Loading MNIST sample: " << sample;

		// Iterate through the image.
		for (size_t yi=0; (yi+patch_size) <= image_height; yi++)
			for (size_t xi=0; (xi +patch_size) <= image_width; xi++) {
				// Create a new matrix.
				mic::types::MatrixXfPtr patch (new mic::types::MatrixXf(patch_size, patch_size));

				// Iterate through the patch.
				for (size_t yp=0; yp < patch_size; yp++)
					for (size_t xp=0; xp < patch_size; xp++) {
						// Parse and set image data.
						(*patch)(yp, xp) = image(yi+yp, xi+xp);
				}//: for patch

				sample_data.push_back(patch);
				sample_labels.push_back(std::make_shared <unsigned int> (temp_label) );
			}//: for image

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
