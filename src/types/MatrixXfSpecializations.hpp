/*!
 * \file MatrixXfSpecializations.hpp
 * \brief 
 * \author tkornut
 * \date Mar 7, 2016
 */

#ifndef SRC_TYPES_MATRIXXFSPECIALIZATIONS_HPP_
#define SRC_TYPES_MATRIXXFSPECIALIZATIONS_HPP_

#include <types/Matrix.hpp>

//#include <types/Pair.hpp>


#ifdef OpenBLAS_FOUND
#include <cblas.h>
#endif

namespace mic {
namespace types {

/*!
 * \brief Template specialization: overloaded matrix multiplication operator - for floats. Uses OpenBLAS if found by CMAKE.
 * \author tkornuta
 */
template<>
inline Eigen::MatrixXf mic::types::Matrix<float>::operator *(
		const Eigen::MatrixXf & mat_) {

#ifdef OpenBLAS_FOUND
	// Using openBLAS boost!
	//printf("Using specialized (float) BLAS boost!\n");
	// Get dimensions.
	size_t M = this->rows();
	size_t K = this->cols();
	size_t N = mat_.cols();
	// Create temporary matrix.
	Eigen::MatrixXf c(M,N);

	cblas_sgemm( CblasColMajor, CblasNoTrans, CblasNoTrans, M, N, K, 1.0,
			this->data(), M,
			mat_.data(), K, 0.0, c.data(), M );
	return c;
#else
	// Calling base EIGEN operator *
	//printf("Calling specialized (float) base EIGEN operator *\n");
	return Eigen::MatrixXf::operator*(mat_);
#endif
}


/*!
 * \brief Vector of floats with single precision (of dynamic size).
 * \author tkornuta
 */
typedef Eigen::VectorXf VectorXf;


/*!
 * \brief Shared pointer to vector of floats with single precision (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< Eigen::VectorXf >VectorXfPtr;


/*!
 * \brief Matrix of floats with single precision (of dynamic size).
 * \author tkornuta
 */
typedef mic::types::Matrix<float> MatrixXf;


/*!
 * \brief Shared pointer to matrix with single precision floats (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::MatrixXf > MatrixXfPtr;


/*!
 * \brief The <single precision float matrix-char> pair type.
 * \author tkornuta
 */
typedef std::pair< MatrixXfPtr, std::shared_ptr<char> > MatrixXfCharPair;

/*!
 * \brief The <single precision float matrix-unsigned int> pair.
 * \author tkornuta
 */
typedef std::pair< MatrixXfPtr, std::shared_ptr<unsigned int> > MatrixXfUintPair;

/*!
 * \brief The <single precision float matrix - single precision float matrix> pair.
 * \author tkornuta
 */
typedef std::pair< MatrixXfPtr, MatrixXfPtr > MatrixXfMatrixXfPair;


}//: namespace types
}//: namespace mic



#endif /* SRC_TYPES_MATRIXXFSPECIALIZATIONS_HPP_ */
