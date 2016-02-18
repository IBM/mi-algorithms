/*!
 * \file MatrixTypes.hpp
 * \brief Contains definition of basic matrix datatypes derived from Eigen.
 * \author tkornut
 * \date Feb 5, 2016
 */

#ifndef SRC_TYPES_MATRIXTYPES_HPP_
#define SRC_TYPES_MATRIXTYPES_HPP_

#include <Eigen/Dense>

#ifdef OpenBLAS_FOUND
#include <cblas.h>
#endif

#include <random>

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




/*!
 * \brief Matrix with single precision floats (of dynamic size).
 * Uses OpenBLAS if found by CMAKE.
 *
 * \date Feb 17, 2016
 * \author tkornuta
 */
class MatrixXf : public Eigen::MatrixXf {
public:
	/*!
	 * Constructor. Calls default Eigen::MatrixXf constructor.
	 * @param Rows_ Number of rows.
	 * @param Cols_ Number of columns.
	 */
	MatrixXf (int Rows_, int Cols_) : Eigen::MatrixXf(Rows_, Cols_) { };

	/*!
	 * Overloaded operator - calls base operator.
	 */
    EIGEN_STRONG_INLINE Eigen::MatrixXf& operator =(const Eigen::MatrixXf& mat_)
    {
    	// Using base EIGEN operator =
    	return Base::operator=(mat_); // work
	}


    /*!
     * Overloaded matrix multiplication operator. Uses OpenBLAS if found by CMAKE.
     */
    EIGEN_STRONG_INLINE Eigen::MatrixXf operator *(const Eigen::MatrixXf& mat_)
    {

    	#ifdef OpenBLAS_FOUND
    	// Using openBLAS boost!
			// Get dimensions.
			size_t M = this->rows();
			size_t K =  this->cols();
			size_t N = mat_.cols();
			// Create temporary matrix.
			Eigen::MatrixXf c(M,N);

			cblas_sgemm( CblasColMajor, CblasNoTrans, CblasNoTrans, M, N, K, 1.0,
					this->data(), M,
					mat_.data(), K, 0.0, c.data(), M );
			return c;
		#else
			// Calling base EIGEN operator *
			return Base::operator*(mat_);
		#endif
    }

    /*!
     * Set values of all matrix elements to random with a normal distribution.
     * @param mean Mean
     * @param stddev Variance
     */
    void normRandReal(float mean, float stddev) {

    	// Initialize random number generator with normal distribution.
    	std::random_device rd;
    	std::mt19937 mt(rd());
    	std::normal_distribution<> randn(mean, stddev);

    	// Get access to data.
    	float* data_ptr = this->data();

		#pragma omp parallel for
    	for (int i = 0; i < this->rows()*this->cols(); i++) {
    		data_ptr[i] = randn(mt);
		}
	}


	void elementwiseFunctionMatrix(float (*func)(float, float), Eigen::MatrixXf & mat_) {

		// Check dimensions.
		if ((this->rows() != mat_.rows()) || (this->cols() != mat_.cols()))
			printf("elementwise_function_matrix, dim mismatch!\n");

    	// Get access to data.
    	float* data_ptr = this->data();
    	float* m_data_ptr = mat_.data();

    	// Apply function to all elements.
		#pragma omp parallel for
		for (size_t i = 0; i < rows()*cols(); i++) {
			data_ptr[i] = (*func)(data_ptr[i], m_data_ptr[i]);
		}

	}

};


/*!
 * \brief Shared pointer to matrix with single precision floats (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::MatrixXf > matrixf_ptr_t;


/*!
 * \brief The <single precision float matrix-char> pair type.
 * \author tkornuta
 */
typedef std::pair< matrixf_ptr_t, std::shared_ptr<char> > matrixf_char_pair_t;

/*!
 * \brief The <single precision float matrix-unsigned int> pair.
 * \author tkornuta
 */
typedef std::pair< matrixf_ptr_t, std::shared_ptr<unsigned int> > matrixf_uint_pair_t;








}//: namespace types
}//: namespace mic



#endif /* SRC_TYPES_MATRIXTYPES_HPP_ */
