/*!
 * \file MatrixXf.cpp
 * \brief 
 * \author tkornut
 * \date Feb 17, 2016
 */

#include <types/MatrixXf.hpp>

#include <random>

#ifdef OpenBLAS_FOUND
#include <cblas.h>
#endif


namespace mic {
namespace types {

Eigen::MatrixXf MatrixXf::operator *(const Eigen::MatrixXf& mat_)
{

	#ifdef OpenBLAS_FOUND
	// Using openBLAS boost!
		printf("Using BLAS boost!\n");
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
		printf("Calling base EIGEN operator *\n");
		// Calling base EIGEN operator *
		return Base::operator*(mat_);
	#endif
}

void MatrixXf::normRandReal(float mean, float stddev) {

	// Initialize random number generator with normal distribution.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<> dist(mean, stddev);

	// Get access to data.
	float* data_ptr = this->data();

	#pragma omp parallel for
	for (int i = 0; i < this->rows()*this->cols(); i++) {
		data_ptr[i] = dist(mt);
	}
}

void MatrixXf::uniRandReal(float min, float max) {

	// Initialize random number generator with normal distribution.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> dist(min, max);

	// Get access to data.
	float* data_ptr = this->data();

	#pragma omp parallel for
	for (int i = 0; i < this->rows()*this->cols(); i++) {
		data_ptr[i] = dist(rd);
	}
}


void MatrixXf::elementwiseFunctionMatrix(float (*func)(float, float), Eigen::MatrixXf & mat_) {

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

} /* namespace types */
} /* namespace mic */
