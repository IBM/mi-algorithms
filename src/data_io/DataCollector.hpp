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
 * \file DataCollector.hpp
 * \brief 
 * \author tkornut
 * \date Feb 22, 2016
 */

#ifndef SRC_DATA_IO_DATACOLLECTOR_HPP_
#define SRC_DATA_IO_DATACOLLECTOR_HPP_

#include <vector>
#include <string>
#include <map>
#include <limits>

#include <fstream>      // std::ofstream

#include<logger/Log.hpp>

#include <types/image_types.hpp>

#include <types/MatrixTypes.hpp>

namespace mic {
namespace data_io {

/*!
 * Template container type used in data collector.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 */
template <typename DATA_TYPE>
struct DataContainer {
	/*!
	 * Vector storing the data.
	 */
	std::vector<DATA_TYPE> data;

	/*!
	 * Flag denoting whether data should be scaled automatically.
	 */
	bool auto_scale;

	/*!
	 * Minimum value.
	 */
	DATA_TYPE min_value;

	/*!
	 * Maximum value.
	 */
	DATA_TYPE max_value;

	/*!
	 * Colour - used in visualization.
	 */
	mic::types::color_rgba color;

	/*!
	 * Colour - used in visualization.
	 */
	float line_width;
};

/*!
 * \brief Type representing a pointer to data container.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 * \author tkornuta
 */
template<class DATA_TYPE>
using DataContainerPtr = typename std::shared_ptr < DataContainer < DATA_TYPE> >;

/*!
 * \brief Type representing a set (map) of containers.
 * \tparam LABEL_TYPE Template parameter denoting the label type.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 * \author tkornuta
 */
template<class LABEL_TYPE, class DATA_TYPE>
using DataContainers = typename std::map<LABEL_TYPE, DataContainerPtr <DATA_TYPE> >;



/*!
 * \brief Type representing a data container iterator.
 * \tparam LABEL_TYPE Template parameter denoting the label type.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 * \author tkornuta
 */
template<class LABEL_TYPE, class DATA_TYPE>
using DataContainerIt = typename std::map<LABEL_TYPE, DataContainerPtr <DATA_TYPE> >::iterator;


/*!
 * \brief Class responsible for collection of data during experiments end exporting the results to files.
 * \tparam LABEL_TYPE Template parameter denoting the label type.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 * \author tkornuta
 */
template <class LABEL_TYPE, class DATA_TYPE>
class DataCollector {
public:
	/*!
	 * Constructor. Empty for now.
	 */
	DataCollector() { };

	/*!
	 * Destructor. Empty for now.
	 */
	virtual ~DataCollector() { };

	/*!
	 * Creates new data container for a given label. Sets min and max values.
	 * @param label_ Name of the container.
	 * @param min_ Minimum value (used in visualization).
	 * @param min_ Maximum value (used in visualization).
	 * @param color_ Colour of line/label (used in visualization).
	 * @param line_width_ Line width (used in visualization).
	 */
	void createContainer(LABEL_TYPE label_, DATA_TYPE min_, DATA_TYPE max_, mic::types::color_rgba color_ = mic::types::color_rgba(255, 255, 255, 180), float line_width_ = 1.0f) {
		LOG(LTRACE)<< "DataCollector::createDataContainer";

		DataContainerPtr<DATA_TYPE> tmp (new DataContainer<DATA_TYPE> );
		// Set min-max parameters.
		tmp->auto_scale = false;
		tmp->min_value = min_;
		tmp->max_value = max_;
		tmp->line_width = line_width_;
		tmp->color = color_;
		// Add container.
		containers.insert( std::make_pair (label_, tmp) );
	}

	/*!
	 * Creates new data container for a given label. This container will automatically scale (find the min/max values when adding new ones)
	 * @param label_ Name of the container.
	 * @param color_ Colour of line/label (used in visualization).
	 * @param line_width_ Line width (used in visualization).
	 */
	void createContainer(LABEL_TYPE label_, mic::types::color_rgba color_ = mic::types::color_rgba(255, 255, 255, 180), float line_width_ = 1.0f) {
		LOG(LTRACE)<< "DataCollector::createDataContainer";

		DataContainerPtr<DATA_TYPE> tmp (new DataContainer<DATA_TYPE> );
		// Set min-max parameters.
		tmp->auto_scale = true;
		tmp->min_value = std::numeric_limits<DATA_TYPE>::max();
		tmp->max_value = std::numeric_limits<DATA_TYPE>::min();
		tmp->line_width = line_width_;
		tmp->color = color_;
		// Add data container.
		containers.insert( std::make_pair (label_, tmp) );
	}


