/*!
 * \file matrix_test.cpp
 * \brief Contains test
 * \author tkornut
 * \date Feb 17, 2016
 */


#include <iostream>
#include <iomanip>

#include <sys/time.h>

#include <types/MatrixTypes.hpp>
#include <data_utils/functions.h>


namespace mic {
namespace types {


}//: namespace types
}//: namespace mic


class Timer {

	public:

		Timer() = default;
		~Timer() = default;

		void start(void) {

			gettimeofday(&s, NULL);
		}

		double end(void) {

		    struct timeval  diff_tv;
		    gettimeofday(&e, NULL);

		    diff_tv.tv_usec = e.tv_usec - s.tv_usec;
		    diff_tv.tv_sec = e.tv_sec - s.tv_sec;

		    if (s.tv_usec > e.tv_usec) {
		        diff_tv.tv_usec += 1000000;
		        diff_tv.tv_sec--;
		    }

		    return (double) diff_tv.tv_sec + ( (double) diff_tv.tv_usec / 1000000.0);

		}

	protected:

		struct timeval s;
		struct timeval e;
};

#ifdef OpenBLAS_FOUND
// c = a * b
void BLAS_mmul( Eigen::MatrixXf& __restrict c, Eigen::MatrixXf& __restrict a,
		Eigen::MatrixXf& __restrict b, bool aT = false, bool bT = false) {

	// Both matrices are not transposed - as default.
	// Transposition is a different class in EIGEN.

	enum CBLAS_TRANSPOSE transA = aT ? CblasTrans : CblasNoTrans;
	enum CBLAS_TRANSPOSE transB = bT ? CblasTrans : CblasNoTrans;

	size_t M = c.rows();
	size_t N = c.cols();
	size_t K = aT ? a.rows() : a.cols();

	float alpha = 1.0f;
	float beta = 0.0f;

	size_t lda = aT ? K : M;
	size_t ldb = bT ? N : K;
	size_t ldc = M;

	cblas_sgemm( CblasColMajor, transA, transB, M, N, K, alpha,
				 a.data(), lda,
				 b.data(), ldb, beta, c.data(), ldc );


}

// returns a * b
Eigen::MatrixXf BLAS_mmultilpy(Eigen::MatrixXf& __restrict a, Eigen::MatrixXf& __restrict b) {

	// Both matrices are not transposed - as default.
	// Transposition is a different class in EIGEN.

	size_t M = a.rows();
	size_t K =  a.cols();
	size_t N = b.cols();

	Eigen::MatrixXf c(M,N);

	cblas_sgemm( CblasColMajor, CblasNoTrans, CblasNoTrans, M, N, K, 1.0,
				 a.data(), M,
				 b.data(), K, 0.0, c.data(), M );

//	std::cout <<"c=\n" << c <<  std::endl;

	return c;
}

#endif /* USE_BLAS */




/*!
 * \brief Main program function for testing operations on matrices.
 * \author tkornuta
 * @param[in] argc Number of parameters (not used).
 * @param[in] argv List of parameters (not used).
 * @return (not used).
 */
int main(int argc, char* argv[]) {

#ifdef OpenBLAS_FOUND
	std::cout << "Using OpenBLAS-boosted matrix multiplication\n";
#else
	std::cout << "Using pure EIGEN matrix multiplication\n";
#endif

	// Default sizes of matrices.
	const size_t N = 200;
	const size_t M = 300;
	const size_t K = 400;
	const size_t number_of_multiplications = 100;

	// Matrices.
	mic::types::MatrixXf nm(N, M);
	mic::types::MatrixXf mk(M, K);
	mic::types::MatrixXf nk(N, K);

	// Initialize matrices with random numbers.
	nm.normRandReal(1, 0.001);
	mk.normRandReal(1, 0.001);
	std::cout <<"nm=\n" << nm <<  std::endl;

	// Adding test.
	Eigen::VectorXf vect(N);
	vect.setRandom();
	std::cout <<"vect=\n" << vect <<  std::endl;
	nm.colwise() += vect;
	std::cout <<"nm+=vect\n" << nm <<  std::endl;

	// Elementwise function test.
	mic::types::MatrixXf nm2(N, M);
	nm2.normRandReal(1, 0.00001);
	std::cout <<"nm2=\n" << nm2 <<  std::endl;
	nm.elementwiseFunctionMatrix(&_add, nm2);
	std::cout <<"nm+=nm2\n" << nm <<  std::endl;


	std::cout <<"mk=\n" << mk <<  std::endl;

	Timer timer;
	timer.start();

	for (size_t i = 0; i < number_of_multiplications; i++) {
		//std::cout <<  std::setfill('=') << std::setw(80)<< "=\n" <<std::setfill(' ');

		// Multiplication.
		nk = nm * mk;

/*#ifdef OpenBLAS_FOUND
		BLAS_mmul(nk, nm, mk);
		//nk = BLAS_mmultilpy(nm, mk);
		//std::cout <<"blas nk=\n" << nk <<  std::endl;
#else
		nk = nm * mk;
		//std::cout <<"eigen nk=\n" << nk <<  std::endl;
#endif*/
	}//: for
	double time = timer.end();
	std::cout <<"nk=\n" << nk <<  std::endl;

	std::cout  <<  "Multiplication time = " << time <<  std::endl;

}//: main


