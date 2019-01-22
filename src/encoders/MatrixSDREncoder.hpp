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
 * \file MatrixSDREncoder.hpp
 * \brief 
 * \author tkornut
 * \date Mar 30, 2016
 */

#ifndef SRC_ENCODERS_MATRIXSDRENCODER_HPP_
#define SRC_ENCODERS_MATRIXSDRENCODER_HPP_

#include <encoders/Encoder.hpp>

#include <types/MatrixTypes.hpp>

namespace mic {
namespace encoders {

/*!
 * @brief Abstract parent class for all encoders using MatrixXf as SDR datatype.
 *
 * @author tkornuta
 * @tparam inputDataType Template parameter defining the input (sample) datatype.
 * @tparam SDRType Template parameter defining the SDR type.
 */
template <typename inputDataType, typename outputDataType = float>
class MatrixSDREncoder : public mic::encoders::Encoder <inputDataType, mic::types::Matrix<outputDataType> > {
public:
	/*!
	 * @brief Constructor.
	 * @param sdr_length_ SDR length.
	 */
    MatrixSDREncoder(size_t sdr_length_) : mic::encoders::Encoder<inputDataType, mic::types::Matrix<outputDataType> >(),
		sdr_length(sdr_length_)
	{
//		std::cout<<" Hello MatrixXfSDREncoder\n";
	};

	/*!
	 * Virtual destructor - empty.
	 */
    virtual ~MatrixSDREncoder() { }


	/*!
	 * Method responsible for encoding batch containing several samples into matrix containing several SDRs.
	 * @param[in] batch_ Vector of shared pointers containing samples
	 * @return Matrix containing several SDRs.
	 */
    virtual std::shared_ptr<mic::types::Matrix<outputDataType> > encodeBatch(const std::vector<std::shared_ptr<inputDataType> >& batch_) {
		// Create returned matrix.
        std::shared_ptr<mic::types::Matrix<outputDataType> > sdrs (new mic::types::Matrix<outputDataType> (sdr_length, batch_.size()));

		// Encode the samples one by one.
		for (size_t i=0; i < batch_.size(); i++ ) {
			// Encode single sample.
            std::shared_ptr<mic::types::Matrix<outputDataType> > sample_sdr = this->encodeSample(batch_[i]);

			// Set SDR rows.
			sdrs->col(i) = sample_sdr->col(0);
		}//: for

		// Return the matrix containing SDRs.
		return sdrs;
	}


	/*!
	 * Method responsible for encoding batch containing several samples into matrix containing several SDRs. Each sample SDR is stored in a separate column.
	 * @param[in] batch_ Vector of shared pointers containing samples
	 * @return Matrix containing several SDRs.
	 */
    virtual std::vector<std::shared_ptr<inputDataType> > decodeBatch(const std::shared_ptr<mic::types::Matrix<outputDataType> >& sdrs_) {
		// Create the output vector.
		std::vector<std::shared_ptr<inputDataType> > output;
        std::shared_ptr<mic::types::Matrix<outputDataType> > sample_sdr (new mic::types::Matrix<outputDataType> (sdrs_->rows(), 1));

		// Iterate through columns and decode them one by one.
		for (size_t i=0; i < (size_t)sdrs_->cols(); i++ ) {
			sample_sdr->col(0) = sdrs_->col(i);
			std::shared_ptr<inputDataType> decoded = this->decodeSample(sample_sdr);
			// Add result to vector.
			output.push_back(decoded);
		}//: for

		// Return the decoded vector.
		return output;
	}


	/*!
	 * Returns the SDR length.
	 */
	size_t getSDRLength() {
		return sdr_length;
	}

	/*!
	 * Sets SDR length.
	 * @param sdr_length_ SDR length.
	 */
	void setSDRLength(size_t sdr_length_) {
		sdr_length = sdr_length_;
	}

protected:

	/// Length of SDR.
	size_t sdr_length;
};

} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_ENCODERS_MATRIXSDRENCODER_HPP_ */
