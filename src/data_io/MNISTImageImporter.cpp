/*!
 * \file MNISTImageImporter.cpp
 * \brief 
 * \author tkornut
 * \date Dec 21, 2015
 */

#include <data_io/MNISTImageImporter.hpp>

#include <logger/Log.hpp>

// Dependency on data_utils.
#include <data_utils/image_tools.hpp>

#include <fstream>

namespace mic {
namespace data_io {

MNISTImageImporter::MNISTImageImporter(std::string node_name_) : Importer (node_name_),
		data_filename("data_filename","images-idx3-ubyte"),
		labels_filename("labels_filename", "labels-idx1-ubyte")
{
	// Register properties - so their values can be overridden (read from the configuration file).
	registerProperty(data_filename);
	registerProperty(labels_filename);
	// Set image properties.
	image_width = 28;
	image_height = 28;
}

bool MNISTImageImporter::importData(){

	// Read labels file.
	std::ifstream labels_file(labels_filename, std::ios::in | std::ios::binary);

	char buffer[28*28];

	int offset_bytes = 8;

	LOG(LSTATUS) << "Importing MNIST labels from file: " << labels_filename;
	// Read labels from file.
	if (labels_file.is_open()) {

			labels_file.seekg (offset_bytes, std::ios::beg);
			// While not eof.
			while(!labels_file.eof()) {

				labels_file.read(buffer, 1);

				// If not reached the EOF.
				if (!labels_file.eof()) {
					unsigned int temp_label = (unsigned int)buffer[0];

					sample_labels.push_back(std::make_shared <unsigned int> (temp_label) );

				}//: if !eof
			}//: while !eof

		labels_file.close();

	} else {

		LOG(LFATAL) << "Oops! Couldn't find file: " << labels_filename;
		return false;
	}//: else
	LOG(LINFO) << "Imported " << sample_labels.size() << " labels";

	LOG(LSTATUS) << "Importing MNIST images from file: " << data_filename;
	// Read file containing images (binary format).
	offset_bytes = 16;
	int count = 0;
	std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);

	if (data_file.is_open()) {

		// Skip header.
		data_file.seekg (offset_bytes, std::ios::beg);

		// While !EOF.
		while(!data_file.eof()) {

			data_file.read(buffer, image_width*image_height);

			// If not reached the EOF.
			if (!data_file.eof()) {
				std::shared_ptr<mic::types::image> img_ptr (new mic::types::image());

				// Set image label ID - DEPRICATED.
				img_ptr->label_id = *sample_labels[(unsigned int)count++];

				// Alloc image data.
				mic::data_utils::alloc_image( img_ptr.get(), mic::types::GRAYSCALE, image_width, image_height);

				// Parse and set image data.
				for (size_t i = 0; i <  (size_t)image_width*image_height; i++) {

					unsigned row = i / image_width;
					unsigned col = i % image_height;

					set_color_float( img_ptr.get(), mic::types::GRAY, row, col, (float)(uint8_t)buffer[i]/255.0f);
				}//: for
				// Add image to data vector.
				sample_data.push_back( img_ptr );
			}//: if !eof

		}//: while !eof

		LOG(LINFO) << "Imported " << sample_data.size() << " MNIST images";
		data_file.close();

		// Fill the indices table(!)
		for (size_t i=0; i < sample_data.size(); i++ )
			sample_indices.push_back(i);

	} else {
		LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
		return false;
	}
	LOG(LINFO) << "Data import finished";
	return true;
}

} /* namespace data_io */
} /* namespace mic */
