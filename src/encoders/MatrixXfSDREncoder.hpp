/*!
 * \file MatrixXfSDREncoder.hpp
 * \brief 
 * \author tkornut
 * \date Mar 30, 2016
 */

#ifndef SRC_ENCODERS_MATRIXXFSDRENCODER_HPP_
#define SRC_ENCODERS_MATRIXXFSDRENCODER_HPP_

#include <encoders/Encoder.hpp>

#include <types/MatrixTypes.hpp>

namespace mic {
namespace encoders {

/*!
 * @brief Abstract parent class for all encoders using MatrixXf as SDR datatype.
 *
 * @author tkornuta
 * @tparam inputDataType Template parameter defining the input (sample) datatype.
 * @tparam SDRType Template parameter defining the SDR type.
 */
template <typename inputDataType>
class MatrixXfSDREncoder : public mic::encoders::Encoder <inputDataType, mic::types::MatrixXf> {
public:
	/*!
	 * @brief Constructor.
	 * @param sdr_length_ SDR length.
	 */
	MatrixXfSDREncoder(size_t sdr_length_) : mic::encoders::Encoder<inputDataType, mic::types::MatrixXf>(),
		sdr_length(sdr_length_)
	{
//		std::cout<<" Hello MatrixXfSDREncoder\n";
	};

	/*!
	 * Virtual destructor - empty.
	 */
	virtual ~MatrixXfSDREncoder() { }


	/*!
	 * @brief Method responsible for encoding input sample into SDR.
	 * @param[in] sample_ Shared pointer to sample data.
	 * @return Shared pointer to encoded SDR.
	 */
	virtual std::shared_ptr<mic::types::MatrixXf> encodeSample(const std::shared_ptr<inputDataType>& sample_) = 0;

	/*!
	 * Method responsible for encoding batch containing several samples into matrix containing several SDRs.
	 * @param[in] batch_ Vector of shared pointers containing samples
	 * @return Matrix containing several SDRs.
	 */
	virtual std::shared_ptr<mic::types::MatrixXf> encodeBatch(const std::vector<std::shared_ptr<inputDataType> >& batch_) {
		// Create returned matrix.
		std::shared_ptr<mic::types::MatrixXf> sdrs (new mic::types::MatrixXf(sdr_length, batch_.size()));

		// Encode the samples one by one.
		for (size_t i=0; i < batch_.size(); i++ ) {
			// Encode single sample.
			std::shared_ptr<mic::types::MatrixXf> sample_sdr = encodeSample(batch_[i]);

			// Set SDR rows.
			sdrs->col(i) = sample_sdr->col(0);
		}//: for

		// Return the matrix containing SDRs.
		return sdrs;
	}

	/*!
	 * Method responsible for decoding of SDR back into original data type.
	 * @param[in] sdr_ Shared pointer to SDR.
	 * @return Shared pointer to decoded data.
	 */
	virtual std::shared_ptr<inputDataType> decodeSample(const std::shared_ptr<mic::types::MatrixXf>& sdr_) = 0;

	/*!
	 * Method responsible for encoding batch containing several samples into matrix containing several SDRs.
	 * @param[in] batch_ Vector of shared pointers containing samples
	 * @return Matrix containins several SDRs.
	 */
	virtual std::vector<std::shared_ptr<inputDataType> > decodeBatch(const std::shared_ptr<mic::types::MatrixXf>& sdrs_) {
		// Create the output vector.
		std::vector<std::shared_ptr<inputDataType> > output;
		std::shared_ptr<mic::types::MatrixXf> sample_sdr (new mic::types::MatrixXf(sdrs_->rows(), 1));

		// Iterate through columns and decode them one by one.
		for (size_t i=0; i < sdrs_->cols(); i++ ) {
			sample_sdr->col(0) = sdrs_->col(i);
			std::shared_ptr<inputDataType> decoded = decodeSample(sample_sdr);
			// Add result to vector.
			output.push_back(decoded);
		}//: for

		// Return the decoded vector.
		return output;
	}


	/*!
	 * Returns the SDR length.
	 */
	size_t getSDRLength() {
		return sdr_length;
	}

	/*!
	 * Sets SDR length.
	 * @param sdr_length_ SDR length.
	 */
	void setSDRLength(size_t sdr_length_) {
		sdr_length = sdr_length_;
	}

protected:

	/// Length of SDR.
	size_t sdr_length;
};

} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_ENCODERS_MATRIXXFSDRENCODER_HPP_ */
