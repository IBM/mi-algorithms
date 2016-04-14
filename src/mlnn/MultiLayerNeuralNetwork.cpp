/*!
 * \file MultiLayerNeuralNetwork.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/MultiLayerNeuralNetwork.hpp>

namespace mic {
namespace mlnn {

MultiLayerNeuralNetwork::MultiLayerNeuralNetwork(size_t minibatch_size) : batch_size(minibatch_size) { }

MultiLayerNeuralNetwork::~MultiLayerNeuralNetwork() {

	for (size_t i = 0; i < layers.size(); i++) {

		delete(layers[i]);
	}

}

void MultiLayerNeuralNetwork::forward(mic::types::MatrixXf& input_data, bool skip_dropout) {

	//copy inputs to the lowest point in the network
	(*(layers[0]->s['x'])) = input_data;

	//compute forward activations
	for (size_t i = 0; i < layers.size(); i++) {
		//std::cout << "layer i =  " << i << " name = " << layers[i]->id() << std::endl;

		//y = f(x)
		layers[i]->forward(skip_dropout);

		//x(next layer) = y(current layer)
		if (i + 1 < layers.size())
			(*(layers[i+1]->s['x'])) = (*(layers[i]->s['y']));
			//layers[i + 1]->x = layers[i]->y;
	}

}

void MultiLayerNeuralNetwork::backward(mic::types::MatrixXf& t) {

	//set targets at the top
	//layers[layers.size() - 1]->dy = t;
	(*(layers[layers.size() - 1]->g['y'])) = t;

	//propagate error backward
	for (int i = layers.size() - 1; i >= 0; i--) {
		//std::cout << "layer i =  " << i << " name = " << layers[i]->id() << std::endl;

		layers[i]->resetGrads();
		layers[i]->backward();

		//dy(previous layer) = dx(current layer)
		if (i > 0) {

//			layers[i - 1]->dy = layers[i]->dx;
			(*(layers[i - 1]->g['y'])) = (*(layers[i]->g['x']));

		}

	}

}

void MultiLayerNeuralNetwork::update(float alpha, float decay) {

	//update all layers according to gradients
	for (size_t i = 0; i < layers.size(); i++) {
		//std::cout << "layer i =  " << i << " name = " << layers[i]->id() << std::endl;

		layers[i]->applyGrads(alpha, decay);

	}

}

void MultiLayerNeuralNetwork::train(std::deque<datapoint>& data, float alpha, float decay, size_t iterations, size_t classes) {

	//get random examples of size batch_size from data
	mic::types::VectorXi random_numbers(batch_size);
	mic::types::MatrixXf batch, targets;
	float loss, smooth_loss;
	float damping_factor = 0.99;

	/* TODO: change it to an external param */

	size_t correct;

	for (size_t ii = 0; ii < iterations; ii++) {

		correct = 0;

		randi(random_numbers, 0, data.size() - 1);

		// [784 x batch_size]
		batch = make_batch(data, random_numbers);
		targets = make_targets(data, random_numbers, classes);

		//forward activations
		forward(batch);

		//correct += count_correct_predictions(layers[layers.size() - 1]->y, targets);
		 correct += countCorrectPredictions(*(layers[layers.size() - 1]->s['y']), targets);

		//loss = cross_entropy(layers[layers.size() - 1]->y, targets);
		loss = calculateCrossEntropy(*(layers[layers.size() - 1]->s['y']), targets);

		smooth_loss = ii == 0 ? loss : damping_factor * smooth_loss + (1 - damping_factor) * loss;
		std::cout << std::setprecision(1) << std::setw(5) << "[" << std::setw(3) << ii + 1 << "/"
				  << std::setw(3) << iterations
				  << "] Loss = " << std::setw(5) << loss << ", Smooth loss = "
				  << std::setw(5) << smooth_loss << ", " << std::setw(5)
				  << std::fixed << 100.0 * (float)correct / (float)batch_size
				  << "% batch correct" << std::endl <<  std::flush;

		//backprogagation
		backward(targets);

		//apply changes
		update(alpha, decay);

	}

}


/* TDOO: mic::types::mic::types::MatrixXfXf compute_numerical_gradients(void) { } */

float MultiLayerNeuralNetwork::test(std::deque<datapoint>& data, size_t classes) {

	mic::types::VectorXi numbers(batch_size);
	mic::types::MatrixXf batch, targets;

	size_t correct = 0;

	for (size_t ii = 0; ii < data.size(); ii += batch_size) {

		linspace(numbers, ii, ii + batch_size);

		batch = make_batch(data, numbers);
		targets = make_targets(data, numbers, classes);

		//skip dropout layers at test time
		bool skip_dropout = true;

		forward(batch, skip_dropout);

		//correct += count_correct_predictions(layers[layers.size() - 1]->y, targets);
		correct += countCorrectPredictions(*(layers[layers.size() - 1]->s['y']), targets);

	}

	return (float)correct / (float)(data.size());

}

size_t MultiLayerNeuralNetwork::countCorrectPredictions(mic::types::MatrixXf& predictions_, mic::types::MatrixXf& targets_) {

	// Get vectors of indices denoting classes (type of 1-ouf-of-k dencoding).
	mic::types::VectorXf predicted_classes = predictions_.colwiseReturnMaxIndices();
	mic::types::VectorXf target_classes = targets_.colwiseReturnMaxIndices();

	// Get direct pointers to data.
	float *p = predicted_classes.data();
	float *t = target_classes.data();

	size_t correct=0;
	size_t i;

#pragma omp parallel for private(i) shared(correct)
	for(i=0; i<predicted_classes.size(); i++) {
		if (p[i] == t[i]) {
#pragma omp atomic
			correct++;
		}// if
	}//: for

	return correct;
}



float MultiLayerNeuralNetwork::calculateCrossEntropy(mic::types::MatrixXf& predictions_, mic::types::MatrixXf& targets_) {

	float ce = 0.0;
	mic::types::MatrixXf error(predictions_.rows(), predictions_.cols());

	//check what has happened and get information content for that event
	error.array() = -predictions_.unaryExpr(std::ptr_fun(::logf)).array() * targets_.array();

	// Sum the errors.
	ce = error.sum();

	return ce;
}



/* TODO: serialization */

void MultiLayerNeuralNetwork::save_to_files(std::string prefix) {

	for (size_t i = 0; i < layers.size(); i++) {

		std::ostringstream ss;
		ss << std::setw(3) << std::setfill('0') << i + 1;
		layers[i]->save_to_files(prefix + "_" + ss.str() + "_" + layers[i]->name);
	}

}

size_t MultiLayerNeuralNetwork::lastLayerInputsSize() {
	return layers.back()->inputsSize();
}

size_t MultiLayerNeuralNetwork::lastLayerOutputsSize() {
	return layers.back()->outputsSize();
}

size_t MultiLayerNeuralNetwork::lastLayerBatchSize() {
	return layers.back()->batchSize();
}



} /* namespace mlnn */
} /* namespace mic */
