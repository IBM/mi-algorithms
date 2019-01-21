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
