/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
