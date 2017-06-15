/*!
 * @file: MatrixTests.cpp
 * @Author: Tomasz Kornuta <tkornut@us.ibm.com>
 * @Date:   Nov 22, 2016
 *
 * Copyright (c) 2016, IBM Corporation. All rights reserved.
 *
 */

#include <gtest/gtest.h>

#include <types/Tensor.hpp>
#include <types/Matrix.hpp>

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

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

	mic::types::Tensor<float> nm({N, M});
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
	mic::types::Tensor<float> restored_tensor;
	restored_tensor.randn();

	{
		// Create and input archive
		std::ifstream ifs(fileName);
		boost::archive::text_iarchive ar(ifs);
		// Load data
		ar & restored_tensor;
//		std::cout << "Restored tensor = " << restored_tensor << std::endl;
	}

	for (size_t i =0; i< (size_t)nm.size(); i++)
		ASSERT_EQ(nm(i), restored_tensor(i));

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


