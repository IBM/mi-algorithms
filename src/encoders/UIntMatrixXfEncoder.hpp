/*!
 * \file UIntMatrixXfEncoder.hpp
 * \brief 
 * \author Alexis-Asseman
 * \date June 12, 2017
 */

#ifndef SRC_ENCODERS_UINTMATRIXXFENCODER_HPP_
#define SRC_ENCODERS_UINTMATRIXXFENCODER_HPP_

#include <encoders/UIntMatrixEncoder.hpp>

namespace mic {
namespace encoders {

/*!
 * \brief (Legacy Compatibility) Wrapper for MatrixSDREncoder<float>
 * \author Alexis-Asseman
 */

typedef UIntMatrixEncoder<float> UIntMatrixXfEncoder;

} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_ENCODERS_UINTMATRIXXFENCODER_HPP_ */
