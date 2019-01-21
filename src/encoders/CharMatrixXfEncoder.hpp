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
 * \file CharMatrixXfEncoder.hpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#ifndef SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_
#define SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_

#include <encoders/MatrixSDREncoder.hpp>

namespace mic {
namespace encoders {

/*!
 * \brief Encoder responsible for encoding character into Matrix SDRs.
 * A 1-of-k encoder, i.e. it simply encodes char to ASCII and turns on that bit, i.e. there is no learning.
 * \author tkornuta
 */
class CharMatrixXfEncoder: public mic::encoders::MatrixSDREncoder<char, float> {
public:
	/*!
	 * Default constructor.
	 * @param sdr_length_ SDR length.
	 */
    CharMatrixXfEncoder(size_t sdr_length_) : MatrixSDREncoder(sdr_length_) {
//		std::cout<<" Hello CharMatrixXfEncoder\n";
	};

	/// Default destructor - empty.
	~CharMatrixXfEncoder() { }

	/*!
	 * @brief Method responsible for encoding of input data sample into SDR (a vector of floats).
	 * @param[in] sample_ Shared pointer to a char.
	 * @return Shared pointer to SDR - here in the form of 1d matrix (cols,1) of floats. Memory to this variable must be assigned earlier.
	 */
	virtual mic::types::MatrixXfPtr encodeSample(const std::shared_ptr<char>& sample_);

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[in] sdr_ Shared pointer to SDR.
	 * @return Shared pointer to a char
	 */
	virtual std::shared_ptr<char> decodeSample(const mic::types::MatrixXfPtr& sdr_);
};

} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_ */
