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
#include <types/TensorTypes.hpp>

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>


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

	// Create new tensor.
	mic::types::TensorXd t0({N, M});
	t0.enumerate();
	std::cout << "t0 = " << t0 << std::endl;
	for(size_t row=0; row<N; row++) {
		for(size_t col=0; col<M; col++)
			std::cout << " t0(" << row << "," << col << ") = " << t0({row,col});
		std::cout << std::endl;
	}//: for


	// Create new tensor.
	mic::types::TensorXd t1({N*M*K});
	t1.enumerate();
	// Different methods of setting the new value of data.
	t1({2}) = 33.1;
	t1(4) = 55;
	double* data = t1.data();
	data[10] = 1000.1;
	std::cout << "t1 = " << t1 << std::endl;

	// Conservative resize - with keeping the old values. Number of elements must remained unchanged!
	t1.conservativeResize({N, M, K});
	t1({2,2,2}) = 222.1;
	t1(5) = 666;
	std::cout << "resized t1 = " << t1 << std::endl;

	// Get access to elements one by one.
	std::cout << "Printing elements one by one (getIndex): ";
	for(size_t k=0; k<K; k++)
		for(size_t m=0; m<M; m++)
			for(size_t n=0; n<N; n++) {
				double d = t1({n,m,k});
				std::cout << d << ", ";
			}//: for
	std::cout << std::endl;

	std::cout << "Printing elements one by one: ";
	for(size_t k=0; k<t1.size(); k++){
		double d = t1(k);
		std::cout << d << ", ";
	}//: for
	std::cout << std::endl;


	// Resize and change the total number of elements.
	t1.resize({4,4,4,4});
	t1.enumerate();
	std::cout << "resized t1 = " << t1 << std::endl;

	// Get a subtensor.
	mic::types::TensorXd t2 = t1.block({{0,3},{0},{0},{0,1}});
	std::cout << "subtensor t2 = " << t2 << std::endl;

	// Flatten both tensors.
	t1.flatten();
	std::cout << "flattened t1 = " << t1 << std::endl;
	t2.flatten();
	std::cout << "flattened t2 = " << t2 << std::endl;

	// Concatenate tensors - add t2 two times.
	t1.concatenate({t2,t2});
	std::cout << " t1 concatenated with {t2,t2} = " << t1 << std::endl;
	t1.resize({3,2});
	std::cout << " t1 resized to {3, 2} = " << t1 << std::endl;

	// Map 2D tensor to matrix.
	mic::types::Matrix<double> mat = t1;
	std::cout << "matrix from tensor = \n" <<  mat << std::endl;
	mat *= 2;
	std::cout << "matrix *2 = \n" <<  mat << std::endl;

	// Map matrix to 2D tensor.
	mic::types::TensorXd t4 = mat;
	std::cout << "tensor from matrix = " << t4 << std::endl;

	const char* fileName = "saved.txt";

	// Save data
	{
		// Create an output archive
		std::ofstream ofs(fileName);
		boost::archive::text_oarchive ar(ofs);
		// Write data
		ar & t4;
	}

	// Restore data
	mic::types::TensorXd restored_t5;

	{
		// Create and input archive
		std::ifstream ifs(fileName);
		boost::archive::text_iarchive ar(ifs);
		// Load data
		ar & restored_t5;
		std::cout << "restored = " << restored_t5 << std::endl;
	}

	// Tensor pointer.
	mic::types::TensorXfPtr ten_ptr (new mic::types::TensorXf({N, M, K}));
	ten_ptr->enumerate();
	std::cout << "Pointer to tensor = " << *ten_ptr << std::endl;

} //: main

