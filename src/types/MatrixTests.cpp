/*!
 * @file: MatrixTests.cpp
 * @Author: Tomasz Kornuta <tkornut@us.ibm.com>
 * @Date:   Nov 22, 2016
 *
 * Copyright (c) 2016, IBM Corporation. All rights reserved.
 *
 */

#include <gtest/gtest.h>

#include <types/Matrix.hpp>

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/*!
 * Tests whether matrix has proper dimensions (2x5).
 */
TEST(Matrix, Dimensions2x5) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 5;

	mic::types::Matrix<float> nm(N, M);

	ASSERT_EQ(nm.rows(), N);
	ASSERT_EQ(nm.cols(), M);
}


/*!
 * Tests matrix serialization.
 */
TEST(Matrix, Serialization) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 5;

	mic::types::Matrix<float> nm(N, M);
	nm.randn();

	const char* fileName = "saved.txt";
	// Save data
	{
		// Create an output archive
		std::ofstream ofs(fileName);
		boost::archive::text_oarchive ar(ofs);
		// Write data
		ar & nm;
//		std::cout << "Saved matrix = " << nm << std::endl;
	}

	// Restore data
	mic::types::Matrix<float> restored_mat;
	restored_mat.randn();

	{
		// Create and input archive
		std::ifstream ifs(fileName);
		boost::archive::text_iarchive ar(ifs);
		// Load data
		ar & restored_mat;
//		std::cout << "Restored matrix = " << restored_mat << std::endl;
	}

	for (size_t i =0; i< (size_t)nm.size(); i++)
		ASSERT_EQ(nm(i), restored_mat(i));
}


/*!
 * Tests enumeration.
 */
TEST(Matrix, Enumeration4x5) {
	// Default sizes of matrices.
	const size_t N = 4;
	const size_t M = 5;

	mic::types::Matrix<float> nm(N, M);
	nm.enumerate();

	for (size_t i =0; i< N*M; i++)
		ASSERT_EQ(nm(i), i);
}

/*!
 * Tests functions added to make Eigen-derived Matrix as much compatible to Armadillo-derived Matrix as possible.
 */
TEST(Matrix, ArmadilloCompatibilityTest) {
	// Default sizes of matrices.
	const size_t N = 4;
	const size_t M = 5;

	mic::types::Matrix<float> nm(N, M);
	nm.zeros();

	for (size_t i =0; i< N*M; i++)
		ASSERT_EQ(nm[i], i);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


