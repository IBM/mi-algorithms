/*!
 * \file MultiLayerNeuralNetwork.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/MultiLayerNeuralNetwork.hpp>

namespace mic {
namespace mlnn {

MultiLayerNeuralNetwork::MultiLayerNeuralNetwork(size_t minibatch_size) : batch_size(minibatch_size) {

}

MultiLayerNeuralNetwork::~MultiLayerNeuralNetwork() {
	for (size_t i = 0; i < layers.size(); i++) {

		delete(layers[i]);
	}
}


void MultiLayerNeuralNetwork::forward(mic::types::MatrixXf& input_data, bool apply_dropout) {

	//copy inputs to the lowest point in the network
	layers[0]->x = input_data;

	//compute forward activations
	for (size_t i = 0; i < layers.size(); i++) {

		//y = f(x)
		layers[i]->forward(apply_dropout);

		//x(next layer) = y(current layer)
		if (i + 1 < layers.size())
			layers[i + 1]->x = layers[i]->y;
	}

}

void MultiLayerNeuralNetwork::backward(mic::types::MatrixXf& t) {

	//set targets at the top
	layers[layers.size() - 1]->dy = t;

	//propagate error backward
	for (int i = layers.size() - 1; i >= 0; i--) {

		layers[i]->resetGrads();
		layers[i]->backward();

		//dy(previous layer) = dx(current layer)
		if (i > 0) {

			layers[i - 1]->dy = layers[i]->dx;
		}

	}

}

void MultiLayerNeuralNetwork::update(float alpha, float decay) {

	//update all layers according to gradients
	for (size_t i = 0; i < layers.size(); i++) {

		layers[i]->applyGrads(alpha, decay);

	}

}

void MultiLayerNeuralNetwork::train(std::deque<datapoint>& data, float alpha, float decay, size_t iterations, bool apply_dropout) {

	//get random examples of size batch_size from data
	mic::types::VectorXi random_numbers(batch_size);
	mic::types::MatrixXf batch, targets;
	float loss, smooth_loss;
	float damping_factor = 0.99;

	/*
				TODO: change it to an external param
	*/
	size_t classes = 10;
	size_t correct;

	for (size_t ii = 0; ii < iterations; ii++) {

		correct = 0;

		randi(random_numbers, 0, data.size() - 1);

		// [784 x batch_size]
		batch = make_batch(data, random_numbers);
		targets = make_targets(data, random_numbers, classes);

		//forward activations
		forward(batch, apply_dropout);

		correct += count_correct_predictions(layers[layers.size() - 1]->y, targets);

		loss = cross_entropy(layers[layers.size() - 1]->y, targets);
		smooth_loss = ii == 0 ? loss : damping_factor * smooth_loss + (1 - damping_factor) * loss;
		std::cout << std::setprecision(1) << std::setw(5) << "[" << std::setw(3) << ii + 1 << "/"
				  << std::setw(3) << iterations
				  << "] Loss = " << std::setw(5) << loss << ", Smooth loss = "
				  << std::setw(5) << smooth_loss << ", " << std::setw(5)
				  << std::fixed << 100.0 * (float)correct / (float)batch_size
				  << "% batch correct" << "      \r" << std::flush;

		//backprogagation
		backward(targets);

		//apply changes
		update(alpha, decay);
	}

}

float MultiLayerNeuralNetwork::test(std::deque<datapoint>& data) {

	mic::types::VectorXi numbers(batch_size);
	mic::types::MatrixXf batch, targets;

	size_t classes = 10;
	size_t correct = 0;

	for (size_t ii = 0; ii < data.size(); ii += batch_size) {

		linspace(numbers, ii, ii + batch_size);

		batch = make_batch(data, numbers);
		targets = make_targets(data, numbers, classes);

		forward(batch);

		correct += count_correct_predictions(layers[layers.size() - 1]->y, targets);

	}

	return (float)correct / (float)(data.size());

}

/*TODO: serialization*/
void MultiLayerNeuralNetwork::save_to_files(std::string prefix) {

	for (size_t i = 0; i < layers.size(); i++) {
		layers[i]->save_to_files(prefix + "_" + layers[i]->name + std::to_string(i + 1));
	}

}


} /* namespace mlnn */
} /* namespace mic */
