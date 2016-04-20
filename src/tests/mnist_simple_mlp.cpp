/*
* @Author: kmrocki
* @Date:   2016-02-24 09:43:05
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-02-24 15:30:27
*/

#include <logger/Log.hpp>
#include <logger/ConsoleOutput.hpp>
using namespace mic::logger;

#include <data_io/MNISTMatrixImporter.hpp>
#include <encoders/MatrixXfMatrixXfEncoder.hpp>
#include <encoders/UIntMatrixXfEncoder.hpp>

#include <mlnn/MultiLayerNeuralNetwork.hpp>

// Using multi layer neural networks
using namespace mic::mlnn;
using namespace mic::types;

int main() {
	// Task parameters.
	size_t iterations = 500;
	size_t batch_size = 100;

	// Set console output.
	ConsoleOutput* co = new ConsoleOutput();
	LOGGER->addOutput(co);


	//[60000, 784]
	// Load the MNIST training...
	mic::data_io::MNISTMatrixImporter training;
	// Manually set paths. DEPRICATED! Used here only for simplification of the test.
	training.setDataFilename("../../../data/mnist/train-images-idx3-ubyte");
	training.setLabelsFilename("../../../data/mnist/train-labels-idx1-ubyte");
	training.setBatchSize(batch_size);

	if (!training.importData())
		return -1;

	// ... and test datasets.
	mic::data_io::MNISTMatrixImporter test;
	// Manually set paths. DEPRICATED! Used here only for simplification of the test.
	test.setDataFilename("../../../data/mnist/t10k-images-idx3-ubyte");
	test.setLabelsFilename("../../../data/mnist/t10k-labels-idx1-ubyte");
	test.setBatchSize(batch_size);

	if (!test.importData())
		return -1;

	// Initialize the encoders.
	mic::encoders::MatrixXfMatrixXfEncoder mnist_encoder(28, 28);
	mic::encoders::UIntMatrixXfEncoder label_encoder(10);

	// Create a simple NN for classification (should give around 95.3% accuracy)
	//MNIST - 28x28 -> 256 -> 100 -> 10
	MultiLayerNeuralNetwork nn;
	nn.layers.push_back(new Linear(28 * 28, 256, batch_size));
	nn.layers.push_back(new ReLU(256, 256, batch_size));
	nn.layers.push_back(new Linear(256, 100, batch_size));
	nn.layers.push_back(new ReLU(100, 100, batch_size));
	nn.layers.push_back(new Linear(100, 10, batch_size));
	nn.layers.push_back(new Softmax(10, 10, batch_size));


	// std::cout << "Before training..." << std::endl;
	// nn.test(test_data);

	std::cout << "Training..." << std::endl;
	double 	learning_rate = 0.005;
	double 	weight_decay = 0;
	MatrixXfPtr encoded_batch, encoded_targets;

	// Perform the training.
	for (size_t ii = 0; ii < iterations; ii++) {
		std::cout<< "[" << std::setw(4) << ii << "/" << std::setw(4) << iterations << "] ";

		// Get random batch [784 x batch_size].
		MNISTBatch rand_batch = training.getRandomBatch();
		encoded_batch  = mnist_encoder.encodeBatch(rand_batch.data());
		encoded_targets  = label_encoder.encodeBatch(rand_batch.labels());

		// Train network with batch.
		nn.train (encoded_batch, encoded_targets, learning_rate, weight_decay);

	}//: for
	std::cout << "Training finished. Calculating performance for both datasets..." << std::endl;

	// Check performance on the test dataset.
	size_t correct = 0;
	test.setNextSampleIndex(0);
	while(!test.isLastBatch()) {

		// Get next batch [784 x batch_size].
		MNISTBatch next_batch = test.getNextBatch();
		encoded_batch  = mnist_encoder.encodeBatch(next_batch.data());
		encoded_targets  = label_encoder.encodeBatch(next_batch.labels());

		// Test network response.
		correct += nn.test(encoded_batch, encoded_targets);

	}
	double test_acc = (double)correct / (double)(test.size());
	std::cout << "Test  : " << std::setprecision(3) << 100.0 * test_acc << " %" << std::endl;

	// Check performance on the training dataset.
	correct = 0;
	training.setNextSampleIndex(0);
	while(!training.isLastBatch()) {

		// Get next batch [784 x batch_size].
		MNISTBatch next_batch = training.getNextBatch();
		encoded_batch  = mnist_encoder.encodeBatch(next_batch.data());
		encoded_targets  = label_encoder.encodeBatch(next_batch.labels());

		// Test network response.
		correct += nn.test(encoded_batch, encoded_targets);

	}
	double train_acc = (double)correct / (double)(training.size());
	std::cout << "Train : " << std::setprecision(3) << 100.0 * train_acc << " %" <<  std::endl;

}
