/*!
 * \file tensor_test.cpp
 * \brief Contains program for testing of tensors/Eigen map.
 * \author tkornuta
 * \date Feb 17, 2016
 */

#include <iostream>
#include <iomanip>

#include <sys/time.h>

#include <types/MatrixTypes.hpp>

#include <types/TensorView.hpp>

using namespace Eigen;
using namespace std;


/*
Eigen Concatenation
Horizontally:

MatrixXd C(A.rows(), A.cols()+B.cols());
C << A, B;

Vertically:

MatrixXd D(A.rows()+B.rows(), A.cols());
D << A,
     B;
 */

/*!
 * \brief Program for testing tensors/Eigen map
 * \author tkornuta
 * @param[in] argc Number of parameters (not used).
 * @param[in] argv List of parameters (not used).
 * @return (not used).
 */
int main(int argc, char* argv[]) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 3;
	const size_t K = 4;

	mic::types::Tensor<double> t1({N*M*K*N});
	t1.enumerate();
	t1({2}) = 33.1;
	t1(4) = 55;
	std::cout << t1;

	double* data = t1.data();
	data[10] = 1000.1;

	t1.resize({N, M, K});
	t1({2,2,2}) = 222.1;
	t1(5) = 666;
	std::cout << t1;


	//for(size_t l=0; l<K; l++)
	for(size_t k=0; k<K; k++)
		for(size_t m=0; m<M; m++)
			for(size_t n=0; n<N; n++) {
				double d = t1({n,m,k});
				std::cout << d << ", ";
			}//: for
	std::cout << std::endl;

	t1.resize({4,4,4});
	t1.enumerate();
	std::cout << t1;

	mic::types::Tensor<double> t2 = t1.block({{0,1},{1,2},{1,3}});
	//t2.resizeForced({2,2});
	std::cout << t2;

	t1.flatten();
	t2.flatten();

	std::cout << t1;
	t1.concatenate({t2,t2});
	t1.resize({3,2});
	std::cout << t1;

	mic::types::Matrix<double> mat = t1;
	std::cout << mat << std::endl;


	mic::types::Tensor<double> t4 = mat;
	std::cout << t4;

	// Matrices.
//	mic::types::Matrix<float> nm(N, M);
//	mic::types::MatrixXf xxx = nm;
/*	mic::types::MatrixXf mk(M, K);
	mic::types::MatrixXf nk(N, K);*/

	// Initialize matrices with random numbers.
//	nm.normRandReal(1, 0.001);
//	mk.normRandReal(1, 0.001);
//	std::cout << "nm=\n" << nm << std::endl;

	/*	int data[] = {1,2,3,4,5,6,7,8,9};
	 Map<RowVectorXi> v(data,4);
	 cout << "The mapped vector v is: " << v << "\n";
	 v(2) = 100;
	 cout << "The mapped vector v is: " << v << "\n";

	 new (&v) Map<RowVectorXi>(data+4,5);
	 cout << "Now v is: " << v << "\n";*/

	// Adding test.
	/*	Eigen::VectorXf vect(N);
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

	 mic::data_utils::Timer timer;
	 timer.start();

	 for (size_t i = 0; i < number_of_multiplications; i++) {

	 // Multiplication.
	 nk = nm * mk;

	 }//: for
	 double time = timer.end();
	 std::cout <<"nk=\n" << nk <<  std::endl;

	 std::cout  <<  "Multiplication time = " << time <<  std::endl;
	 */
} //: main

