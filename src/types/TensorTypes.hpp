/*!
 * \file TensorTypes.hpp
 * \brief Contains declaration of tensor types.
 * \author tkornut
 * \date Mar 17, 2016
 */

#ifndef SRC_TYPES_TENSORTYPES_HPP_
#define SRC_TYPES_TENSORTYPES_HPP_

#include <types/Tensor.hpp>

#include <types/Batch.hpp>

namespace mic {
namespace types {


/*!
 * \brief Typedef for a shared pointer to template-typed dynamic matrices.
 * \author tkornuta
 */
template<typename eT>
using TensorPtr = typename std::shared_ptr< mic::types::Tensor<eT> >;

/*!
 * \brief Macro for initialization of tensor pointer.
 * \author tkornuta
 */
#define MAKE_TENSOR_PTR(eT, ...) std::make_shared<mic::types::Tensor<eT> >( mic::types::Tensor<eT> ( { __VA_ARGS__ } ))

/*!
 * \brief The <Tensor-uint> sample type.
 * \author tkornuta
 */
template<typename eT>
using TensorSample = typename mic::types::Sample<mic::types::Tensor<eT>, unsigned int>;

/*!
 * \brief Pointer to the <Tensor-uint> sample type.
 * \author tkornuta
 */
template<typename eT>
using TensorSamplePtr = typename std::shared_ptr<mic::types::TensorSample<eT> >;


/*!
 * \brief The <Tensor-uint> batch type.
 * \author tkornuta
 */
template<typename eT>
using TensorBatch = typename mic::types::Batch<mic::types::Tensor<eT>, unsigned int>;


/*!
 * \brief Pointer to the <Tensor-uint> batch type.
 * \author tkornuta
 */
template<typename eT>
using TensorBatchPtr = typename std::shared_ptr<mic::types::TensorBatch<eT> > ;


///////////////////// SPACIALIZATIONS /////////////////////


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
