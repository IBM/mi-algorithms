/*!
 * \file UIntMatrixXfEncoder.hpp
 * \brief 
 * \author tkornut
 * \date Apr 15, 2016
 */

#ifndef SRC_ENCODERS_UINTMATRIXXFENCODER_HPP_
#define SRC_ENCODERS_UINTMATRIXXFENCODER_HPP_

#include <encoders/MatrixXfSDREncoder.hpp>

namespace mic {
namespace encoders {

/*!
 * \brief Encoder responsible for encoding unsigned integers into Matrix SDRs.
 * A 1-of-k encoder, i.e. it simply turns on the adequate, i.e. there is no learning.
 * \author tkornuta
 */
class UIntMatrixXfEncoder: public mic::encoders::MatrixXfSDREncoder<unsigned int> {
public:
	/*!
	 * Default constructor.
	 * @param sdr_length_ SDR length.
	 */
	UIntMatrixXfEncoder(size_t sdr_length_) : MatrixXfSDREncoder(sdr_length_) {
//		std::cout<<" Hello CharMatrixXfEncoder\n";
	};

	/// Default destructor - empty.
	~UIntMatrixXfEncoder() { }

	/*!
	 * @brief Method responsible for encoding of input data sample into SDR (a vector of floats).
	 * @param[in] sample_ Shared pointer to unsigned int.
	 * @return Shared pointer to SDR - here in the form of 1d matrix (cols,1) of floats. Memory to this variable must be assigned earlier.
	 */
	virtual mic::types::MatrixXfPtr encodeSample(const std::shared_ptr<unsigned int>& sample_);

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[in] sdr_ Shared pointer to SDR.
	 * @return Shared pointer to unsigned int.
	 */
	virtual std::shared_ptr<unsigned int> decodeSample(const mic::types::MatrixXfPtr& sdr_);
};


} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_ENCODERS_UINTMATRIXXFENCODER_HPP_ */
