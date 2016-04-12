/*!
 * \file mlnn_test.cpp
 * \brief Contains program for testing creation/serialization of multi-layer neural networks.
 * \author tkornut
 * \date Feb 17, 2016
 */

#include <iostream>

#include <mlnn/MultiLayerNeuralNetwork.hpp>

#include <types/MatrixArray.hpp>

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


// Using multi-layer neural networks
using namespace mic::mlnn;

int main() {
	// Default sizes of matrices.
	const size_t N = 2;
	const size_t M = 3;
	const size_t B = 4;

	// Test MatrixArray operations.
	mic::types::MatrixArray<double> ma1("test_array");

	ma1.add (
				{
					std::make_tuple ( "x", M, B ), 	// input
					std::make_tuple ( "y", N, B ) 		// output
				} );

	ma1.add (std::make_tuple ( "w", M, N ));

	ma1["x"]->normRandReal(1, 0.00001);
	ma1["w"]->normRandReal(1, 0.00001);


	std::cout<<"Saved MatrixArray = " << ma1;

	const char* fileName = "saved.txt";
	// Save data
	{
		// Create an output archive
		std::ofstream ofs(fileName);
		boost::archive::text_oarchive ar(ofs);
		// Write data
		ar & ma1;
	}

	std::cout<<"Calculated MatrixArray = " << ma1;
	(*ma1["y"]) = (*ma1["w"]) * (*ma1["x"]);


	// Restore data
	mic::types::MatrixArray<double> restored_ma;
	{
		// Create and input archive
		std::ifstream ifs(fileName);
		boost::archive::text_iarchive ar(ifs);
		// Load data
		ar & restored_ma;
		std::cout << "Restored MatrixArray = " << restored_ma << std::endl;
	}


	/*
	size_t batch_size = 100;
	MultiLayerNeuralNetwork nn(batch_size);

	nn.layers.push_back(new Linear(28 * 28, 256, batch_size));
	nn.layers.push_back(new ReLU(256, 256, batch_size));
	nn.layers.push_back(new Linear(256, 100, batch_size));
	nn.layers.push_back(new ReLU(100, 100, batch_size));
	nn.layers.push_back(new Linear(100, 10, batch_size));
	nn.layers.push_back(new Softmax(10, 10, batch_size));
	*/



}
