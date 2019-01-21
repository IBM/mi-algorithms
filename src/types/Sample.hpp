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
 * \file Sample.hpp
 * \brief 
 * \author tkornuta
 * \date Mar 30, 2016
 */

#ifndef SRC_TYPES_SAMPLE_HPP_
#define SRC_TYPES_SAMPLE_HPP_

#include <memory>

namespace mic {
namespace types {

/*!
 * \brief Template class storing the data-label pairs. Additionally it stores the the index of the sample (mainly for debug purposes).
 * @tparam DataType Template parameter defining the sample data type.
 * @tparam LabelType Template parameters defining the sample label label.
 * \author tkornuta
 */
template<typename DataType, typename LabelType>
class Sample {
public:

	/*!
	 * Constructor without parameters - nulls both data and label pointers and set index to -1.
	 */
	Sample() : sample_data(nullptr), sample_label(nullptr), sample_index(-1) { }

	/*!
	 * Constructor.
	 * @param data_ The sample itself (data).
	 * @param label_ Sample label.
	 * @param index_ Sample index.
	 */
	Sample(std::shared_ptr<DataType> data_, std::shared_ptr<LabelType> label_, size_t index_= -1) : sample_data(data_), sample_label(label_), sample_index(index_) { }

	/*!
	 * Copy constructor.
	 * @param sample_ Sample to be copied.
	 */
	Sample(const mic::types::Sample<DataType, LabelType>& sample_) : sample_data(sample_.data()), sample_label(sample_.label()), sample_index(sample_.index()) { }

	/*!
	 * Assignment operator - sets pointers and number.
	 * @param sample_ Sample to be copied.
	 * @return An exact copy of the input sample.
	 */
	mic::types::Sample<DataType, LabelType>& operator =(const mic::types::Sample<DataType, LabelType>& sample_) {
		// Copy sample data.
		this->sample_data = sample_.sample_data;
		this->sample_label = sample_.sample_label;
		this->sample_index = sample_.sample_index;
		// Return object.
		return *this;
	}

	/*!
	 * Destructor. Empty for now.
	 */
	virtual ~Sample() { };

	/// Returns the sample data.
	std::shared_ptr<DataType> data() const {
		return sample_data;
	}

	/// Returns the returns the sample label.
	std::shared_ptr<LabelType> label() const {
		return sample_label;
	}

	/// Returns the sample number (the sample "position" in original dataset).
	size_t index() const {
		return sample_index;
	}

private:
	/// Stores the data.
	std::shared_ptr<DataType> sample_data;

	/// Stores the label.
	std::shared_ptr<LabelType> sample_label;

	/// The sample index (the sample "position" in original dataset).
	size_t sample_index;
};


} /* namespace types */
} /* namespace mic */

#endif /* SRC_TYPES_SAMPLE_HPP_ */
