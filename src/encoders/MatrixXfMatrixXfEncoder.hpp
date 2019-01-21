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
