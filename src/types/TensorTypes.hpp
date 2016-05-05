/*!
 * \file TensorTypes.hpp
 * \brief Contains declaration of tensor types.
 * \author tkornut
 * \date Mar 17, 2016
 */

#ifndef SRC_TYPES_TENSORTYPES_HPP_
#define SRC_TYPES_TENSORTYPES_HPP_

#include <types/Tensor.hpp>

namespace mic {
namespace types {


/*!
 * \brief Tensor of integers (of dynamic size).
 * \author tkornuta
 */
typedef mic::types::Tensor<int> TensorXi;


/*!
 * \brief Shared pointer to tensor of integers (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::TensorXi > TensorXiPtr;


/*!
 * \brief Tensor of single precision floats (of dynamic size).
 * \author tkornuta
 */
typedef mic::types::Tensor<float> TensorXf;


/*!
 * \brief Shared pointer to tensor of single precision floats (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::TensorXf > TensorXfPtr;


/*!
 * \brief Tensor of double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef mic::types::Tensor<double> TensorXd;


/*!
 * \brief Shared pointer to tensor of double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::TensorXd > TensorXdPtr;

} //: namespace types
} //: namespace mic

#endif /* SRC_TYPES_TENSORTYPES_HPP_ */
