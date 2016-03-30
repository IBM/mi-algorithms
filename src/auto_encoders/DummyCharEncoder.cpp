/*!
 * \file DummyCharEncoder.cpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#include <auto_encoders/DummyCharEncoder.hpp>

#include <logger/Log.hpp>

#include <cstring> // for memset 
#ifdef _WIN32
#include <string.h>
#endif

namespace mic {
namespace auto_encoders {

void DummyCharEncoder::encode(const char& input_sample_, mic::types::MatrixXf& sdr_) {
	// SDR must be in fact a vector, with number of columns equal to 1.
	assert(sdr_.cols() == 1);
	// Set all zeros.
	sdr_.setZero();
	// Convert ASCII char to int.
	int a = input_sample_ - 32;
	//std::cout <<" input_ = " << input_ <<" a = " << a;
	if (a<0)
		LOG(LERROR) << "Could not properly encode character '" <<input_sample_ << "' ("<<a<<")!";
	else if (a>=sdr_.rows())
		LOG(LERROR) << "The SDR is too short for proper encoding of the character '" <<input_sample_ << "' ("<<a<<")!";
	else
		sdr_(a) = 1;
}

void DummyCharEncoder::decode(char& output_, const mic::types::MatrixXf& sdr_) {
	// SDR must be in fact a vector, with number of columns equal to 1.
	assert(sdr_.cols() == 1);
	// Find index of max value.
	int max =-1;
	int index = -1;
	size_t row, col;
	sdr_.maxCoeff();//col, row);
	for(size_t i=0; i< sdr_.rows(); i++){
		if (sdr_(i) > max) {
			max = sdr_(i);
			index = i;
		}//: if
	}//: for
	// Convert int to ASCII.
	output_ = index + 32;
}


void DummyCharEncoder::encode(const std::vector<std::shared_ptr<char> >& input_batch_, mic::types::MatrixXf& sdr_) {
	// Number of SDR columns must be equal to size of vector.
	assert(sdr_.cols() == input_batch_.size());

	// Encode the samples one by one.
	mic::types::MatrixXf sample_sdr(sdr_.rows(), 1);
	for (size_t i=0; i < input_batch_.size(); i++ ) {
		// Encode single sample.
		encode(*(input_batch_[i]), sample_sdr);
		// Set SDR rows.
		sdr_.col(i) = sample_sdr.col(0);
	}//: for
}

void DummyCharEncoder::decode(std::vector<std::shared_ptr<char> >& output_, const mic::types::MatrixXf& sdr_) {
	// Clear the output vector.
	output_.clear();
	char tmp;
	mic::types::MatrixXf sample_sdr(sdr_.rows(), 1);
	// Iterate through columns and decode them one by one.
	for (size_t i=0; i < sdr_.cols(); i++ ) {
		sample_sdr.col(0) = sdr_.col(i);
		decode(tmp, sample_sdr);
		// Add result to vector.
		output_.push_back(std::make_shared<char>(tmp));
	}//: for


}


} /* namespace auto_encoders */
} /* namespace mic */
