/*!
 * \file ColMatrixEncoder.hpp
 * \brief 
 * \author Alexis-Asseman
 * \date June 12, 2017
 */

#ifndef SRC_AUTO_ENCODERS_SINGLECHANNELMATRIXBATCHENCODER_H_
#define SRC_AUTO_ENCODERS_SINGLECHANNELMATRIXBATCHENCODER_H_

#include <encoders/ColMatrixEncoder.hpp>

namespace mic {
namespace encoders {

/*!
 * \brief (Legacy compatibility) Wrapper for ColMatrixEncoder<float>.
 * \author Alexis-Asseman
 */

typedef ColMatrixEncoder<float> MatrixXfMatrixXfEncoder;

} /* namespace encoders */
} /* namespace mic */

#endif /* SRC_AUTO_ENCODERS_SINGLECHANNELMATRIXBATCHENCODER_H_ */
