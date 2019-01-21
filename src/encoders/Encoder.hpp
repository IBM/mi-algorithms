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
 * \file Encoder.hpp
 * \brief Contains definition of an abstract, template parent class for all auto-encoders.
 * \author tkornuta
 * \date Nov 16, 2015
 */

#ifndef __ENCODER_HPP__
#define __ENCODER_HPP__

#include <string>
#include <vector>
#include <iostream>

#include <memory> // std::shared_ptr

namespace mic {

/*!
 * \brief Contains encoder parent class and all derived classes.
 * \author tkornuta
 */
namespace encoders {

/*!
 * @brief Abstract parent class for all encoders.
 *
 * @author tkornuta
 * @tparam inputDataType Template parameter defining the input (sample) datatype.
 * @tparam SDRType Template parameter defining the SDR type.
 */
template <typename inputDataType, typename SDRType>
class Encoder {
public:

	/*!
	 * @brief Constructor - empty.
	 */
	Encoder()  {
//		std::cout<<" Hello Encoder\n";
	};

	/*!
	 * Virtual destructor - empty.
	 */
	virtual ~Encoder() { };

	/*!
	 * @brief Method responsible for encoding input sample into SDR.
	 * @param[in] sample_ Shared pointer to sample data.
	 * @return Shared pointer to encoded SDR.
	 */
	virtual std::shared_ptr<SDRType> encodeSample(const std::shared_ptr<inputDataType>& sample_) = 0;


	/*!
	 * Method responsible for decoding of SDR back into original data type.
	 * @param[in] sdr_ Shared pointer to SDR.
	 * @return Shared pointer to decoded data.
	 */
	virtual std::shared_ptr<inputDataType> decodeSample(const std::shared_ptr<SDRType>& sdr_) = 0;

};



} // namespace auto_encoders
} // namespace mic

#endif
