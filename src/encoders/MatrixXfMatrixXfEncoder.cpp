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


std::shared_ptr<mic::types::MatrixXf> MatrixXfMatrixXfEncoder::encodeSample(const std::shared_ptr<mic::types::MatrixXf>& sample_) {
	// Create new matrix.
	std::shared_ptr<mic::types::MatrixXf> sdr (new mic::types::MatrixXf(sample_->cols(), sample_->rows()));
	// Copy data.
	(*sdr) = (*sample_);
	// Resize.
	sdr->resize(sdr_length, 1);
	return sdr;
}


std::shared_ptr<mic::types::MatrixXf> MatrixXfMatrixXfEncoder::decodeSample(const std::shared_ptr<mic::types::MatrixXf>& sdr_) {
	// Create new matrix.
	std::shared_ptr<mic::types::MatrixXf> decoded (new mic::types::MatrixXf(sdr_->cols(), sdr_->rows()));
	// Copy data.
	(*decoded) = (*sdr_);
	// Resize.
	decoded->resize(matrix_height, matrix_width);
	return decoded;
}



} // namespace encoders
} // namespace mic
