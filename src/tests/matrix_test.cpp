/*!
 * \file matrix_test.cpp
 * \brief Contains program for testing multiplication of matrices with/without OpenBLAS.
 * \author tkornut
 * \date Feb 17, 2016
 */


#include <iostream>
#include <iomanip>

#include <sys/time.h>

#include <types/MatrixTypes.hpp>

#include <data_utils/functions.h>
#include <data_utils/Timer.hpp>

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


#ifdef OpenBLAS_FOUND
#include <cblas.h>
#endif


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
 * \brief Program for testing multiplication of matrices with/without OpenBLAS.
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
	const size_t N = 2;
	const size_t M = 3;
	const size_t K = 4;
	const size_t number_of_multiplications = 100;

	// Matrices.
	mic::types::Matrix<float> nm(N, M);
	mic::types::Matrix<float> mk(M, K);
	mic::types::Matrix<float> nk(N, K);

	// Initialize matrices with random numbers.
	nm.normRandReal(1, 0.001);
	mk.normRandReal(1, 0.001);
	//std::cout <<"nm=\n" << nm <<  std::endl;

	// Initialize vector.
	mic::types::VectorXf vect(N);
	vect.setRandom();

	// Examples of mapping from vectors to matrices.
	mic::types::MatrixXf vect_copy = vect;
	mic::types::MatrixXf vect_copy2;
	vect_copy2 = (mic::types::MatrixXf)vect;

	// Adding test.
	vect_copy2 += vect;
	std::cout <<"vect_copy=\n" << vect_copy <<  std::endl;
	std::cout <<"vect_copy2=\n" << vect_copy2 <<  std::endl;

	std::cout <<"vect=\n" << vect <<  std::endl;
	nm.colwise() += vect;
	std::cout <<"nm+=vect\n" << nm <<  std::endl;



	// Elementwise function test.
	mic::types::MatrixXf nm2(N, M);
	nm2.normRandReal(1, 0.00001);
	std::cout <<"nm2=\n" << nm2 <<  std::endl;
	nm.elementwiseFunctionMatrix(&_add, nm2);
	std::cout <<"nm+=nm2\n" << nm <<  std::endl;

	//std::cout <<"mk=\n" << mk <<  std::endl;

	mic::data_utils::Timer timer;
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
	//std::cout <<"nk=\n" << nk <<  std::endl;

	std::cout  <<  "Multiplication time = " << time <<  std::endl;


	const char* fileName = "saved.xml";

	// Save data
	{
		// Create an output archive
		std::ofstream ofs(fileName);
		boost::archive::text_oarchive ar(ofs);
		// Write data
		ar & nm;
		std::cout << "Saved matrix = " << nm << std::endl;

	}

	// Restore data
	mic::types::MatrixXd restored_mat;

	{
		// Create and input archive
		std::ifstream ifs(fileName);
		boost::archive::text_iarchive ar(ifs);
		// Load data
		ar & restored_mat;
		std::cout << "Restored matrix = " << restored_mat << std::endl;
	}

}//: main


