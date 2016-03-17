/*
* @Author: kmrocki
* @Date:   2016-03-10 09:43:05
* @Last Modified by:   kmrocki
* @Last Modified time: 2016-03-11 10:31:37
*/

#include <nn/importer.h>
#include <nn/layers.h>
#include <nn/nn.h>
#include <iostream>
#include <nn/nn_utils.h>

#ifdef OpenBLAS_FOUND
#include <cblas.h>
#endif


int main() {

	//using OpenMP - limit OpenBLAS to core
#ifdef OpenBLAS_FOUND
	//openblas_set_num_threads(1);
#endif


	// load MNIST data
	std::deque<datapoint> train_data =
		MNISTImporter::importFromFile("../../../data/mnist/train-images-idx3-ubyte",
									  "../../../data/mnist/train-labels-idx1-ubyte");
	std::deque<datapoint> test_data =
		MNISTImporter::importFromFile("../../../data/mnist/t10k-images-idx3-ubyte",
									  "../../../data/mnist/t10k-labels-idx1-ubyte");

	// 20 Epochs
	// Train : 99.99 %
	// Test  : 99.61 %

	size_t 	input_channels = 1;
	size_t 	filter_size[] = {3, 3, 3, 3, 3};
	size_t 	filters[] = {32, 32, 128, 128, 256};

	size_t 	pooling_window = 2;
	float 	dropout = 0.5f;
	bool 	apply_dropout = true;

	size_t 	epochs = 500;
	size_t 	batch_size = 16;
	double 	learning_rate = 1e-2;
	double 	weight_decay = 0;

	NN nn(batch_size);

	//CONV 3x3 -> CONV 3x3 -> POOL 2x
	nn.layers.push_back(new Convolution(train_data[0].x.rows(), input_channels, filter_size[0], filters[0], batch_size));
	nn.layers.push_back(new ReLU(nn.layers.back()->y.rows(), nn.layers.back()->y.rows(), batch_size));
	nn.layers.push_back(new Convolution(nn.layers.back()->y.rows() / filters[0], filters[0], filter_size[1], filters[1], batch_size));
	nn.layers.push_back(new ReLU(nn.layers.back()->y.rows(), nn.layers.back()->y.rows(), batch_size));
	nn.layers.push_back(new Pooling(nn.layers.back()->y.rows(), pooling_window, filters[2], batch_size));

	//CONV 3x3 -> CONV 3x3 -> POOL 2x
	nn.layers.push_back(new Convolution(nn.layers.back()->y.rows() / filters[1], filters[1], filter_size[2], filters[2], batch_size));
	nn.layers.push_back(new ReLU(nn.layers.back()->y.rows(), nn.layers.back()->y.rows(), batch_size));
	nn.layers.push_back(new Convolution(nn.layers.back()->y.rows() / filters[2], filters[2], filter_size[3], filters[3], batch_size));
	nn.layers.push_back(new ReLU(nn.layers.back()->y.rows(), nn.layers.back()->y.rows(), batch_size));
	nn.layers.push_back(new Pooling(nn.layers.back()->y.rows(), pooling_window, filters[3], batch_size));

	//CONV 3x3 -> POOL 2x
	nn.layers.push_back(new Convolution(nn.layers.back()->y.rows() / filters[3], filters[3], filter_size[4], filters[4], batch_size));
	nn.layers.push_back(new Pooling(nn.layers.back()->y.rows(), pooling_window, filters[4], batch_size));

	//FULLY CONNECTED
	nn.layers.push_back(new Linear(nn.layers.back()->y.rows(), 256, batch_size, dropout));
	nn.layers.push_back(new ReLU(256, 256, batch_size));

	//SOFTMAX
	nn.layers.push_back(new Linear(nn.layers.back()->y.rows(), 10, batch_size));
	nn.layers.push_back(new Softmax(10, 10, batch_size));

	for (size_t e = 0; e < epochs; e++) {

		std::cout << "Epoch " << e + 1 << std::endl << std::endl;
		nn.train(train_data, learning_rate, weight_decay, train_data.size() / batch_size, apply_dropout);

		nn.save_to_files("out/mnist_conv");

		double train_acc = nn.test(train_data);
		double test_acc = nn.test(test_data);

		// std::cout << std::endl;
		std::cout << std::setprecision(2) << "Train : " << 100.0 * train_acc << " %" <<  std::endl;
		std::cout << std::setprecision(2) << "Test  : " << 100.0 * test_acc << " %" << std::endl;

	}

}
