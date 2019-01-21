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
 * \file MatrixXdSpecializations.hpp
 * \brief 
 * \author tkornut
 * \date Mar 7, 2016
 */

#ifndef SRC_TYPES_MATRIXXDSPECIALIZATIONS_HPP_
#define SRC_TYPES_MATRIXXDSPECIALIZATIONS_HPP_

#include <types/Matrix.hpp>

#ifdef OpenBLAS_FOUND
#include <cblas.h>
#endif

namespace mic {
namespace types {

/*!
 * \brief Template specialization: overloaded matrix multiplication operator - for doubles. Uses OpenBLAS if found by CMAKE.
 * \author tkornuta
 */
template<>
inline Eigen::MatrixXd mic::types::Matrix<double>::operator *(const Eigen::MatrixXd & mat_) {

#ifdef OpenBLAS_FOUND
	// Using openBLAS boost!
	//printf("Using specialized (double) BLAS boost!\n");
	// Get dimensions.
	size_t M = this->rows();
	size_t K = this->cols();
	size_t N = mat_.cols();
	// Create temporary matrix.
	Eigen::MatrixXd c(M,N);

	cblas_dgemm( CblasColMajor, CblasNoTrans, CblasNoTrans, M, N, K, 1.0,
			this->data(), M,
			mat_.data(), K, 0.0, c.data(), M );
	return c;
#else
	// Calling base EIGEN operator *
	//printf("Calling specialized (double) base EIGEN operator *\n");
	return Eigen::MatrixXd::operator*(mat_);
#endif
}


/*!
 * \brief Vector of double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::VectorXd VectorXd;


/*!
 * \brief Matrix of double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef mic::types::Matrix<double> MatrixXd;


/*!
 * \brief Shared pointer to matrix of double precision floats (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::MatrixXd > MatrixXdPtr;


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



#endif /* SRC_TYPES_MATRIXXDSPECIALIZATIONS_HPP_ */
