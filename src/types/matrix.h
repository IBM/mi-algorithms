/*
* @File: matrix.h
* @Author: kamilrocki
* @Email: kmrocki@us.ibm.com
* @Created:	2015-04-26 13:04:17
* @Last Modified by:   kamilrocki
* @Last Modified time: 2015-05-19 16:45:09
* @Last Modified time: 2015-05-18 15:50:57
*/

#ifndef __CXXMATRIX_H__
#define __CXXMATRIX_H__

#include <types/tensor.h>
#include <iomanip>
#include <typeinfo>

#include <cblas.h>
//#include<boost::numeric::ublas>

#include <data_utils/RandomGenerator.hpp>


namespace mic {
namespace types {


enum class DISPLAY_MODE {DEFAULT_MODE, ROWS_ARE_IMAGES, ROWS_ARE_WEIGHTS, UNFLATTEN, GRAPH};

// TODO: do wywalenia!
#define MATRIX_MEMCPY(X, Y) memcpy((X)->data, (Y)->data, (X)->elements * sizeof(float))
#define MATRIX_MEMCPY_ROW(X, Y, N) memcpy(&((X)->data[(N)*(Y)->elements]), (Y)->data, (Y)->elements * sizeof(float))


/**
 * \brief Class representing an n-D matrix.
 * \author krocki
 * \tparam T template parameter denoting data type stored in matrix.
 */
template <class T>
class Matrix : public Tensor<T> {

	public:

		Matrix() = default;

		/*!
		 * Copying constructor.
		 * \author tkornuta
		 * @param m
		 */
		Matrix(const Matrix& m)  {
			//if (Tensor<T>::data != nullptr) delete(Tensor<T>::data);

			// Copy dimesions and number of elements.
			Tensor<T>::dim = m.dim;
			Tensor<T>::elements = m.elements;

			// Allocate memory and copy data!
			Tensor<T>::data = new T[Tensor<T>::elements];
			memcpy(Tensor<T>::data, m.data, sizeof(T) * Tensor<T>::elements);
		}

		Matrix(size_t M, size_t N) : Tensor<T>({M, N}) {

				transposed = false;
				display_mode = DISPLAY_MODE::DEFAULT_MODE;
		}

		virtual ~Matrix() = default;

		void transpose(void) {

			transposed = !transposed;

		}

        virtual void serialize(std::ostream& os) const {

            Tensor<T>::serialize(os);

            os << std::endl;

            for (size_t i = 0; i < Tensor<T>::dim[0]; i++) {
            	for (size_t j = 0; j < Tensor<T>::dim[1]; j++) {

		            if (typeid(T) == typeid(bool)) {
		            	os << Tensor<T>::data[i * Tensor<T>::dim[1] + j];
		            } else {
	            		os << "[" << std::setw( 3 ) << std::setfill( '-' ) << i << 
	            		"," << std::setw( 3 ) << std::setfill( '-' ) << j << "] " << 
	            		std::fixed << std::setw( 6 ) << std::setprecision( 3 ) << 
	            		std::setfill( ' ' ) << Tensor<T>::data[i * Tensor<T>::dim[1] + j] << " ";
            		}
            	}
            	
            	if (i < Tensor<T>::dim[0] - 1) os << std::endl;
            }
        }

		T& operator()(const size_t i, const size_t j) {
		 
		    return Tensor<T>::data[(i * n_cols() + j) * !transposed + transposed * (j * n_cols() + i)];
		}

		Matrix<T>& operator=(Matrix<T>& m) {

			std::cout << "dupa12\n";

			Tensor<T>::operator=(m);
			transposed = m.transposed;
			display_mode = m.display_mode;
			return *this;
		}

		T sum() {

			return cblas_sasum(this->elements, (T*)this->data, 1);

		}

		T norm() {

			return cblas_snrm2(this->elements, (T*)this->data, 1);

		}

		static void repmat(Matrix<T> &out, Matrix<T> &in, size_t batchsize) {

			//TODO, optimize
			for (size_t j = 0; j < batchsize; j++) {

				for (size_t i = 0; i < out.n_rows(); i++) {

					out(i, j) = in(i, 0);
				}

			}

		}

        static void sgemm(Matrix<T> &c, Matrix<T> &a, Matrix<T> &b) {

        	//check if dimensions agree

        	//printf("%d x %d = %d x %d * %d x %d\n", c.n_rows(), c.n_cols(), a.n_rows(), a.n_cols(), b.n_rows(), b.n_cols());

			enum CBLAS_TRANSPOSE transA = a.transposed ? CblasTrans : CblasNoTrans;
			enum CBLAS_TRANSPOSE transB = b.transposed ? CblasTrans : CblasNoTrans;

			float alpha = 1.0f;
			float beta = 0.0f;

			size_t M = a.n_rows();
			size_t N = c.n_cols();
			size_t K = b.n_rows();

			size_t lda = a.dim[1];
			size_t ldb = b.dim[1];
			size_t ldc = c.dim[1];

			cblas_sgemm(CblasRowMajor, transA, transB, M, N, K, alpha, (T*)a.data, lda, (T*)b.data, ldb, beta, (T*)c.data, ldc);

        }

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

