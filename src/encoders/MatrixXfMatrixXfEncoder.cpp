/*!
 * \file MatrixXfMatrixXfEncoder.cpp
 * \brief 
 * \author tkornuta
 * \date Mar 29, 2016
 */

#include <encoders/MatrixXfMatrixXfEncoder.hpp>

namespace mic {
namespace encoders {

using namespace mic::types;


mic::types::MatrixXfPtr MatrixXfMatrixXfEncoder::encodeSample(const mic::types::MatrixXfPtr& sample_) {
	// Create new matrix.
	mic::types::MatrixXfPtr sdr (new mic::types::MatrixXf(sample_->cols(), sample_->rows()));
	// Copy data.
	(*sdr) = (*sample_);
	// Resize.
	sdr->resize(sdr_length, 1);
	return sdr;
}


mic::types::MatrixXfPtr MatrixXfMatrixXfEncoder::decodeSample(const mic::types::MatrixXfPtr& sdr_) {
	// Create new matrix.
	mic::types::MatrixXfPtr decoded (new mic::types::MatrixXf(sdr_->cols(), sdr_->rows()));
	// Copy data.
	(*decoded) = (*sdr_);
	// Resize.
	decoded->resize(matrix_height, matrix_width);
	return decoded;
}



} // namespace encoders
} // namespace mic
