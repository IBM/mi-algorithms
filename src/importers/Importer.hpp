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
 * \file Importer.hpp
 * \brief Contains declaration (and definition) of base template class of all data importers.
 * \author tkornuta
 * \date Dec 21, 2015
 */

#ifndef SRC_importers_IMPORTER_HPP_
#define SRC_importers_IMPORTER_HPP_

#include <vector>
#include <stdexcept>

#include <logger/Log.hpp>
#include <configuration/PropertyTree.hpp>

#include <types/Batch.hpp>

namespace mic {

/**
 * \namespace mic::importers
 * \brief Namespace containing data import/export classess and procedures.
 * \author tkornuta
 */
namespace importers {

/*!
 * @brief Parent class for all data importers.
 *
 * @author tkornuta
 * @tparam DataType Template parameter defining the imported datatype.
 * @tparam LabelType Template parameters defining the datatype of label.
 */
template <typename DataType, typename LabelType>
class Importer : public mic::types::Batch<DataType, LabelType>, public mic::configuration::PropertyTree {
public:
	/*!
	 * Constructor. Initializes random generator and sets index to zero. Registers index property.
	 * @param node_name_ Name of the node in configuration file.
	 * @param batch_size_ Size of the batch.
	 */
	Importer(std::string node_name_, size_t batch_size_ = 1) : mic::types::Batch<DataType, LabelType>::Batch(batch_size_), PropertyTree(node_name_)
	{
		// Register properties - so their values can be overridden (read from the configuration file).
	}

	/*!
	 * Virtual destructor.
	 */
	virtual ~Importer() {
	}

	/*!
	 * Abstract method responsible for importing/loading the samples.
	 * @return TRUE if data loaded successfully, FALSE otherwise.
	 */
	virtual bool importData() = 0;

};

} /* namespace importers */
} /* namespace mic */

#endif /* SRC_importers_IMPORTER_HPP_ */
