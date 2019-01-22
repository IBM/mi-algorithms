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
 * \file CharMatrixXfEncoder.cpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#include <encoders/CharMatrixXfEncoder.hpp>

#include <logger/Log.hpp>

#include <cstring> // for memset 
#ifdef _WIN32
#include <string.h>
#endif

namespace mic {
namespace encoders {

mic::types::MatrixXfPtr CharMatrixXfEncoder::encodeSample(const std::shared_ptr<char>& sample_) {
	// Create returned SDR - a vector, i.e. a matrix with number of columns equal to 1.
	mic::types::MatrixXfPtr sdr (new mic::types::MatrixXf(sdr_length,1));
	// Set all zeros.
	sdr->setZero();

	// Convert ASCII char to int.
	char sample = *sample_;
	int a = sample - 32;
	//std::cout <<" input_ = " << input_ <<" a = " << a;
	if (a < 0)
		LOG(LERROR) << "Could not properly encode character '" <<sample << "' ("<<a<<")!";
	else if ((size_t) a >= (size_t) sdr_length)
		LOG(LERROR) << "The SDR is too short for proper encoding of the character '" <<sample << "' ("<<a<<")!";
	else
		(*sdr)(a) = 1;
	return sdr;
}

std::shared_ptr<char> CharMatrixXfEncoder::decodeSample(const mic::types::MatrixXfPtr& sdr_) {
	// SDR must be in fact a vector, with number of columns equal to 1.
	assert(sdr_->cols() == 1);

	// Find index of max value.
	mic::types::MatrixXf::Index maxRow, maxCol;
	float max_value = sdr_->maxCoeff(&maxRow, &maxCol);
	LOG(LDEBUG) << "(maxRow, maxCol) = max_value [ (" << maxRow << "," << maxCol <<") = " << max_value << "]";

	// Convert int to ASCII.
	char decoded = maxRow + 32;

	return std::make_shared<char>(decoded);
}


} /* namespace encoders */
} /* namespace mic */
