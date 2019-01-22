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
 * \file MatrixTypes.hpp
 * \brief Contains definition of basic matrix datatypes derived from Eigen.
 * \author tkornut
 * \date Feb 5, 2016
 */

#ifndef SRC_TYPES_MATRIXTYPES_HPP_
#define SRC_TYPES_MATRIXTYPES_HPP_

#include <types/MatrixXdSpecializations.hpp>

#include <types/MatrixXfSpecializations.hpp>

#include <types/MatrixXiSpecializations.hpp>

/*!
 * \brief Macro for initialization of matrix pointer.
 * \author tkornuta
 */
#define MAKE_MATRIX_PTR(eT, ...) std::make_shared<mic::types::Matrix<eT> >( mic::types::Matrix<eT> (__VA_ARGS__))


#endif /* SRC_TYPES_MATRIXTYPES_HPP_ */
