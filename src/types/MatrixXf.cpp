/*!
 * \file MatrixXf.cpp
 * \brief 
 * \author tkornut
 * \date Feb 17, 2016
 */


//#ifdef FALSE

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
		//printf("Using BLAS boost!\n");
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
		//printf("Calling base EIGEN operator *\n");
		return Base::operator*(mat_);
	#endif
}

void MatrixXf::normRandReal(float mean, float stddev) {

	// Initialize random number generator with normal distribution.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<> dist(mean, stddev);

	// Get access to data.
	float* data_ptr = data();

	#pragma omp parallel for
	for (size_t i = 0; i < rows()*cols(); i++) {
		data_ptr[i] = dist(mt);
	}
}

void MatrixXf::uniRandReal(float min, float max) {

	// Initialize random number generator with normal distribution.
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<> dist(min, max);

	// Get access to data.
	float* data_ptr = data();

	#pragma omp parallel for
	for (size_t i = 0; i < rows()*cols(); i++) {
		data_ptr[i] = dist(rd);
	}
}


void MatrixXf::elementwiseFunction(float (*func)(float)) {

	// Get access to data.
	float* data_ptr = this->data();

	// Apply function to all elements.
	#pragma omp parallel for
	for (size_t i = 0; i < rows()*cols(); i++) {
		data_ptr[i] = (*func)(data_ptr[i]);
	}//: for i
}


void MatrixXf::elementwiseFunctionScalar(float (*func)(float, float), float scalar_) {

	// Get access to data.
	float* data_ptr = data();

	// Apply function to all elements.
	#pragma omp parallel for
	for (size_t i = 0; i < rows()*cols(); i++) {
		data_ptr[i] = (*func)(data_ptr[i], scalar_);
	}//: for i
}



void MatrixXf::elementwiseFunctionMatrix(float (*func)(float, float), Eigen::MatrixXf & mat_) {

	// Check dimensions.
	if ((rows() != mat_.rows()) || (cols() != mat_.cols()))
		printf("elementwiseFunctionMatrix: dimensions mismatch!\n");

	// Get access to data.
	float* data_ptr = this->data();
	float* m_data_ptr = mat_.data();

	// Apply function to all elements.
	#pragma omp parallel for
	for (size_t i = 0; i < rows()*cols(); i++) {
		data_ptr[i] = (*func)(data_ptr[i], m_data_ptr[i]);
	}

}


void MatrixXf::matrixColumnVectorFunction( float(*func)(float, float), mic::types::VectorXf& v_) {

	if (this->rows() != v_.cols())
		printf("matrixColumnVectorFunction: dimensions mismatch\n");

	// Get access to data.
	//float* data_ptr = data();
	//int rows = this->rows();
	//int cols = this->cols();
	//float* vector_data_ptr = v_.data();

	#pragma omp parallel for
	for (size_t x = 0; x < this->cols(); x++) {
		for (size_t y = 0; y < this->rows(); y++) {
			//data_ptr[x + y*cols] = (*func)(data_ptr[x + y*cols], vector_data_ptr[x]);
			(*this)(y,x) = (*func)((*this)(y,x), v_(y));
		}//: for y
	}//: for x

}
/*
void matrix_column_vector_function(T(*func)(T, T), Matrix<T>& v) {

	if ((this->n_cols() != v.n_cols()) && (v.n_rows() == 1))
		printf("matrix_vector_function!\n");

	#pragma omp parallel for
	for (size_t i = 0; i < n_rows(); i++) {
		for (size_t j = 0; j < n_cols(); j++) {
			this->operator()(i, j) = (*func)(this->operator()(i, j), v(0, j));
		}
	}

}*/

void MatrixXf::matrixRowVectorFunction( float(*func)(float, float), mic::types::VectorXf& v_) {

	if (this->cols() != v_.cols())
		printf("matrixRowVectorFunction: dimensions mismatch\n");

	// Get access to data.
	/*float* data_ptr = data();
	int rows = this->rows();
	int cols = this->cols();
	float* vector_data_ptr = v_.data();*/

	#pragma omp parallel for
	for (size_t x = 0; x < this->cols(); x++) {
		for (size_t y = 0; y < this->rows(); y++) {
			//h(y,x) += c(x);
			//(*this)(y,x) += v_(x);
			(*this)(y,x) = (*func)((*this)(y,x), v_(x));
			//data_ptr[x + y*cols] = (*func)(data_ptr[x + y*cols], vector_data_ptr[y]);
		}//: for y
	}//: for x

}
/*
void matrix_row_vector_function(T(*func)(T, T), Matrix<T>& v) {

	if ((this->n_rows() != v.n_rows()) && (v.n_cols() == 1))
		printf("matrix_vector_function!\n");

	#pragma omp parallel for
	for (size_t i = 0; i < n_rows(); i++) {
		for (size_t j = 0; j < n_cols(); j++) {
			this->operator()(i, j) = (*func)(this->operator()(i, j), v(i, 0));
		}
	}

}
*/
} /* namespace types */
} /* namespace mic */

//#endif
