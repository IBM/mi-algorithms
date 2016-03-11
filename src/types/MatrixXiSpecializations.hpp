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
