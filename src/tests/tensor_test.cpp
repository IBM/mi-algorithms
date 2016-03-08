/*!
 * \file tensor_test.cpp
 * \brief Contains program for testing of tensors/Eigen map.
 * \author tkornut
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
 Eigen::MatrixXf operator () (size_t n) {
 float *addr = data+ n*(dim[0]*dim[1]);
 // Map
 return Map<Eigen::MatrixXf>(addr,dim[1],dim[0]).transpose();
 }
 */


/*	std::vector<int> v = { 0, 1, 2, 3, 4, 5 };

	for (const int &i : v) // access by const reference
		std::cout << i << ' ';
	std::cout << '\n';

	for (auto i : v) // access by value, the type of i is int
		std::cout << i << ' ';
	std::cout << '\n';

	for (auto&& i : v) // access by reference, the type of i is int&
		std::cout << i << ' ';
	std::cout << '\n';

	for (int n : { 0, 1, 2, 3, 4, 5 }) // the initializer may be a braced-init-list
		std::cout << n << ' ';
	std::cout << '\n';

	int a[] = { 0, 1, 2, 3, 4, 5 };
	for (int n : a)  // the initializer may be an array
		std::cout << n << ' ';
	std::cout << '\n';

	for (int n : a)
		std::cout << 1 << ' '; // the loop variable need not be used
	std::cout << '\n';*/


/*!
 * \brief Program for testing tensors/Eigen map
 * \author tkornuta
 * @param[in] argc Number of parameters (not used).
 * @param[in] argv List of parameters (not used).
 * @return (not used).
 */
int main(int argc, char* argv[]) {
	// Default sizes of matrices.
	const size_t N = 20;
	const size_t M = 30;
	const size_t K = 4;
	const size_t number_of_multiplications = 100;


	mic::types::Tensor<double> t1({N, M, K});
	for (int i=0; i<N*M*K; i++)
		t1.data[i] = i;

	mic::types::TensorView<double> tv1(t1, {VR(0,0), VR(0,2,14), VR()});
	std::cout << tv1;

	mic::types::TensorView<double> tv2(tv1, {VR(0,0), VR(0,0), VR(1,2)});
	std::cout << tv2;

	mic::types::MatrixXd mv1 = tv1.eval();
	std::cout << mv1;

	mic::types::MatrixXd mv2 = tv2.eval();
	std::cout << mv2;

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

