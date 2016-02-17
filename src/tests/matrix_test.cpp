/*!
 * \file matrix_test.cpp
 * \brief Contains test
 * \author tkornut
 * \date Feb 17, 2016
 */

#include <types/MatrixTypes.hpp>
#include <iostream>
#include <random>
#include <sys/time.h>

#ifdef OpenBLAS_FOUND
#include <cblas.h>
#endif

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
		Eigen::MatrixXf& __restrict b, bool aT = false, bool bT = false ) {

	enum CBLAS_TRANSPOSE transA = aT ? CblasTrans : CblasNoTrans;
	enum CBLAS_TRANSPOSE transB = bT ? CblasTrans : CblasNoTrans;

	size_t M = c.rows();
	size_t N = c.cols();
	size_t K = aT ? a.rows() : a.cols();

	float alpha = 1.0f;
	float beta = 1.0f;

	size_t lda = aT ? K : M;
	size_t ldb = bT ? N : K;
	size_t ldc = M;

	cblas_sgemm( CblasColMajor, transA, transB, M, N, K, alpha,
				 a.data(), lda,
				 b.data(), ldb, beta, c.data(), ldc );


}
#endif /* USE_BLAS */


void randn(Eigen::MatrixXf& m, float mean, float stddev) {

	// random number generator
	// unfortunately, Eigen does not implement normal distribution
	// TODO: make it cleaner, more parallel

	std::random_device rd;
	std::mt19937 mt(rd());
	std::normal_distribution<> randn(mean, stddev);

	for (int i = 0; i < m.rows(); i++) {
		for (int j = 0; j < m.cols(); j++) {
			m.coeffRef(i, j) = randn(mt);
		}
	}

}


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
	const size_t N = 64;
	const size_t M = 256;
	const size_t K = 128;
	const size_t number_of_multiplications = 100;

	// Matrices.
	Eigen::MatrixXf nm(N, M);
	Eigen::MatrixXf mk(M, K);
	Eigen::MatrixXf nk(N, K);

	// Initialize matrices with random numbers.
	randn(nm, 1, 0.001);
	randn(mk, 0, 1.0);

	Timer timer;
	timer.start();

	for (size_t i = 0; i < number_of_multiplications; i++) {
	// Multiplication.
#ifdef OpenBLAS_FOUND
		BLAS_mmul(nk, nm, mk);
#else
		nk = nm * mk;
#endif
	}//: for

	double time = timer.end();
	std::cout  <<  "time = " << time <<  std::endl;

}//: main


