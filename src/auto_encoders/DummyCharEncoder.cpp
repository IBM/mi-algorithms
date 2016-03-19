/*!
 * \file DummyCharEncoder.cpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#include <auto_encoders/DummyCharEncoder.hpp>
#include <cstring> // for memset 
#ifdef _WIN32
#include <string.h>
#endif

namespace mic {
namespace auto_encoders {

void DummyCharEncoder::encode(const char& input_, mic::types::floatSDR& sdr_, const size_t size_) {
	// Set all zeros.
	memset( &sdr_, 0, sizeof( mic::types::floatSDR ) );
	// Convert ASCII char to int.
	int a = input_ - 32;
	sdr_[a] = 1;
}

void DummyCharEncoder::decode(char& output_, const mic::types::floatSDR& sdr_, const size_t size_) {
	// Find index of max value.
	int max =-1;
	int index = -1;
	for(size_t i=0; i< size_; i++)
		if (sdr_[i] > max) {
			max = sdr_[i];
			index = i;
		}//: if
	// Convert int to ASCII.
	output_ = index + 32;
}


} /* namespace auto_encoders */
} /* namespace mic */
