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