        void matrix_column_vector_function(T(*func)(T, T), Matrix<T>& v) {

        	if ((this->n_cols() != v.n_cols()) && (v.n_rows() == 1))
				printf("matrix_vector_function!\n");

			#pragma omp parallel for
			for (size_t i = 0; i < n_rows(); i++) {
				for (size_t j = 0; j < n_cols(); j++) {
					this->operator()(i, j) = (*func)(this->operator()(i, j), v(0, j));
				}
			}

        }

        void diff_cols(Matrix<T>& m1, Matrix<T>& m2) {

        	if ((this->n_cols() != m1.n_cols()) || (m1.n_cols() != m2.n_cols()) || (m1.n_rows() != m2.n_rows()))
				printf("diff_cols!\n");

			#pragma omp parallel for
			for (size_t j = 0; j < n_cols(); j++) {
				
				float diff = 0.0f;
				
				for (size_t i = 0; i < n_rows(); i++) {
				
					diff += m1(i, j) - m2(i, j);
				}

				this->operator()(0, j) = diff;
			}

        }

        void diff_rows(Matrix<T>& m1, Matrix<T>& m2) {

        	if ((this->n_rows() != m1.n_rows()) || (m1.n_cols() != m2.n_cols()) || (m1.n_rows() != m2.n_rows()))
				printf("diff_rows!\n");

			#pragma omp parallel for
			for (size_t i = 0; i < m1.n_rows(); i++) {
			
				float diff = 0.0f;
				
				for (size_t j = 0; j < m1.n_cols(); j++) {
				
					diff += m1(i, j) - m2(i, j);
				}

				this->operator()(i, 0) = diff;
			}

        }

        void sum_rows(Matrix<T>& m1) {

        	if ((this->n_rows() != m1.n_rows()))
				printf("sum_rows!\n");

			#pragma omp parallel for
			for (size_t i = 0; i < m1.n_rows(); i++) {
			
				float sum = 0.0f;
				
				for (size_t j = 0; j < m1.n_cols(); j++) {
				
					sum += m1(i, j);
				}

				this->operator()(i, 0) = sum;
			}

        }

        void sum_cols(Matrix<T>& m1) {

        	if ((this->n_cols() != m1.n_cols()))
				printf("sum_cols!\n");

			#pragma omp parallel for
			for (size_t j = 0; j < m1.n_cols(); j++) {
				
				float sum = 0.0f;
				
				for (size_t i = 0; i < m1.n_rows(); i++) {
				
					sum += m1(i, j);
				}

				this->operator()(0, j) = sum;
			}

        }

		void elementwise_function_matrix(T(*func)(T, T), Matrix<T> &m) {

			if ((this->n_rows() != m.n_rows()) || (this->n_cols() != m.n_cols()))
				printf("elementwise_function_matrix, dim mismatch!\n");

			#pragma omp parallel for
			for (size_t i = 0; i < Tensor<T>::elements; i++) {

				Tensor<T>::data[i] = (*func)(Tensor<T>::data[i], m.data[i]);

			}

		}

		void reset_diagonal(void) {

			#pragma omp parallel for
			for (size_t i = 0; i < n_rows(); i++) {
					this->operator()(i, i) = 0;
			}


		}

		static void submatrix(Matrix<T>& in, Matrix<T>& out, size_t x, size_t y) {

			#pragma omp parallel for
			for (size_t i = 0; i < out.n_rows(); i++) {

				for (size_t j = 0; j < out.n_cols(); j++) {

					out(i, j) = in(i + x, j + y);

				}

			}

		}

		static void resize(Matrix<T>& in, Matrix<T>& out) {

			float scale_x = (float)out.n_rows()/(float)in.n_rows();
			float scale_y = (float)out.n_cols()/(float)in.n_cols();

			#pragma omp parallel for
			for (size_t i = 0; i < out.n_rows(); i++) {

				for (size_t j = 0; j < out.n_cols(); j++) {

					// NN
					out(i, j) = in(size_t(i/scale_x), size_t(j/scale_y));

				}

			}

		}


		static void randsubmatrix(Matrix<T>& in, Matrix<T>& out) {

			size_t x = RAN_GEN->uniRandInt() % (in.n_rows() - out.n_rows() + 1);
			size_t y = RAN_GEN->uniRandInt() % (in.n_cols() - out.n_cols() + 1);

			submatrix(in, out, x, y);

		}

        size_t n_rows(void) { return transposed ? Tensor<T>::dim[1] : Tensor<T>::dim[0]; }
        size_t n_cols(void) { return transposed ? Tensor<T>::dim[0] : Tensor<T>::dim[1]; }

		bool transposed;
		DISPLAY_MODE display_mode;
};


/*!
 * \brief matrix of floats type.
 * \author tkornuta
 */
typedef Matrix<float> matrix_float_t;


/*!
 * \brief Shared pointer to matrix of short type.
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::Matrix<float> > matrix_float_ptr_t;


/*!
 * \brief matrix of floats type.
 * \author tkornuta
 */
typedef Matrix<short> matrix_short_t;


/*!
 * \brief Shared pointer to matrix of short type.
 * \author tkornuta
 */
typedef std::shared_ptr< mic::types::Matrix<short> > matrix_short_ptr_t;



}//: namespace types
}//: namespace mic

#endif /*__MATRIX_H__*/
