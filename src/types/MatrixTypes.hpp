/*!
 * \file MatrixTypes.hpp
 * \brief Contains definition of basic matrix datatypes derived from Eigen.
 * \author tkornut
 * \date Feb 5, 2016
 */

#ifndef SRC_TYPES_MATRIXTYPES_HPP_
#define SRC_TYPES_MATRIXTYPES_HPP_

#include <Eigen/Dense>

namespace mic {
namespace types {

/*!
 * \brief Vector with integers (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::VectorXi vectori_t;


/*!
 * \brief Matrix with integers (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::MatrixXi matrixi_t;

/*!
 * \brief Shared pointer to matrix with integers (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< Eigen::MatrixXi > matrixi_ptr_t;


/*!
 * \brief Vector with double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::VectorXd vectord_t;


/*!
 * \brief Matrix with double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::MatrixXd matrixd_t;


/*!
 * \brief Shared pointer to matrix with double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< Eigen::MatrixXd > matrixd_ptr_t;


/*!
 * \brief The <matrix-char> pair type used by e.g. IBMFontMatrixImporter.
 * \author tkornuta
 */
typedef std::pair< matrixd_ptr_t, std::shared_ptr<char> > matrixd_char_pair_t;

/*!
 * \brief The <matrix-unsigned int> pair type used by e.g. MNISTMatrixImporter.
 * \author tkornuta
 */
typedef std::pair< matrixd_ptr_t, std::shared_ptr<unsigned int> > matrixd_uint_pair_t;


/*!
 * \brief The <char-char> pair type used by e.g. RawTextImporter.
 * \author tkornuta
 */
typedef std::pair< std::shared_ptr<char>, std::shared_ptr<char> > char_char_pair_t;


}//: namespace types
}//: namespace mic



#endif /* SRC_TYPES_MATRIXTYPES_HPP_ */
