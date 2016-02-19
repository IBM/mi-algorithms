/*!
 * \file MatrixXf.hpp
 * \brief 
 * \author tkornut
 * \date Feb 17, 2016
 */

#ifndef SRC_TYPES_MATRIXXF_HPP_
#define SRC_TYPES_MATRIXXF_HPP_

//#ifdef FALSE
#include <Eigen/Dense>

namespace mic {
namespace types {

class MatrixXf;

/*!
 * \brief Vector of floats with single precision (of dynamic size).
 * \author tkornuta
 */
//typedef Eigen::VectorXf VectorXf;
class VectorXf : public Eigen::VectorXf {
public:

	/*!
	 * Constructor. Calls default Eigen::VectorXf constructor.
	 */
	VectorXf () : Eigen::VectorXf() { };


	/*!
	 * Constructor. Calls default Eigen::VectorXf constructor.
	 * @param Cols_ Number of columns.
	 */
	VectorXf (int Cols_) : Eigen::VectorXf(Cols_) { };

	/*!
	 * Overloaded operator - calls base operator.
	 */
    EIGEN_STRONG_INLINE Eigen::VectorXf& operator =(const Eigen::VectorXf& mat_) {
		// Using base EIGEN operator =
		return Base::operator=(mat_);
	}


    void sum_cols(MatrixXf& m1);

    void diff_cols(MatrixXf& m1, MatrixXf& m2);

    void sum_rows(MatrixXf& m1);

    void diff_rows(MatrixXf& m1, MatrixXf& m2);

	void elementwise_function_scalar(float(*func)(float, float), float scalar) {

		#pragma omp parallel for
		for (size_t i = 0; i < this->rows()*this->cols(); i++) {

			data()[i] = (*func)(data()[i], scalar);

		}

	}

	void elementwise_function_vector(float(*func)(float, float), VectorXf &m) {

		if ((this->rows() != m.rows()) || (this->cols() != m.cols()))
			printf("elementwise_function_matrix, dim mismatch!\n");

		#pragma omp parallel for
		for (size_t i = 0; i < this->rows()*this->cols(); i++) {

			data()[i] = (*func)(data()[i], m.data()[i]);

		}

	}

};




/*!
 * \brief Shared pointer to vector of floats with single precision (of dynamic size).
 * \author tkornuta
 */
typedef std::shared_ptr< Eigen::VectorXf >VectorXfPtr;



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
	 */
	MatrixXf () : Eigen::MatrixXf() { };


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
    	return Base::operator=(mat_);
	}

    /*!
     * Overloaded matrix multiplication operator. Uses OpenBLAS if found by CMAKE.
     */
    Eigen::MatrixXf operator *(const Eigen::MatrixXf& mat_);

    /*!
     * Set values of all matrix elements to random with a normal distribution.
     * @param mean Mean
     * @param stddev Variance
     */
    void normRandReal(float mean = 0, float stddev = 1);

	/*!
	 * Set values of all matrix elements to random real numbers from range <min, max> - uniform distribution.
	 * @param min Min value.
	 * @param max Max value.
	 * @return Random real value.
	 */
	void uniRandReal(float min = 0, float max = 1);

    /*!
     * Applies elementwise function to all matrix elements.
     * @param func Function to be applied.
     */
	void elementwiseFunction(float (*func)(float));

	/*!
     * Applies elementwise function to all matrix elements passing scalar as function argument.
     * @param func Function to be applied.
	 * @param scalar_ Scalar passed to function as argument.
	 */
	void elementwiseFunctionScalar(float (*func)(float, float), float scalar_);

    /*!
     * Applies elementwise function to all matrix element and uses additional Matrix mat_ data as function paramter.
     * @param func Function to be applied.
     * @param mat_ Matrix passed to function as argument.
     */
	void elementwiseFunctionMatrix(float (*func)(float, float), Eigen::MatrixXf & mat_);


	void matrixColumnVectorFunction( float(*func)(float, float), mic::types::VectorXf& v_);

	void matrixRowVectorFunction( float(*func)(float, float), mic::types::VectorXf& v_);



    void matrix_row_vector_function(float(*func)(float,float), VectorXf& v) {

    	if ((this->rows() != v.rows()) && (v.cols() == 1))
			printf("matrix_vector_function!\n");

		#pragma omp parallel for
		for (size_t i = 0; i < this->rows(); i++) {
			for (size_t j = 0; j < this->cols(); j++) {
				this->operator()(i, j) = (*func)(this->operator()(i, j), v(i));
			}
		}

    }

	void elementwise_function(float(*func)(float)) {

		#pragma omp parallel for
		for (size_t i = 0; i < this->rows()*this->cols(); i++) {

			data()[i] = (*func)(data()[i]);

		}

	}


	void elementwise_function_matrix(float(*func)(float, float), MatrixXf &m) {

		if ((this->rows() != m.rows()) || (this->cols() != m.cols()))
			printf("elementwise_function_matrix, dim mismatch!\n");

		#pragma omp parallel for
		for (size_t i = 0; i < this->rows()*this->cols(); i++) {

			data()[i] = (*func)(data()[i], m.data()[i]);

		}

	}

    void matrix_column_vector_function(float(*func)(float, float), VectorXf& v) {

    	//if ((this->cols() != v.cols()) && (v.rows() == 1))
		//	printf("matrix_vector_function!\n");

		#pragma omp parallel for
		for (size_t i = 0; i < this->rows(); i++) {
			for (size_t j = 0; j < this->cols(); j++) {
				this->operator()(i, j) = (*func)(this->operator()(i, j), v(j));
			}
		}

    }

	static void repmat(MatrixXf &out, VectorXf &in, size_t batchsize) {

		//TODO, optimize
		for (size_t j = 0; j < batchsize; j++) {

			for (size_t i = 0; i < out.rows(); i++) {

				out(i, j) = in(i);
			}

		}

	}

	void elementwise_function_scalar(float(*func)(float, float), float scalar) {

		#pragma omp parallel for
		for (size_t i = 0; i < this->rows()*this->cols(); i++) {

			data()[i] = (*func)(data()[i], scalar);

		}

	}



};


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


} /* namespace types */
} /* namespace mic */

//#endif

#endif /* SRC_TYPES_MATRIXXF_HPP_ */
