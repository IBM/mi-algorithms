/*!
 * \file MatrixXfSDREncoder.hpp
 * \brief 
 * \author Alexis-Asseman
 * \date June 12, 2017
 */

#ifndef SRC_ENCODERS_MATRIXXFSDRENCODER_HPP_
#define SRC_ENCODERS_MATRIXXFSDRENCODER_HPP_

#include <encoders/MatrixSDREncoder.hpp>

namespace mic {
namespace encoders {

/*!
 * @brief (Legacy compatibility) Wrapper for MatrixSDREncoder<inputDataType, float>
 *
 * @author Alexis-Asseman
 * @tparam inputDataType Template parameter defining the input (sample) datatype.
 * @tparam SDRType Template parameter defining the SDR type.
 */

template <typename inputDataType = float>
using MatrixXfSDREncoder = MatrixSDREncoder<inputDataType, float>;


} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_ENCODERS_MATRIXXFSDRENCODER_HPP_ */
