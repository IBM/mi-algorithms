/*!
 * \file UIntMatrixEncoder.hpp
 * \brief 
 * \author tkornut
 * \date Apr 15, 2016
 */

#ifndef SRC_ENCODERS_UINTMATRIXENCODER_HPP_
#define SRC_ENCODERS_UINTMATRIXENCODER_HPP_

#include <encoders/MatrixSDREncoder.hpp>

#include <logger/Log.hpp>

#include <cstring> // for memset
#ifdef _WIN32
#include <string.h>
#endif

namespace mic {
namespace encoders {

/*!
 * \brief Encoder responsible for encoding unsigned integers into Matrix SDRs.
 * A 1-of-k encoder, i.e. it simply turns on the adequate, i.e. there is no learning.
 * \author tkornuta
 */

template<typename T = float>
class UIntMatrixEncoder: public mic::encoders::MatrixSDREncoder<unsigned int, T> {
public:
	/*!
	 * Default constructor.
	 * @param sdr_length_ SDR length.
	 */
    UIntMatrixEncoder(size_t sdr_length_) : MatrixSDREncoder<unsigned int, T>(sdr_length_) {
//		std::cout<<" Hello CharMatrixXfEncoder\n";
	};

	/// Default destructor - empty.
    ~UIntMatrixEncoder() { }

	/*!
	 * @brief Method responsible for encoding of input data sample into SDR (a vector of floats).
	 * @param[in] sample_ Shared pointer to unsigned int.
	 * @return Shared pointer to SDR - here in the form of 1d matrix (cols,1) of floats. Memory to this variable must be assigned earlier.
	 */
    virtual mic::types::MatrixPtr<T> encodeSample(const std::shared_ptr<unsigned int>& sample_){
        // Create returned SDR - a vector, i.e. a matrix with number of columns equal to 1.
        mic::types::MatrixPtr<T> sdr (new mic::types::Matrix<T>(sdr_length,1));
        // Set all zeros.
        sdr->setZero();

        unsigned int index = (*sample_);

        if (index >= sdr_length)
            LOG(LERROR) << "The SDR is too short for proper encoding of "<<index<<"!";
        else
            (*sdr)(index) = 1;
        return sdr;
    }

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[in] sdr_ Shared pointer to SDR.
	 * @return Shared pointer to unsigned int.
	 */
    virtual std::shared_ptr<unsigned int> decodeSample(const mic::types::MatrixPtr<T>& sdr_){
        // SDR must be in fact a vector, with number of columns equal to 1.
        assert(sdr_->cols() == 1);

        // Find index of max value.
        typename mic::types::Matrix<T>::Index maxRow, maxCol;
        float max_value = sdr_->maxCoeff(&maxRow, &maxCol);
        LOG(LDEBUG) << "(maxRow, maxCol) = max_value [ (" << maxRow << "," << maxCol <<") = " << max_value << "]";

        // Convert to int.
        unsigned int decoded = maxRow;

        return std::make_shared<unsigned int>(decoded);
    }

private:
    using MatrixSDREncoder<unsigned int, T>::sdr_length;
};


} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_ENCODERS_UINTMATRIXENCODER_HPP_ */
