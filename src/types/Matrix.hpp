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
#include <memory> // std::shared_ptr

#include <boost/serialization/serialization.hpp>
// include this header to serialize vectors
#include <boost/serialization/vector.hpp>
// include this header to serialize arrays
#include <boost/serialization/array.hpp>
#include <boost/serialization/version.hpp>

// Forward declaration of class boost::serialization::access
namespace boost {
namespace serialization {
class access;
}//: serialization
}//: access

namespace mic {
namespace types {

// Forward declaration of a class Tensor.
template<typename T>
class Tensor;

// Forward declaration of a class Vector.
//template<typename T>
//class Vector;

/*!
 * \brief Template-typed Matrix of dynamic size.
 * Uses OpenBLAS if found by CMAKE - overloaded, specializations of * operator for types: float, double.
 *
 * \tparam T Template parameter denoting elementary type of data used (int, float, double etc.)
 * \date Mar 7, 2016
 * \author tkornuta
 */
template<typename T>
class Matrix : public Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> {
public:

	/*!
	 * Constructor. Calls default Eigen::MatrixXf constructor.
	 */
	EIGEN_STRONG_INLINE
	Matrix() :
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>() {
	}

	/*!
	 * Constructor. Calls default Eigen::MatrixXf constructor.
	 * @param Rows_ Number of rows.
	 * @param Cols_ Number of columns.
	 */
	EIGEN_STRONG_INLINE
	Matrix(int Rows_, int Cols_) :
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(Rows_, Cols_) {
		this->setZero();
	}

	/*!
	 * Copying constructor on the basis of a vector. Sets dimensions to rows = size(), cols = 1.
	 * @param vector_ Vector
	 */
	EIGEN_STRONG_INLINE
	Matrix(const Eigen::Matrix<T, Eigen::Dynamic, 1>& vector_) :
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(vector_.size(), 1)
	{
		// Copy the whole vector block.
		memcpy(this->data(), vector_.data(), vector_.size() * sizeof(T));
	}

	/*!
	 * Copying constructor on the basis of another matrix.
	 * @param matrix_ Matrix to be copied.
	 */
	EIGEN_STRONG_INLINE
	Matrix(const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& matrix_) :
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(matrix_.rows(), matrix_.cols())
	{
		// Copy the whole vector block.
		memcpy(this->data(), matrix_.data(), matrix_.size() * sizeof(T));
	}

	/*!
	 * Copying constructor on the basis of a tensor. Copies dimensions and data.
	 * Note: tensor must be 2D.
	 * @param tensor_ Tensor
	 */
	EIGEN_STRONG_INLINE
	Matrix(mic::types::Tensor<T>& tensor_) :
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(tensor_.dim(1), tensor_.dim(0))
	{
		// Tensor must be 2D!
		assert(tensor_.dims().size() == 2);
		// Copy the whole block.
		memcpy(this->data(), tensor_.data(), tensor_.size() * sizeof(T));
	}

	/*
	 * Overloaded assignment operator - calls base operator.
	 * @param mat_ Input matrix
	 * @return An exact copy of the input matrix.
	 */
/*	EIGEN_STRONG_INLINE
	const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& operator =(const mic::types::Matrix<T>& mat_) {
		// Using base EIGEN operator =
		return Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>::operator=(mat_);
	}*/


/*	EIGEN_STRONG_INLINE
	const Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& operator =(const Eigen::Matrix<T, Eigen::Dynamic, 1>& vector_) {
		// Using base EIGEN operator =
		return Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>::operator=(vector_);
	}*/

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
	 * Overloaded multiplication operator.
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
	 * Sets values of all element to the value given as parameter.
	 * @param value_ The value to be set.
	 */
	void setValue(T value_) {
		// Get access to data.
		T* data_ptr = this->data();

#pragma omp parallel for
		for (size_t i = 0; i < (size_t) (this->rows() * this->cols()); i++)
			data_ptr[i] = value_;
	}

	/*!
	 * Enumerates - sets values of elements to their indices.
	 */
	void enumerate() {
		// Get access to data.
		T* data_ptr = this->data();

#pragma omp parallel for
		for (size_t i = 0; i < (size_t)this->size(); i++)
			data_ptr[i] = i;
	}

	/*!
	 * Set values of all matrix elements to random with a normal distribution.
	 * @param mean Mean
	 * @param stddev Variance
	 */
	void randn(T mean = 0, T stddev = 1) {

		// Initialize random number generator with normal distribution.
		std::random_device rd;
		std::mt19937 mt(rd());
		std::normal_distribution<double> dist(mean, stddev);

		// Get access to data.
		T* data_ptr = this->data();

#pragma omp parallel for
		for (size_t i = 0; i < (size_t) (this->rows() * this->cols()); i++) {
			data_ptr[i] = (T)dist(mt);
		}
	}

