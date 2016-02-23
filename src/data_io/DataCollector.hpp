/*!
 * \file DataCollector.hpp
 * \brief 
 * \author tkornut
 * \date Feb 22, 2016
 */

#ifndef SRC_DATA_IO_DATACOLLECTOR_HPP_
#define SRC_DATA_IO_DATACOLLECTOR_HPP_

#include<vector>
#include <string>
#include <map>
#include <limits>

#include<logger/Log.hpp>

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
	 * Maximum value.
	 */
	DATA_TYPE max_value;

	/*!
	 * Minimum value.
	 */
	DATA_TYPE min_value;

};

/*!
 * \brief Type representing a set (map) of containers.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 * \tparam LABEL_TYPE Template parameter denoting the label type.
 * \author tkornuta
 */
template<class LABEL_TYPE, class DATA_TYPE>
using DataContainers = typename std::map<LABEL_TYPE, DataContainer <DATA_TYPE> >;



/*!
 * \brief Type representing a data container iterator.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 * \tparam LABEL_TYPE Template parameter denoting the label type.
 * \author tkornuta
 */
template<class LABEL_TYPE, class DATA_TYPE>
using DataContainerIt = typename std::map<LABEL_TYPE, DataContainer <DATA_TYPE> >::iterator;


/*!
 * \brief Class responsible for collection of data during experiments end exporting the results to files.
 * \tparam DATA_TYPE Template parameter denoting basic used datatype.
 * \tparam LABEL_TYPE Template parameter denoting the label type.
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
	 */
	void createContainer(LABEL_TYPE label_, DATA_TYPE min_, DATA_TYPE max_) {
		LOG(LTRACE)<< "DataCollector::createDataContainer";

		DataContainer<DATA_TYPE> tmp;
		// Set min-max parameters.
		tmp.auto_scale = false;
		tmp.min_value = min_;
		tmp.max_value = max_;

		// Add container.
		containers.insert( std::make_pair (label_, tmp) );
	}

	/*!
	 * Creates new data container for a given label. This container will automatically scale (find the min/max values when adding new ones)
	 * @param label_ Name of the container.
	 */
	void createContainer(LABEL_TYPE label_) {
		LOG(LTRACE)<< "DataCollector::createDataContainer";

		DataContainer<DATA_TYPE> tmp;
		// Set min-max parameters.
		tmp.auto_scale = true;
		tmp.min_value = std::numeric_limits<DATA_TYPE>::max();
		tmp.max_value = std::numeric_limits<DATA_TYPE>::min();
		// Add data container.
		containers.insert( std::make_pair (label_, tmp) );
	}


	/*!
	 * Adds new value to the container.
	 * @param label_ Name of the container.
	 * @param value_ Value to be added.
	 */
	void addDataToContainer(LABEL_TYPE label_, DATA_TYPE value_){
		LOG(LTRACE)<< "DataCollector::addDataToContainer<int>";

		// Try to find the label in registry.
		DataContainerIt<LABEL_TYPE, DATA_TYPE> it = containers.find(label_);

		if (it != containers.end()) {
			// Add new value to data.
			(it->second).data.push_back(value_);
			// Check autoscale.
			if ((it->second).auto_scale) {
				(it->second).min_value = ((it->second).min_value > value_) ? (it->second).min_value : value_;
				(it->second).max_value = ((it->second).max_value > value_) ? (it->second).max_value : value_;
			}
		} else {
			LOG(LERROR) << "There is no container with label: " << label_;
		}
	}

	/*!
	 * Returns the container with a given label.
	 * @param label_ Name of the container.
	 * @return Container with given label. If container not found - it will return an empty container.
	 */
	DataContainer<DATA_TYPE> getContainer(LABEL_TYPE label_){
		LOG(LTRACE)<< "DataCollector::getContainer";

		// Try to find the label in registry.
		DataContainerIt<LABEL_TYPE, DATA_TYPE> it = containers.find(label_);

		if (it == containers.end()) {
			LOG(LERROR) << "There is no container with label: " << label_;
			// Return empty container.
			DataContainer<DATA_TYPE> tmp;
			tmp.auto_scale = true;
			tmp.min_value = std::numeric_limits<DATA_TYPE>::max();
			tmp.max_value = std::numeric_limits<DATA_TYPE>::min();
			return tmp;
		} else
			return it->second;
	}

	/*!
	 * Returns the containers.
	 * @return Map of all stored containers.
	 */
	DataContainers<LABEL_TYPE, DataContainer <DATA_TYPE> > getContainers(){
		LOG(LTRACE)<< "DataCollector::getContainer";
		return containers;
	}



protected:
	/*!
	 * Registry storing the containers.
	 */
	std::map<LABEL_TYPE, DataContainer <DATA_TYPE> > containers;

};





} /* namespace data_io */
} /* namespace mic */

#endif /* SRC_DATA_IO_DATACOLLECTOR_HPP_ */
