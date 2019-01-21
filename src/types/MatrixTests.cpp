/*!
 * Copyright (C) tkornuta, IBM Corporation 2015-2019
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*!
 * @file: MatrixTests.cpp
 * @Author: Tomasz Kornuta <tkornut@us.ibm.com>
 * @Date:   Nov 22, 2016
 *
 * Copyright (c) 2016, IBM Corporation. All rights reserved.
 *
 */

#include <gtest/gtest.h>

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

// Redefine word "public" so every class field/method will be accessible for tests.
#define private public
#include <types/Matrix.hpp>

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
TEST(Matrix, Enumeration2x3) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 3;

	mic::types::Matrix<float> nm(N, M);
	nm.enumerate();

	for (size_t i =0; i< N*M; i++)
		ASSERT_EQ(nm(i), i);

	for(size_t row=0; row<N; row++) {
		for(size_t col=0; col<M; col++)
			std::cout << " nm(" << row << "," << col << ") = " << nm(row,col);
		std::cout << std::endl;
	}//: for

	ASSERT_EQ(nm(0,0), 0);
	ASSERT_EQ(nm(0,1), 2);
	ASSERT_EQ(nm(0,2), 4);
	ASSERT_EQ(nm(1,0), 1);
	ASSERT_EQ(nm(1,1), 3);
	ASSERT_EQ(nm(1,2), 5);

	nm.resize(M,N);

	for(size_t row=0; row<M; row++) {
		for(size_t col=0; col<N; col++)
			std::cout << " nm(" << row << "," << col << ") = " << nm(row,col);
		std::cout << std::endl;
	}//: for

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
		ASSERT_EQ(nm[i], 0);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


