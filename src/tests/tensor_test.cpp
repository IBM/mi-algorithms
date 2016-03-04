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

#include <data_utils/functions.h>
#include <data_utils/Timer.hpp>



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

	mic::data_utils::Timer timer;
	timer.start();

	for (size_t i = 0; i < number_of_multiplications; i++) {

		// Multiplication.
		nk = nm * mk;

	}//: for
	double time = timer.end();
	std::cout <<"nk=\n" << nk <<  std::endl;

	std::cout  <<  "Multiplication time = " << time <<  std::endl;

}//: main


