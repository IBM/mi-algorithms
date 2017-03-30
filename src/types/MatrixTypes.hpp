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
 * \brief Macro for initialization of values of vector pointer.
 * \author tkornuta
 */
#define MAKE_MATRIX_PTR(eT, ...) std::make_shared<mic::types::Matrix<eT> >( mic::types::Matrix<eT> (__VA_ARGS__))


#endif /* SRC_TYPES_MATRIXTYPES_HPP_ */
