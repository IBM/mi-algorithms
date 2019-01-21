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
 * \file MatrixXiSpecializations.hpp
 * \brief 
 * \author tkornut
 * \date Mar 7, 2016
 */

#ifndef SRC_TYPES_MATRIXXISPECIALIZATIONS_HPP_
#define SRC_TYPES_MATRIXXISPECIALIZATIONS_HPP_

#include <types/Matrix.hpp>

namespace mic {
namespace types {

/*!
 * \brief Vector of integers (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::VectorXi VectorXi;


/*!
 * \brief Shared pointer of vector with integers (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< Eigen::VectorXi >VectorXiPtr;


/*!
 * \brief Matrix of integers (of dynamic size).
 * \author tkornuta
 */
typedef mic::types::Matrix<int> MatrixXi;

/*!
 * \brief Shared pointer to matrix of integers (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::MatrixXi > MatrixXiPtr;


}//: namespace types
}//: namespace mic


#endif /* SRC_TYPES_MATRIXXISPECIALIZATIONS_HPP_ */
