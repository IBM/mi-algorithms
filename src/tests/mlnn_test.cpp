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

#include <types/Batch.hpp>

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
	nn.addLayer(new Linear(10, 20, 1));
	nn.addLayer(new ReLU(20, 20, 1));
	nn.addLayer(new Linear(20, 4, 1));
//	nn.addLayer(new Softmax(4, 4, 1));
	nn.addLayer(new Regression(4, 4, 1));

/*	// Generate sample.
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
	}//: for*/

	Batch<MatrixXf, MatrixXf> batch;

	// Generate batch.
	for(size_t i=0; i< 10; i++) {
		// Generate "pose".
		MatrixXfPtr pose (new MatrixXf(10, 1));
		pose->setZero();
		(*pose)(i,0)=1;
		batch.data().push_back(pose);

		// Generate desired action .
		MatrixXfPtr action (new MatrixXf(4, 1));
		action->setZero();
//		(*action)(i%4,0)= 1;
		(*action)(i%4,0)= 10*((i%3) - 1.0);
		batch.labels().push_back(action);

		// Add index.
		batch.indices().push_back(i);
	}//: for

	// Training.
	size_t iteration = 0;
	while (iteration < 100000) {
		Sample <MatrixXf, MatrixXf> sample = batch.getRandomSample();
		//std::cout << "[" << iteration++ << "]: sample (" << sample.index() << "): "<< sample.data()->transpose() << "->" << sample.label()->transpose() << std::endl;

		float loss = nn.train(sample.data(), sample.label(), 0.005, 0.0);
		// Compare results
		MatrixXf predictions = (*nn.getPredictions());
		if (iteration % 1000 == 0){
			std::cout<<"[" << iteration << "]: Loss        : " << loss << std::endl;
		}
		//std::cout<<"Targets     : " << sample.label()->transpose() << std::endl;
		//std::cout<<"Predictions : " << predictions.transpose() << std::endl << std::endl;
		iteration++;
	}//: while

	iteration = 0;
	while (iteration < 10) {
		Sample <MatrixXf, MatrixXf> sample = batch.getRandomSample();
		std::cout << "[" << iteration++ << "]: sample (" << sample.index() << "): "<< sample.data()->transpose() << "->" << sample.label()->transpose() << std::endl;

		float loss = nn.test(sample.data(), sample.label());
		// Compare results
		MatrixXf predictions = (*nn.getPredictions());
		std::cout<<"Loss        : " << loss << std::endl;
		std::cout<<"Targets     : " << sample.label()->transpose() << std::endl;
		std::cout<<"Predictions : " << predictions.transpose() << std::endl << std::endl;

	}//: while

}
