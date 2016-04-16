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

MNISTMatrixImporter::MNISTMatrixImporter(std::string data_filename_, std::string labels_filename_, std::string node_name_) : Importer (node_name_),
		data_filename("data_filename",data_filename_),
		labels_filename("labels_filename", labels_filename_)
{
	// Register properties - so their values can be overridden (read from the configuration file).
	registerProperty(data_filename);
	registerProperty(labels_filename);

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


/*int ReverseInt (int i)
{
    unsigned char ch1, ch2, ch3, ch4;
    ch1=i&255;
    ch2=(i>>8)&255;
    ch3=(i>>16)&255;
    ch4=(i>>24)&255;
    return((int)ch1<<24)+((int)ch2<<16)+((int)ch3<<8)+ch4;
}*/


bool MNISTMatrixImporter::importData(){

	std::ifstream labels_file(labels_filename, std::ios::in | std::ios::binary);

	char buffer[28*28];

	int offset_bytes = 8;

	LOG(LSTATUS) << "Importing MNIST labels from file: " << labels_filename;
	// Import labels from file.
	if (labels_file.is_open()) {

			labels_file.seekg (offset_bytes, std::ios::beg);
			// While not eof.
			while(!labels_file.eof()) {

				// Read label.
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
	}

	LOG(LINFO) << "Imported " << sample_labels.size() << " labels";

	offset_bytes = 16;

	LOG(LSTATUS) << "Importing MNIST images from file: " << data_filename;

	// Open data file.
    std::ifstream data_file(data_filename, std::ios::in | std::ios::binary);

	// While file is open.
	if (data_file.is_open()) {

/*
    	// Read header.
        int magic_number=0;
        int number_of_images=0;
        int n_rows=0;
        int n_cols=0;
        data_file.read((char*)&magic_number,sizeof(magic_number));
        magic_number= ReverseInt(magic_number);
        data_file.read((char*)&number_of_images,sizeof(number_of_images));
        number_of_images= ReverseInt(number_of_images);
        data_file.read((char*)&n_rows,sizeof(n_rows));
        n_rows= ReverseInt(n_rows);
        data_file.read((char*)&n_cols,sizeof(n_cols));
        n_cols= ReverseInt(n_cols);
*/
		// Instead - skip it.
		data_file.seekg (offset_bytes, std::ios::beg);

		// While not reached end of file.
		while(!data_file.eof()) {

			// Read buffer.
			data_file.read(buffer, image_width*image_height);

			// If not reached the EOF.
			if (!data_file.eof()) {

				// Create new matrix of MNIST image size.
				mic::types::MatrixXfPtr mat (new mic::types::MatrixXf(image_height, image_width));

				// Parse and set image data.
				for (size_t i = 0; i < image_width*image_height; i++) {

					unsigned row = i / image_width;
					unsigned col = i % image_height;

					(*mat)(row, col) = (float)((uint8_t)buffer[i])/255.0f;

				}//: for

				sample_data.push_back(mat);
			}//: if !eof

		}//: while !eof

		LOG(LINFO) << "Imported " << sample_data.size() << " MNIST images";
		data_file.close();

		// Fill the indices table(!)
		for (size_t i=0; i < sample_data.size(); i++ )
			sample_indices.push_back(i);

		// Count the classes.
		//countClasses();
		number_of_classes = 10;

		LOG(LINFO) << "Data import finished";

	} else {
		LOG(LFATAL) << "Oops! Couldn't find file: " << data_filename;
		return false;
	}

	return true;
}




} /* namespace data_io */
} /* namespace mic */
