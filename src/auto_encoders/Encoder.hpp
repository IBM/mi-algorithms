/*!
 * \file Encoder.hpp
 * \brief Contains definition of an abstract, template parent class for all auto-encoders.
 * \author tkornuta
 * \date Nov 16, 2015
 */

#ifndef __ENCODER_HPP__
#define __ENCODER_HPP__

#include <string>
#include <vector>
#include <iostream>

namespace mic {

/*!
 * \brief Contains auto-encoder parent class and all derived classes.
 * \author tkornuta
 */
namespace auto_encoders {

/*!
 * @brief Parent class for all auto-encoders.
 *
 * @author tkornuta
 * @tparam inputDataType Template parameter defining the input datatype.
 * @tparam SDRType Template parameter defining the SDR type.
 */
template <typename inputDataType, typename SDRType>
class Encoder {

public:

	/*!
	 * @brief Constructor - empty.
	 */
	Encoder() { };

	/*!
	 * Virtual destructor - empty.
	 */
	virtual ~Encoder() { };

	/*!
	 * @brief Method responsible for encoding input data into SDR (a table of floats).
	 * @param[in] input_ Input data.
	 * @param[out] sdr_ Output SDR.
	 * @param[in] size_ Size of output SDR. As default set to DEFAULT_SDR_LENGTH.
	 */
	virtual void encode(const inputDataType& input_, SDRType& sdr_, const size_t size_ = DEFAULT_SDR_LENGTH) = 0;

	/*!
	 * Method responsible for decoding of SDR into data.
	 * @param[out] output_ Output data.
	 * @param[in] sdr_ Input SDR.
	 * @param[in] size_ Size of output SDR. As default set to DEFAULT_SDR_LENGTH.
	 */
	virtual void decode(inputDataType& output_, const SDRType& sdr_, const size_t size_ = DEFAULT_SDR_LENGTH) = 0;

};



} // namespace auto_encoders
} // namespace mic

#endif
