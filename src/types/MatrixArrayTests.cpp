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
 * @file: MatrixArrayTests.cpp
 * @Author: Tomasz Kornuta <tkornut@us.ibm.com>
 * @Date:   Nov 22, 2016
 *
 * Copyright (c) 2016, IBM Corporation. All rights reserved.
 *
 */


#include <gtest/gtest.h>

#include <types/MatrixArray.hpp>

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

/*!
 * Tests whether matrices have proper dimensions.
 */
TEST(MatrixArray, Dimensions2x3x4) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 3;
	const size_t B = 4;

	// Test MatrixArray operations.
	mic::types::MatrixArray<double> ma("test_array");

	// Two different methods of adding matrices to array.
	ma.add (
				{
					std::make_tuple ( "x", M, B ), 	// input
					std::make_tuple ( "y", N, B ) 		// output
				} );

	ma.add (std::make_tuple ( "w", N, M ));

	ASSERT_EQ(ma["x"]->rows(), M);
	ASSERT_EQ(ma["x"]->cols(), B);
	ASSERT_EQ(ma["y"]->rows(), N);
	ASSERT_EQ(ma["y"]->cols(), B);
	ASSERT_EQ(ma["w"]->rows(), N);
	ASSERT_EQ(ma["w"]->cols(), M);
}


/*!
 * Tests matrix array serialization.
 */
TEST(MatrixArray, Serialization) {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 3;

	// Test MatrixArray operations.
	mic::types::MatrixArray<double> ma1("test_array");

	//  Add single matrix.
	ma1.add (std::make_tuple ( "w", N, M ));
	ma1["w"]->randn();

//	std::cout<<"Saved MatrixArray = " << ma1;
	const char* fileName = "saved.txt";
	// Save data.
	{
		// Create an output archive.
		std::ofstream ofs(fileName);
		boost::archive::text_oarchive ar(ofs);
		// Write data
		ar & ma1;
	}

	// Restore data.
	mic::types::MatrixArray<double> restored_ma;
	{
		// Create and input archive.
		std::ifstream ifs(fileName);
		boost::archive::text_iarchive ar(ifs);
		// Load data.
		ar & restored_ma;
//		std::cout << "Restored MatrixArray = " << restored_ma << std::endl;
	}

	for (size_t i =0; i< (size_t)ma1["w"]->size(); i++)
		ASSERT_EQ((*ma1["w"])(i), (*restored_ma["w"])(i));
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


