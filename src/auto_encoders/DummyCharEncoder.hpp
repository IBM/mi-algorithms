/*!
 * \file DummyCharEncoder.hpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#ifndef SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_
#define SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_

#include <auto_encoders/Encoder.hpp>
#include <types/MatrixTypes.hpp>

namespace mic {
namespace auto_encoders {

/*!
 * \brief Encoder responsible for encoding character into SDRs (1-of-k encoder).
 * Dummy - because it simply encodes char to ASCII and turns on that bit, i.e. there is no learning (auto-encoding).
 * \author tkornuta
 */
class DummyCharEncoder: public Encoder<char, mic::types::MatrixXf> {
public:
	/// Default constructor - empty.
	DummyCharEncoder() : Encoder() { }

	/// Default destructor - empty.
	~DummyCharEncoder() { }

	/*!
	 * @brief Method responsible for encoding of input data sample into SDR (a vector of floats).
	 * @param[in] input_ Data sample - a character.
	 * @param[out] sdr_ Output SDR - here in the form of matrix of floats. Memory to this variable must be assigned earlier.
	 */
	void encode(const char& input_sample_, mic::types::MatrixXf& sdr_);

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[out] output_ Output data - a character.
	 * @param[in] sdr_ Input SDR - here in the form of matrix of floats. Memory to this variable must be assigned earlier.
	 */
	void decode(char& output_, const mic::types::MatrixXf& sdr_);

	/*!
	 * @brief Method responsible for encoding of input batch into SDR (a matrix of floats).
	 * @param[in] input_ Input data - a vector of characters.
	 * @param[out] sdr_ Output SDR - here in the form of matrix of floats. Memory to this variable must be assigned earlier.
	 */
	virtual void encode(const std::vector<std::shared_ptr<char> >& input_batch_, mic::types::MatrixXf& sdr_);

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[out] output_ Output data - a vector of characters.
	 * @param[in] sdr_ Input SDR - here in the form of matrix of floats. Memory to this variable must be assigned earlier.
	 */
	virtual void decode(std::vector<std::shared_ptr<char> >& output_, const mic::types::MatrixXf& sdr_);


};

} /* namespace auto_encoders */
} /* namespace mic */

#endif /* SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_ */
