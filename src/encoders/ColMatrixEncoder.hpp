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
 * \file ColMatrixEncoder.hpp
 * \brief 
 * \author tkornuta
 * \date Mar 29, 2016
 */

#ifndef COLMATRIXENCODER_H_
#define COLMATRIXENCODER_H_

#include <encoders/MatrixSDREncoder.hpp>

namespace mic {
namespace encoders {

/*!
 * \brief Encoder responsible for encoding single channel (float) matrices (e.g. grayscale images) into SDRs.
 * There is no learning (auto-encoding), instead it simply transforms matrix of floats into a vector.
 * \author tkornuta
 */

template<typename T = float>
class ColMatrixEncoder : public mic::encoders::MatrixSDREncoder<mic::types::Matrix<T>, T> {
public:

	/*!
	 * Default constructor. Sets the encoding parameters.
	 * @param matrix_width_ Width of the matrix.
	 * @param matrix_height_ Height of the matrix.
	 */
    ColMatrixEncoder(size_t matrix_height_, size_t matrix_width_) : MatrixSDREncoder<mic::types::Matrix<T>, T>(matrix_height_*matrix_width_),
		matrix_height(matrix_height_),
		matrix_width(matrix_width_)
	{
//		std::cout<<" Hello MatrixXfMatrixXfEncoder\n";
	};

	/// Default destructor - empty.
    ~ColMatrixEncoder() { }

	/*!
	 * @brief Method responsible for encoding of input data sample into SDR (a 1D matrix - vector of floats).
	 * @param[in] sample_ Shared pointer to a matrix.
	 * @return Shared pointer to SDR - here in the form of 1D matrix (rows,1) of floats. Memory to this variable must be assigned earlier.
	 */
    virtual mic::types::MatrixPtr<T> encodeSample(const mic::types::MatrixPtr<T>& sample_){
        // Create new matrix.
        mic::types::MatrixPtr<T> sdr (new mic::types::Matrix<T>(sample_->cols(), sample_->rows()));
        // Copy data.
        (*sdr) = (*sample_);
        // Resize.
        sdr->resize(sdr_length, 1);
        return sdr;
    }

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[in] sdr_ Shared pointer to SDR (1D matrix).
	 * @return Shared pointer to a 2D matrix.
	 */
    virtual mic::types::MatrixPtr<T> decodeSample(const mic::types::MatrixPtr<T>& sdr_){
        // Create new matrix.
        mic::types::MatrixPtr<T> decoded (new mic::types::Matrix<T>(sdr_->cols(), sdr_->rows()));
        // Copy data.
        (*decoded) = (*sdr_);
        // Resize.
        decoded->resize(matrix_height, matrix_width);
        return decoded;
    }

protected:
	/// Height of the matrix - number of rows.
	size_t matrix_height;

	/// Width of the matrix - number of columns.
	size_t matrix_width;

private:
    using MatrixSDREncoder<mic::types::Matrix<T>, T>::sdr_length;

};



} /* namespace encoders */
} /* namespace mic */

#endif /* COLMATRIXENCODER_H_ */
