/*!
 * \file DummyGrayscaleImageEncoder.cpp
 * \brief Contains definitions of methods for encoding grayscale images into float-based SDRs and vice versa.
 * \author tkornuta
 * \date Nov 16, 2015
 */


#include <logger/Log.hpp>

#include <algorithm>    // std::min

#include <auto_encoders/DummyGrayscaleImageEncoder.hpp>

namespace mic {
namespace auto_encoders {

using namespace mic::types;

//using namespace mic::system_utils;


void DummyGrayscaleImageEncoder::encode(const image& input_, floatSDR& sdr_){
	// Validate if the image contains any data.
	if (input_.image_data && input_.width > 0 && input_.height > 0) {

		// Check if SDR is capable of encoding the image.
		if ((input_.width * input_.height) <= DEFAULT_SDR_LENGTH) {

			// Check input type - now only grayscale images are encoded.
			if (input_.type == GRAYSCALE)
			{
				// Iterate through image pixels.
				for (size_t row = 0; row < input_.height; row++) {
					for (size_t col = 0; col < input_.width; col++) {
						sdr_[row * input_.width + col] = ((float*)input_.image_data)[row * input_.width + col];
					}//: for
				}//: for

			} else {
				// Inform that it was not a grayscale image.
				CHECK(input_.type == GRAYSCALE);
			}//: else
		} else {
			CHECK((input_.width * input_.height) <= DEFAULT_SDR_LENGTH);
		}//: else
	} else {
		// Inform that there were some problems with image/data.
		CHECK(input_.image_data != NULL);
	}//: else
}


void DummyGrayscaleImageEncoder::decode(image& output_, const floatSDR& sdr_) {
	// Validate if the image has assigned memory for data.
	if (output_.image_data) {

		// Calculate the number of decoded elements.
		size_t decode_size = std::min((size_t)DEFAULT_SDR_LENGTH, (size_t)(output_.width * output_.height));
		// TODO: And what about the elements od SDR that exceed output_.width * output_.height?

		if (output_.type == GRAYSCALE) {
			// Decode consecutive SDR elements.
			for (unsigned i = 0; i < decode_size; i++) {
				unsigned row = i / output_.width;
				unsigned col = i % output_.width;

				// Truncate values.
				if (sdr_[i] < 0.0f)
					((float*)output_.image_data)[row * output_.width + col] = 0.0f;
				else if (sdr_[i] > 1.0f)
					((float*)output_.image_data)[row * output_.width + col] = 1.0f;
				else
					((float*)output_.image_data)[row * output_.width + col] = sdr_[i];
			}//: for
		} else {
			// Inform that it was not a grayscale image.
			CHECK(output_.type == GRAYSCALE);
		}//: else

	} else {
		// Inform that there were some problems with image/data.
		CHECK(output_.image_data != NULL);
	}//: else


}



} // namespace auto_encoders
} // namespace mic
