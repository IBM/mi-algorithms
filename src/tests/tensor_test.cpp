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

	// Create tensor.
/*	mic::types::Tensor<double> t1({N, M, K});
	double* data = t1.data();
	for (int i=0; i<t1.size(); i++)
		data[i] = i;

	// Create different views on tensor.
	mic::types::TensorView<double> tv1(t1, {VR(0), VR(0,2,14), VR()});
	std::cout << tv1;

	mic::types::TensorView<double> tv2(tv1, {VR(0,0), VR(0), VR(1,2)});
	std::cout << tv2;

	mic::types::TensorView<double> tv3(tv1, {VR(0), VR(1), VR(2,2)});
	std::cout << tv2;

	// Get (sub)tensor, (sub)matrix, (sub)vector and scalar.
	mic::types::Tensor<double> t2 = tv1.getTensor();

	mic::types::MatrixXd mat1 = tv1.getMatrix();
	std::cout << mat1 << std::endl;

	mic::types::VectorXd vec1 = tv2.getVector();
	std::cout << vec1 << std::endl;

	double s1 = tv3.getScalar();
	std::cout << s1 << std::endl;*/

	mic::types::Tensor<double> t1({N*M*K*N});
	t1.enumerate();
	//t1({2}) = 33.1;
	//t1(4) = 55;
	//std::cout << t1;

	//double* data = t1.data();
	//data[10] = 1000.1;

	t1.resizeForced({N, M, K});
	//t1({2,2,2}) = 222.1;
	//t1(5) = 666;
	//std::cout << t1;


	//for(size_t l=0; l<K; l++)
	for(size_t k=0; k<K; k++)
		for(size_t m=0; m<M; m++)
			for(size_t n=0; n<N; n++) {
				//mic::types::TensorView<double> tv(t1, {VR(n), VR(m), VR(k)});
				//std::cout << tv ;
				//double d = tv.getScalar();
				double d = t1({n,m,k});
				std::cout << d << ", ";
			}//: for
	std::cout << std::endl;

	t1.resizeForced({4,3,4});
	t1.enumerate();
	std::cout << t1;

	mic::types::Tensor<double> t2 = t1.block({{0,2},{0,2},{1,3}});
	std::cout << t2;


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