	/*!
	 * Adds new value to the container.
	 * @param label_ Name of the container.
	 * @param value_ Value to be added.
	 */
	void addDataToContainer(LABEL_TYPE label_, DATA_TYPE value_){
		LOG(LTRACE)<< "DataCollector::addDataToContainer";

		// Try to find the label in registry.
		DataContainerIt<LABEL_TYPE, DATA_TYPE> it = containers.find(label_);

		if (it != containers.end()) {
			// Add new value to data.
			(it->second)->data.push_back(value_);
			// Check autoscale.
			if ((it->second)->auto_scale) {
				(it->second)->min_value = (value_ < (it->second)->min_value) ? value_ : (it->second)->min_value;
				(it->second)->max_value = (value_ > (it->second)->max_value) ? value_ : (it->second)->max_value;
			}
		} else {
			LOG(LERROR) << "There is no container with label: " << label_;
		}
	}

	/*!
	 * Returns the data stored by container with a given label.
	 * @param label_ Name of the container.
	 * @return Container with given label. If container not found - it will return an empty container.
	 */
	DataContainerPtr<DATA_TYPE> getDataFromContainer(LABEL_TYPE label_){
		LOG(LTRACE)<< "DataCollector::getContainerData";

		// Try to find the label in registry.
		DataContainerIt<LABEL_TYPE, DATA_TYPE> it = containers.find(label_);

		if (it == containers.end()) {
			LOG(LERROR) << "There is no container with label: " << label_;
			// Return empty container.
			DataContainerPtr<DATA_TYPE> tmp (new DataContainer<DATA_TYPE> );
			tmp->auto_scale = true;
			tmp->min_value = std::numeric_limits<DATA_TYPE>::max();
			tmp->max_value = std::numeric_limits<DATA_TYPE>::min();
			return tmp;
		} else
			return it->second;
	}

	/*!
	 * Returns the containers.
	 * @return Map of all stored containers.
	 */
	DataContainers< LABEL_TYPE, DATA_TYPE > getContainers(){
		LOG(LTRACE)<< "DataCollector::getContainer";
		return containers;
	}


	/*!
	 * Exports collected data to csv.
	 * @param filename_ Output filename (=data.csv).
	 */
	void exportDataToCsv(std::string filename_ = "data.csv"){
		LOG(LTRACE)<< "DataCollector::exportDataToCsv";
		// Open output filestream.
		std::ofstream output(filename_);

		// Iterate through containers.
		for(DataContainerIt<LABEL_TYPE, DATA_TYPE> it = containers.begin(); it != containers.end(); it++) {
			// Export header.
			output << it->first << ", min ,";
			output << (it->second)->min_value << ", max, ";
			output << (it->second)->max_value << std::endl;

			// Export data.
			if ((it->second)->data.size() > 1)
				std::copy((it->second)->data.begin(), (it->second)->data.end() - 1, std::ostream_iterator<DATA_TYPE>(output, ", "));
			if ((it->second)->data.size())
				output << (it->second)->data.back() << std::endl;
		}//: for
	}

	/*!
	 * Exports the vector to csv.
	 * @param filename_ Output filename.
	 * @param label_ Data label (written in file, above the line containing data)
	 * @param data_ Data (vector)
	 * @param append_ Flag denoting whether data should be added to existing data in a file or file should be truncated.
	 */
	static void exportVectorToCsv(std::string filename_, std::string label_, std::vector<DATA_TYPE> data_, bool append_ = false){
		LOG(LTRACE)<< "DataCollector::exportVectorToCsv";
		// Open output filestream.
		std::ofstream output;
		if (!append_)
			output.open(filename_);
		else
			output.open(filename_, std::ofstream::out | std::ofstream::app);

		// Export header
		output << label_ << std::endl;

		// Export data.
		if (data_.size() > 1)
			std::copy(data_.begin(), data_.end() - 1, std::ostream_iterator<DATA_TYPE>(output, ", "));
		if (data_.size())
			output << data_.back() << std::endl;
	}

