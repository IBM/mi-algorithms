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
 * \file SDR.hpp
 * \brief Contains definition of SDR type.
 * \author: tkornuta
 * \date Nov 16, 2015
 */

#ifndef SRC_TYPES_SDR_HPP_
#define SRC_TYPES_SDR_HPP_

/**
 * \namespace mic
 * \brief Namespace containing machine intelligence core.
 * \author tkornuta
 */
namespace mic {

/**
 * \namespace mic::types
 * \brief Namespace containing main data types and enums of machine intelligence core.
 * \author tkornuta
 */
namespace types {

/**
 * \brief Type denoting float SDR - Sparse Distributed Representation.
 *
 * A table of floats of DEFAULT_SDR_LENGTH (the length is defined as CMAKE variable).
 * \author tkornuta
 */
typedef float floatSDR [DEFAULT_SDR_LENGTH];

/**
 * \brief Type denoting bool SDR - Sparse Distributed Representation.
 *
 * A table of boolean values of DEFAULT_SDR_LENGTH (the length is defined as CMAKE variable).
 * \author tkornuta
 */
typedef bool boolSDR [DEFAULT_SDR_LENGTH];

}//: namespace types
}//: namespace mic


#endif /* SRC_TYPES_SDR_HPP_ */
