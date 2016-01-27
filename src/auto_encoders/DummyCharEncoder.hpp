/*!
 * \file DummyCharEncoder.hpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#ifndef SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_
#define SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_

#include <auto_encoders/Encoder.hpp>
#include <types/SDR.hpp>

namespace mic {
namespace auto_encoders {

/*!
 * \brief Encoder responsible for encoding character into SDRs (1-of-k encoder).
 * Dummy - because it simply encodes char to ASCII and tunrs on that bit, i.e. there is no learning (auto-encoding).
 * \author tkornuta
 */
class DummyCharEncoder: public Encoder<char, mic::types::floatSDR> {
public:
	/// Default constructor - empty.
	DummyCharEncoder() : Encoder() { }

	/// Default destructor - empty.
	~DummyCharEncoder() { }


	/*!
	 * @brief Method responsible for encoding input data into SDR (a table of floats).
	 * @param[in] input_ Input data - a character.
	 * @param[out] sdr_ Output SDR - here in the form of table of floats. Memory to this variable must be assigned earlier.
	 * @param[in] size_ Size of output SDR. As default set to DEFAULT_SDR_LENGTH.
	 */
	virtual void encode(const char& input_, mic::types::floatSDR& sdr_, const size_t size_ = DEFAULT_SDR_LENGTH);

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[out] output_ Output data - a character.
	 * @param[in] sdr_ Input SDR - here in the form of table of floats.
	 * @param[in] size_ Size of SDR. As default set to DEFAULT_SDR_LENGTH.
	 */
	virtual void decode(char& output_, const mic::types::floatSDR& sdr_, const size_t size_ = DEFAULT_SDR_LENGTH);

};

} /* namespace auto_encoders */
} /* namespace mic */

#endif /* SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_ */