	/*!
	 * Exports a single value to file.
	 * @param filename_ Output filename.
	 * @param label_ Data label (written in file, before the data)
	 * @param data_ Data (a single value).
	 * @param append_ Flag denoting whether data should be added to existing data in a file or file should be truncated.
	 */
	static void exportValueToCsv(std::string filename_, std::string label_, DATA_TYPE value_, bool append_ = false){
		LOG(LTRACE)<< "DataCollector::exportValueToCsv";
		// Open output filestream.
		std::ofstream output;
		if (!append_)
			output.open(filename_);
		else
			output.open(filename_, std::ofstream::out | std::ofstream::app);

		// Export data.
		output << label_ << ", " << value_ << std::endl;
	}


	/*!
	 * Exports a comment to file.
	 * @param filename_ Output filename.
	 * @param comment_ Comment to be added (in a separate line).
	 * @param append_ Flag denoting whether data should be added to existing data in a file or file should be truncated.
	 */
	static void exportCommentToCsv(std::string filename_, std::string exportCommentToCsv, bool append_ = false){
		LOG(LTRACE)<< "DataCollector::exportCommentToCsv";
		// Open output filestream.
		std::ofstream output;
		if (!append_)
			output.open(filename_);
		else
			output.open(filename_, std::ofstream::out | std::ofstream::app);

		// Export data.
		output << exportCommentToCsv <<  std::endl;
	}


	/*!
	 * Exports Eigen (or derived) matrix to a csv file (in Row-major order).
	 * @param filename_ Output filename.
	 * @param label_ Data label (written in file, above the line containing data)
	 * @param matrix_ (A shared pointer to) Matrix to be exported.
	 * @param append_ Flag denoting whether data should be added to existing data in a file or file should be truncated.
	 */
	static void exportMatrixToCsv(std::string filename_, std::string label_, std::shared_ptr< mic::types::Matrix<DATA_TYPE> > matrix_, bool append_ = false){
		LOG(LTRACE)<< "DataCollector::exportVectorToCsv";
		// Open output filestream.
		std::ofstream output;
		if (!append_)
			output.open(filename_);
		else
			output.open(filename_, std::ofstream::out | std::ofstream::app);

		// Export header.
		output << label_ << std::endl;

		// Export matrix.
		for (size_t y = 0; y < (size_t)matrix_->rows(); y++) {
			for (size_t x = 0; x < (size_t)matrix_->cols(); x++) {
				output << matrix_(y,x) << ", ";
			}//: for
		}//: for
		output << std::endl;
	}



	/*!
	 * Exports vector of matrices to csv (in Row-major order).
	 * @param filename_ Output filename.
	 * @param label_ Data label (written in file, above the line containing data)
	 * @param data_ Data - a vector of Eigen::Matrix pointers.
	 * @param append_ Flag denoting whether data should be added to existing data in a file or file should be truncated.
	 */
	static void exportMatricesToCsv(std::string filename_, std::string label_, std::vector< mic::types::MatrixPtr<DATA_TYPE> > data_, bool append_ = false){
		LOG(LTRACE)<< "DataCollector::exportVectorToCsv";
		// Open output filestream.
		std::ofstream output;
		if (!append_)
			output.open(filename_);
		else
			output.open(filename_, std::ofstream::out | std::ofstream::app);

		// Export header.
		output << label_ << std::endl;

		for (size_t i=0; i<data_.size(); i++) {
			mic::types::MatrixPtr<DATA_TYPE> matrix_ptr = data_[i];

			//output << *matrix;

			//DATA_TYPE* data_ptr = matrix->data();
			// Export matrix.
			for (size_t y = 0; y < (size_t)matrix_ptr->rows(); y++) {
				for (size_t x = 0; x < (size_t)matrix_ptr->cols(); x++) {
					output << (*matrix_ptr)(y,x) << ", ";
				}//: for
			}//: for
			//output << data_ptr[matrix->rows()*matrix->cols() -1] << std::endl;
			output << std::endl;
		}//: for
	}


protected:
	/*!
	 * Registry storing the containers.
	 */
	DataContainers< LABEL_TYPE, DATA_TYPE > containers;

};


/*!
 * \brief A pointer to data collector.
 * \tparam LABEL_TYPE Template parameter denoting the label type.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 * \author tkornuta
 */
template<class LABEL_TYPE, class DATA_TYPE>
using DataCollectorPtr = typename std::shared_ptr<mic::data_io::DataCollector<LABEL_TYPE, DATA_TYPE > >;



} /* namespace data_io */
} /* namespace mic */

#endif /* SRC_DATA_IO_DATACOLLECTOR_HPP_ */
