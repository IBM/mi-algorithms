/*
* @Author: kmrocki
* @Date:   2016-02-24 09:43:05
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-02-24 15:30:27
*/

#include <iostream>

#include <mlnn/importer.h>
#include <mlnn/MultiLayerNeuralNetwork.hpp>

// Using multi layer neural networks
using namespace mic::mlnn;

int main() {

	size_t batch_size = 100;
	MultiLayerNeuralNetwork nn(batch_size);

	//a simple NN for classification (should give around 95.3% accuracy)
	//MNIST - 28x28 -> 256 -> 100 -> 10

	nn.layers.push_back(new Linear(28 * 28, 256, batch_size));
	nn.layers.push_back(new ReLU(256, 256, batch_size));
	nn.layers.push_back(new Linear(256, 100, batch_size));
	nn.layers.push_back(new ReLU(100, 100, batch_size));
	nn.layers.push_back(new Linear(100, 10, batch_size));
	nn.layers.push_back(new Softmax(10, 10, batch_size));

	//[60000, 784]
	// load MNIST data
	std::deque<datapoint> train_data =
		MNISTImporter::importFromFile("../../../data/mnist/train-images-idx3-ubyte",
									  "../../../data/mnist/train-labels-idx1-ubyte");
	std::deque<datapoint> test_data =
		MNISTImporter::importFromFile("../../../data/mnist/t10k-images-idx3-ubyte",
									  "../../../data/mnist/t10k-labels-idx1-ubyte");
	size_t classes = 10;

	// std::cout << "Before training..." << std::endl;

	// nn.test(test_data);

	std::cout << "Training..." << std::endl;


	double 	learning_rate = 0.005;
	double 	weight_decay = 0;
	size_t iterations = 500;

	nn.train(train_data, learning_rate, weight_decay, iterations, classes);

	std::cout << "Training finished. Calculating performance for both datasets..." << std::endl;

	double train_acc = nn.test(train_data, classes);
	double test_acc = nn.test(test_data, classes);

	std::cout << std::setprecision(2) << "Train : " << 100.0 * train_acc << " %" <<  std::endl;
	std::cout << std::setprecision(2) << "Test  : " << 100.0 * test_acc << " %" << std::endl;

}
