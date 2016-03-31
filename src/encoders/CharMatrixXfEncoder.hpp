/*!
 * \file CharMatrixXfEncoder.hpp
 * \brief 
 * \author tkornut
 * \date Dec 22, 2015
 */

#ifndef SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_
#define SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_

#include <encoders/MatrixXfSDREncoder.hpp>

namespace mic {
namespace encoders {

/*!
 * \brief Encoder responsible for encoding character into Matrix SDRs.
 * A 1-of-k encoder, i.e. it simply encodes char to ASCII and turns on that bit, i.e. there is no learning.
 * \author tkornuta
 */
class CharMatrixXfEncoder: public mic::encoders::MatrixXfSDREncoder<char> {
public:
	/// Default constructor - empty.
	CharMatrixXfEncoder(size_t sdr_length_) : MatrixXfSDREncoder(sdr_length_) {
		std::cout<<" Hello CharMatrixXfEncoder\n";
	};

	/// Default destructor - empty.
	~CharMatrixXfEncoder() { }

	/*!
	 * @brief Method responsible for encoding of input data sample into SDR (a vector of floats).
	 * @param[in] sample_ Shared pointer to a char.
	 * @return Shared pointer to SDR - here in the form of 1d matrix (cols,1) of floats. Memory to this variable must be assigned earlier.
	 */
	virtual std::shared_ptr<mic::types::MatrixXf> encodeSample(const std::shared_ptr<char>& sample_);

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[in] sdr_ Shared pointer to SDR.
	 * @return Shared pointer to a char
	 */
	virtual std::shared_ptr<char> decodeSample(const std::shared_ptr<mic::types::MatrixXf>& sdr_);
};

} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_AUTO_ENCODERS_DUMMYCHARENCODER_HPP_ */
