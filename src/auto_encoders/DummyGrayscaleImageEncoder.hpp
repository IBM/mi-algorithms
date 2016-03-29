/*!
 * \file DummyGrayscaleImageEncoder.hpp
 * \brief Contains declaration of encoder responsible for coding grayscale images into float-based SDRs.
 * \author tkornuta
 * \date Nov 16, 2015
 */

#ifndef __GRAYSCALE_IMAGE_ENCODER_HPP__
#define __GRAYSCALE_IMAGE_ENCODER_HPP__



#include <auto_encoders/Encoder.hpp>
#include <types/image_types.hpp>
#include <types/SDR.hpp>

namespace mic {
namespace auto_encoders {


/*!
 * \brief Encoder responsible for encoding images into SDRs.
 * It is called dummy because there is no learning (auto-encoding). Instead it simply transforms floats from matrix (grayscale image) to vector (SDR).
 * \author tkornuta
 */
class DummyGrayscaleImageEncoder : public Encoder<mic::types::image, mic::types::floatSDR> {

public:

	/// Default constructor - empty.
	DummyGrayscaleImageEncoder() : Encoder() { }

	/// Default destructor - empty.
	~DummyGrayscaleImageEncoder() { }

	/*!
	 * @brief Method responsible for encoding input data into SDR (a table of floats).
	 * @param[in] input_ Input data - grayscale image.
	 * @param[out] sdr_ Output SDR - here in the form of table of floats. Memory to this variable must be assigned earlier.
	 */
	virtual void encode(const mic::types::image& input_, mic::types::floatSDR& sdr_);

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[out] output_ Output data - a grayscale image. Memory to image data must be assigned earlier.
	 * @param[in] sdr_ Input SDR - here in the form of table of floats.
	 */
	virtual void decode(mic::types::image& output_, const mic::types::floatSDR& sdr_);
	
};


} // namespace auto_encoders
} // namespace mic

#endif