	/*!
	 * Set values of all matrix elements to random numbers from range <min, max> - uniform distribution.
	 * @param min Min value.
	 * @param max Max value.
	 * @return Random real value.
	 */
	void rand(T min = 0, T max = 1) {

		// Initialize random number generator with normal distribution.
		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_real_distribution<double> dist(min, max);

		// Get access to data.
		T* data_ptr = this->data();

#pragma omp parallel for
		for (size_t i = 0; i < (size_t) (this->rows() * this->cols()); i++) {
			data_ptr[i] = (T)dist(rd);
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
		for (size_t i = 0; i < (size_t) (this->rows() * this->cols()); i++) {
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
		for (size_t i = 0; i < (size_t) (this->rows() * this->cols()); i++) {
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
		for (size_t i = 0; i < (size_t) (this->rows() * this->cols()); i++) {
			data_ptr[i] = (*func)(data_ptr[i], m_data_ptr[i]);
		}//: for i

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
		for (size_t x = 0; x < (size_t)this->cols(); x++) {
			for (size_t y = 0; y < (size_t)this->rows(); y++) {
				//data_ptr[x + y*cols] = (*func)(data_ptr[x + y*cols], vector_data_ptr[x]);
				(*this)(y, x) = (*func)((*this)(y, x), v_(y));
			}//: for y
		}//: for x
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
		for (size_t x = 0; x < (size_t)this->cols(); x++) {
			for (size_t y = 0; y < (size_t)this->rows(); y++) {
				//h(y,x) += c(x);
				//(*this)(y,x) += v_(x);
				(*this)(y, x) = (*func)((*this)(y, x), v_(x));
				//data_ptr[x + y*cols] = (*func)(data_ptr[x + y*cols], vector_data_ptr[y]);
			}//: for y
		}//: for x
	}


	/*!
	 * Sets the consecutive columns to be equal to given vector.
	 * @param in Input vector, that will be "cloned".
	 */
	void repeatVector(Eigen::Matrix<T, Eigen::Dynamic, 1> &in) {
#pragma omp parallel for
		for (size_t x = 0; x < (size_t)this->cols(); x++) {
			for (size_t y = 0; y < (size_t)this->rows(); y++) {

				(*this)(y, x) = in(y);
			}//: y
		}//: x
	}

	/*!
	 * Returns a vector of indices indicating maximal elements in consecutive matrix columns (colwise).
	 * @return Vector of indices.
	 */
	Eigen::Matrix<T, Eigen::Dynamic, 1> colwiseReturnMaxIndices() {

		Eigen::Matrix<T, Eigen::Dynamic, 1> indices((*this).cols());

		for (size_t i = 0; i < (size_t)(*this).cols(); i++) {

			T current_max_val;
			T index;

			for (size_t j = 0; j < (size_t)(*this).rows(); j++) {

				if (j == 0 || (*this)(j, i) > current_max_val) {

					index = j;
					current_max_val = (*this)(j, i);
				}

				indices(i) = index;

			}
		}

		return indices;
	}

	/*!
	 * Calculates the cross entropy as measure of how accurate given matrix (treated as prediction) fits to the desired (target) matrix.
	 * @param targets_ Desired results (targets) in the form of a matrix of answers.
	 * @return
	 */
	T calculateCrossEntropy(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& targets_) {

		T ce = 0.0;
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> error(this->rows(), this->cols());

		//check what has happened and get information content for that event
		error.array() = - (this->unaryExpr(std::ptr_fun(::logf)).array() * targets_.array());

		// Sum the errors.
		ce = error.sum();

		return ce;
	}

	/****************** ARMADILLO COMPATIBILITY *********************************/

	/*!
	 * Set zeros.
	 */
	void zeros (){
		this->setZero();
	}

    T& operator [](int idx) {
        return this->data()[idx];
    }

    T operator [](int idx) const {
        return this->data()[idx];
    }

private:

	// Friend class - required for using boost serialization.
    friend class boost::serialization::access;

    /*!
     * Serialization save - saves the matrix object to archive.
     * @param ar Used archive.
     * @param version Version of the matrix class.
     */
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const {
    	size_t rows, cols;
    	rows = this->rows();
    	cols = this->cols();
		ar & rows;
		ar & cols;
        // Save elements.
        size_t elements = (size_t)(this->rows() * this->cols());
		T* data_ptr = (T*)this->data();
        ar & boost::serialization::make_array<T>(data_ptr, elements);
     }

    /*!
     * Serialization load - loads the matrix object to archive.
     * @param ar Used archive.
     * @param version Version of the matrix class.
     */
     template<class Archive>
     void load(Archive & ar, const unsigned int version) {
    	size_t rows, cols;
		ar & rows;
		ar & cols;
		// Allocate memory - resize.
		this->resize(rows, cols);
		// Load elements
        size_t elements = (size_t)(this->rows() * this->cols());
		T* data_ptr = this->data();
		ar & boost::serialization::make_array<T>(data_ptr, elements);
     }

     // The serialization must be splited as load requires to allocate the memory.
     BOOST_SERIALIZATION_SPLIT_MEMBER()

};


/*!
 * \brief Typedef for a shared pointer to template-typed dynamic matrices.
 * \author tkornuta
 */
template<typename T>
using MatrixPtr = typename std::shared_ptr< mic::types::Matrix<T> >;

}//: namespace types
}//: namespace mic



// Just in the case that something important will change in the matrix class - set version.
BOOST_CLASS_VERSION(mic::types::Matrix<bool>, 1)
BOOST_CLASS_VERSION(mic::types::Matrix<short>, 1)
BOOST_CLASS_VERSION(mic::types::Matrix<int>, 1)
BOOST_CLASS_VERSION(mic::types::Matrix<long>, 1)
BOOST_CLASS_VERSION(mic::types::Matrix<float>, 1)
BOOST_CLASS_VERSION(mic::types::Matrix<double>, 1)

#endif /* SRC_TYPES_MATRIX_HPP_ */
