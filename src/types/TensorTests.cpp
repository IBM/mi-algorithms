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
#include <types/Tensor.hpp>
#include <types/Matrix.hpp>

/*!
 * Tests whether matrix has proper dimensions (2x5).
 */
TEST(Tensor, Dimensions2x5) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 5;
	const size_t K = 13;

	mic::types::Tensor<float> nm({N, M, K});

	ASSERT_EQ(nm.dim(0), N);
	ASSERT_EQ(nm.dim(1), M);
	ASSERT_EQ(nm.dim(2), K);
}


/*!
 * Tests enumeration.
 */
TEST(Tensor, Enumeration2x3) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 3;

	mic::types::Tensor<float> nm({N, M});
	nm.enumerate();

//	std::cout << nm << std::endl;

	for (size_t i =0; i< N*M; i++)
		ASSERT_EQ(nm(i), i);

/*	for(size_t row=0; row<N; row++) {
		for(size_t col=0; col<M; col++)
			std::cout << " nm(" << row << "," << col << ") = " << nm({row,col});
		std::cout << std::endl;
	}//: for*/

	ASSERT_EQ(nm({0,0}), 0);
	ASSERT_EQ(nm({1,0}), 1);
	ASSERT_EQ(nm({0,1}), 2);
	ASSERT_EQ(nm({1,1}), 3);
	ASSERT_EQ(nm({0,2}), 4);
	ASSERT_EQ(nm({1,2}), 5);

}


/*!
 * Tests Tensor serialization.
 */
TEST(Tensor, Serialization) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 5;
	const size_t K = 11;

	mic::types::Tensor<float> nm({N, M, K});
	nm.randn();

	const char* fileName = "saved.txt";
	// Save data
	{
		// Create an output archive
		std::ofstream ofs(fileName);
		boost::archive::text_oarchive ar(ofs);
		// Write data
		ar & nm;
		//std::cout << "Saved matrix = " << nm << std::endl;
	}

	// Restore data
	mic::types::Tensor<float> restored_tensor;
	restored_tensor.randn();

	{
		// Create and input archive
		std::ifstream ifs(fileName);
		boost::archive::text_iarchive ar(ifs);
		// Load data
		ar & restored_tensor;
		//std::cout << "Restored tensor = " << restored_tensor << std::endl;
	}

	// Check dimensions.
	ASSERT_EQ(nm.elements, restored_tensor.elements);
	ASSERT_EQ(nm.dimensions.size(), restored_tensor.dimensions.size());
	for (size_t i =0; i< (size_t)nm.dimensions.size(); i++)
		ASSERT_EQ(nm.dimensions[i], restored_tensor.dimensions[i]);

	// Check values.
	float eps = 1e-8;
	for (size_t i =0; i< (size_t)nm.size(); i++)
		EXPECT_LE(fabs(nm(i) - restored_tensor(i)), eps);

}

/*!
 * Tests im2col.
 */
TEST(Tensor, Im2Col2x3) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 3;

	mic::types::Tensor<float> nm({N, M});
	nm.enumerate();


}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


