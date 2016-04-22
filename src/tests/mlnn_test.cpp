/*!
 * \file mlnn_test.cpp
 * \brief Contains program for testing creation/serialization of multi-layer neural networks.
 * \author tkornut
 * \date Feb 17, 2016
 */

#include <logger/Log.hpp>
#include <logger/ConsoleOutput.hpp>
using namespace mic::logger;

#include <iostream>

#include <mlnn/MultiLayerNeuralNetwork.hpp>

#include <fstream>
// Include headers that implement a archive in simple text format
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>


// Using multi-layer neural networks
using namespace mic::mlnn;
using namespace mic::types;

int main() {
	// Set console output.
	LOGGER->addOutput(new ConsoleOutput());

	// Create 1 layer (linear) network.
	MultiLayerNeuralNetwork nn("simple_linear_network");
	nn.addLayer(new Linear(36, 4, 1));
	nn.addLayer(new Regression(4, 4, 1));

	// Generate sample.
	MatrixXfPtr sample (new MatrixXf(36, 1));
	(*sample) << 0,   0,   0,   0,   0,   0,   0,  10,   0,   0, -10,   0, -10,   0, -10,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0;
	std::cout<<"Sample: " << sample->transpose() << std::endl;

	// Forward the sample and get prediction - used later as target.
	nn.forward(*sample);
	MatrixXfPtr targets (new MatrixXf(*nn.getPredictions()));

	// Train several times for the same state and rewards.
	for(size_t i=0; i< 10; i++) {
		float loss = nn.train(sample, targets, 0.005, 0.0);
		// Compare results
		MatrixXf predictions = (*nn.getPredictions());
		std::cout<<"Loss        : " << loss << std::endl;
		std::cout<<"Targets     : " << targets->transpose() << std::endl;
		std::cout<<"Predictions : " << predictions.transpose() << std::endl;
	}//: for


}
