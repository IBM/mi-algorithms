/*!
 * \file UIntMatrixXfEncoder.cpp
 * \brief 
 * \author tkornut
 * \date Apr 15, 2016
 */

#include <encoders/UIntMatrixXfEncoder.hpp>

#include <logger/Log.hpp>

#include <cstring> // for memset
#ifdef _WIN32
#include <string.h>
#endif

namespace mic {
namespace encoders {

mic::types::MatrixXfPtr UIntMatrixXfEncoder::encodeSample(const std::shared_ptr<unsigned int>& sample_) {
	// Create returned SDR - a vector, i.e. a matrix with number of columns equal to 1.
	mic::types::MatrixXfPtr sdr (new mic::types::MatrixXf(sdr_length,1));
	// Set all zeros.
	sdr->setZero();

	unsigned int index = (*sample_);

	if (index >= sdr_length)
		LOG(LERROR) << "The SDR is too short for proper encoding of "<<index<<"!";
	else
		(*sdr)(index) = 1;
	return sdr;
}

std::shared_ptr<unsigned int> UIntMatrixXfEncoder::decodeSample(const mic::types::MatrixXfPtr& sdr_) {
	// SDR must be in fact a vector, with number of columns equal to 1.
	assert(sdr_->cols() == 1);

	// Find index of max value.
	mic::types::MatrixXf::Index maxRow, maxCol;
	float max_value = sdr_->maxCoeff(&maxRow, &maxCol);
	LOG(LDEBUG) << "(maxRow, maxCol) = max_value [ (" << maxRow << "," << maxCol <<") = " << max_value << "]";

	// Convert to int.
	unsigned int decoded = maxRow;

	return std::make_shared<unsigned int>(decoded);
}

} /* namespace encoders */
} /* namespace mic */
