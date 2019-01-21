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
 * \file STL10MatrixImporter.hpp
 * \brief 
 * \author tkornut, Alexis-Asseman
 * \date Jul 21, 2017
 */


#ifndef SRC_DATA_IO_STL10MatrixImporter_HPP_
#define SRC_DATA_IO_STL10MatrixImporter_HPP_

#include <types/MatrixTypes.hpp>
#include <data_io/Importer.hpp>

namespace mic {
namespace data_io {

class STL10MatrixImporter: public mic::data_io::Importer< mic::types::MatrixXf, unsigned int > {
public:
	/*!
     * Constructor. Sets STL-10 image default properties. Registers properties.
	 * @param node_name_ Name of the node in configuration file.
     * @param data_filename_ File (with path) containing STL-10 images.
     * @param labels_filename_ File (with path) containing STL-10 labels.
	 */
    STL10MatrixImporter(std::string node_name_ = "stl10_matrix_importer", std::string data_filename_ = "", std::string labels_filename_ = "");

	/*!
	 * Virtual destructor. Empty.
	 */
    virtual ~STL10MatrixImporter() { };

	/*!
     * Set name and patch of the file containing STL-10 images.
	 * @param data_filename_ File (with path) containing MNIST images.
	 */
	void setDataFilename(std::string data_filename_);

	/*!
     * Set name and patch of the file containing STL-10 labels.
	 * @param labels_filename_ File (with path) containing MNIST labels.
	 */
	void setLabelsFilename(std::string labels_filename_);

	/*!
	 * Method responsible for importing/loading the MNIST dataset.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	bool importData();

	/*!
	 * Method responsible for initialization of all variables that are property-dependent - here not required, yet empty.
	 */
	virtual void initializePropertyDependentVariables() { };

private:
	/*!
     * Width of STL-10 image.
	 */
	int image_width;

	/*!
     * Height of STL-10 image.
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

    bool labeled = false;
};


} /* namespace data_io */
} /* namespace mic */


#endif /* SRC_DATA_IO_STL10MatrixImporter_HPP_ */
