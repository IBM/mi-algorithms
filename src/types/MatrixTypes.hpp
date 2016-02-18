/*!
 * \file MatrixTypes.hpp
 * \brief Contains definition of basic matrix datatypes derived from Eigen.
 * \author tkornut
 * \date Feb 5, 2016
 */

#ifndef SRC_TYPES_MATRIXTYPES_HPP_
#define SRC_TYPES_MATRIXTYPES_HPP_

#include <Eigen/Dense>

#include <types/MatrixXf.hpp>

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
typedef Eigen::MatrixXi MatrixXi;

/*!
 * \brief Shared pointer to matrix of integers (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< Eigen::MatrixXi > MatrixXiPtr;

/*!
 * \brief The <char-char> pair type used by e.g. RawTextImporter.
 * \author tkornuta
 */
typedef std::pair< std::shared_ptr<char>, std::shared_ptr<char> > char_char_pair_t;



/*!
 * \brief Vector of double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::VectorXd VectorXd;


/*!
 * \brief Matrix of double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::MatrixXd MatrixXd;


/*!
 * \brief Shared pointer to matrix of double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< MatrixXd > MatrixXdPtr;


/*!
 * \brief The <matrix-char> pair type used by e.g. IBMFontMatrixImporter.
 * \author tkornuta
 */
typedef std::pair< MatrixXdPtr, std::shared_ptr<char> > MatrixXdCharPair;

/*!
 * \brief The <matrix-unsigned int> pair type used by e.g. MNISTMatrixImporter.
 * \author tkornuta
 */
typedef std::pair< MatrixXdPtr, std::shared_ptr<unsigned int> > MatrixXdUintPair;




}//: namespace types
}//: namespace mic



#endif /* SRC_TYPES_MATRIXTYPES_HPP_ */
