/*!
 * \file Matrix.hpp
 * \brief 
 * \author tkornut
 * \date Mar 7, 2016
 */

#ifndef SRC_TYPES_MATRIX_HPP_
#define SRC_TYPES_MATRIX_HPP_

#include <Eigen/Dense>
#include <random>

namespace mic {
namespace types {

/// Forward declaration of a class Tensor.
template<typename T>
class Tensor;

/*!
 * \brief Typedef for template-typed dynamic matrices.
 * \author tkornuta
 */
//template<typename T>
//using MatrixX = typename Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

/*!
 * \brief Typedef for template-typed dynamic vectors.
 * \author tkornuta
 */
//template<typename T>
//using VectorX = typename Eigen::Matrix<T, Eigen::Dynamic, 1>;

/*!
 * \brief Template-typed Matrix of dynamic size.
 * Uses OpenBLAS if found by CMAKE - overloaded, specializations of * operator for types: float, double.
 *
 * \tparam T Template parameter denoting elementary type of data used (int, float, double etc.)
 * \date Mar 7, 2016
 * \author tkornuta
 */
template<typename T>
class Matrix: public Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> {
public:

	/*!
	 * Constructor. Calls default Eigen::MatrixXf constructor.
	 */
	Matrix() :
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>() {
	}

	/*!
	 * Constructor. Calls default Eigen::MatrixXf constructor.
	 * @param Rows_ Number of rows.
	 * @param Cols_ Number of columns.
	 */
	Matrix(int Rows_, int Cols_) :
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(Rows_, Cols_) {
	}

	/*!
	 * Copying constructor on the basis of a tensor. Copies dimensions and data.
	 * Note: tensor must be 2D.
	 * @param tensor_ Tensor
	 */
	Matrix(mic::types::Tensor<T>& tensor_) :
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(tensor_.dim(1), tensor_.dim(0))
	{
		// Tensor must be 2D!
		assert(tensor_.dims().size() == 2);
		memcpy(this->data(), tensor_.data(), tensor_.size() * sizeof(T));
	}

	/*!
	 * Overloaded assignment operator - calls base operator.
	 * @param mat_ Input matrix
	 * @return An exact copy of the input matrix.
	 */
	EIGEN_STRONG_INLINE
	Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& operator =(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& mat_) {
		// Using base EIGEN operator =
		return Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>::operator=(mat_);
	}

	/*!
	 * Overloaded multiplication operator
	 * @param mat_ Input matrix
	 * @return Resulting matrix - multiplication of this and input mat_.
	 */
	EIGEN_STRONG_INLINE
	Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> operator *(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& mat_) {
		// Calling base EIGEN operator *
		//printf("Calling base EIGEN operator *\n");
		return Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>::operator*(mat_);
	}

	/*!
	 * Set values of all matrix elements to random with a normal distribution.
	 * @param mean Mean
	 * @param stddev Variance
	 */
	void normRandReal(T mean = 0, T stddev = 1) {

		// Initialize random number generator with normal distribution.
		std::random_device rd;
		std::mt19937 mt(rd());
		std::normal_distribution<> dist(mean, stddev);

		// Get access to data.
		T* data_ptr = this->data();

#pragma omp parallel for
		for (size_t i = 0; i < this->rows() * this->cols(); i++) {
			data_ptr[i] = dist(mt);
		}
	}

	/*!
	 * Set values of all matrix elements to random real numbers from range <min, max> - uniform distribution.
	 * @param min Min value.
	 * @param max Max value.
	 * @return Random real value.
	 */
	void uniRandReal(T min = 0, T max = 1) {

		// Initialize random number generator with normal distribution.
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<> dist(min, max);

		// Get access to data.
		float* data_ptr = this->data();

#pragma omp parallel for
		for (size_t i = 0; i < this->rows() * this->cols(); i++) {
			data_ptr[i] = dist(rd);
		}
	}

	/*!
	 * Applies elementwise function to all matrix elements.
	 * @param func Function to be applied.
	 */
	void elementwiseFunction(T (*func)(T)) {

		// Get access to data.
		T* data_ptr = this->data();

		// Apply function to all elements.
#pragma omp parallel for
		for (size_t i = 0; i < this->rows() * this->cols(); i++) {
			data_ptr[i] = (*func)(data_ptr[i]);
		} //: for i
	}

	/*!
	 * Applies elementwise function to all matrix elements passing scalar as function argument.
	 * @param func Function to be applied.
	 * @param scalar_ Scalar passed to function as argument.
	 */
	void elementwiseFunctionScalar(T (*func)(T, T), T scalar_) {

		// Get access to data.
		T* data_ptr = this->data();

		// Apply function to all elements.
#pragma omp parallel for
		for (size_t i = 0; i < this->rows() * this->cols(); i++) {
			data_ptr[i] = (*func)(data_ptr[i], scalar_);
		} //: for i
	}

	/*!
	 * Applies elementwise function to all matrix elements and uses additional Matrix mat_ data as function parameter.
	 * @param func Function to be applied.
	 * @param mat_ Matrix passed to function as argument.
	 */
	void elementwiseFunctionMatrix(T (*func)(T, T), Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> & mat_) {

		// Check dimensions.
		if ((this->rows() != mat_.rows()) || (this->cols() != mat_.cols()))
			printf("elementwiseFunctionMatrix: dimensions mismatch!\n");

		// Get access to data.
		T* data_ptr = this->data();
		T* m_data_ptr = mat_.data();

		// Apply function to all elements.
#pragma omp parallel for
		for (size_t i = 0; i < this->rows() * this->cols(); i++) {
			data_ptr[i] = (*func)(data_ptr[i], m_data_ptr[i]);
		}

	}

	/*!
	 * Applies function to all matrix elements and uses additional vector data as function parameter - columnwise.
	 * @param func Used function
	 * @param v_ Vector passed to function
	 */
	void matrixColumnVectorFunction(T (*func)(T, T),
			Eigen::Matrix<T, Eigen::Dynamic, 1>& v_) {

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
				(*this)(y, x) = (*func)((*this)(y, x), v_(y));
			}		//: for y
		}		//: for x

	}

	/*!
	 * Applies function to all matrix elements and uses additional vector data as function parameter - rowwise.
	 * @param func Used function
	 * @param v_ Vector passed to function
	 */
	void matrixRowVectorFunction(T (*func)(T, T), Eigen::Matrix<T, Eigen::Dynamic, 1>& v_) {

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
				(*this)(y, x) = (*func)((*this)(y, x), v_(x));
				//data_ptr[x + y*cols] = (*func)(data_ptr[x + y*cols], vector_data_ptr[y]);
			}				//: for y
		}				//: for x

	}

	/*!
	 * Sets the consecutive columns to be equal to given vector.
	 * @param in Input vector, that will be "cloned".
	 */
	void repeatVector(Eigen::Matrix<T, Eigen::Dynamic, 1> &in) {
#pragma omp parallel for
		for (size_t x = 0; x < this->cols(); x++) {
			for (size_t y = 0; y < this->rows(); y++) {

				(*this)(y, x) = in(y);
			}				//: y
		}				//: x

	}

};


/*!
 * \brief Typedef for shared pointer to template-typed dynamic matrices.
 * \author tkornuta
 */
template<typename T>
using MatrixPtr = typename std::shared_ptr< mic::types::Matrix<T> >;

}				//: namespace types
}				//: namespace mic

#endif /* SRC_TYPES_MATRIX_HPP_ */
