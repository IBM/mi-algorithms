/*!
 * \file MultiLayerNeuralNetwork.cpp
 * \brief 
 * \author tkornut
 * \date Mar 31, 2016
 */

#include <mlnn/MultiLayerNeuralNetwork.hpp>

namespace mic {
namespace mlnn {

MultiLayerNeuralNetwork::MultiLayerNeuralNetwork(std::string name_ ) : name(name_) { }

MultiLayerNeuralNetwork::~MultiLayerNeuralNetwork() {

/*	for (size_t i = 0; i < layers.size(); i++) {

		delete(layers[i]);
	}*/

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

void MultiLayerNeuralNetwork::backward(mic::types::MatrixXf& targets_) {

	//set targets at the top
	//layers[layers.size() - 1]->dy = t;
	(*(layers.back()->g['y'])) = targets_;

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

	// update all layers according to gradients
	for (size_t i = 0; i < layers.size(); i++) {

		layers[i]->applyGrads(alpha, decay);

	}

}


void MultiLayerNeuralNetwork::train(mic::types::MatrixXfPtr encoded_batch_, mic::types::MatrixXfPtr encoded_targets_, float learning_rate_, float weight_decay_) {

	// Forward activations.
	forward(*encoded_batch_);

	// Get predictions.
	mic::types::MatrixXfPtr encoded_predictions = getPredictions();

	// Backprogagation
	backward(*encoded_targets_);

	// Apply changes
	update(learning_rate_, weight_decay_);

	// Calculate the statistics.
	size_t correct = countCorrectPredictions(*encoded_predictions, *encoded_targets_);
	float loss = encoded_predictions->calculateCrossEntropy(*encoded_targets_);
	std::cout << " Loss = " << std::setprecision(2) << std::setw(6) << loss << " | " << std::setprecision(1) << std::setw(4) << std::fixed << 100.0 * (float)correct / (float)encoded_batch_->cols() << "% batch correct" << std::endl;

}


float MultiLayerNeuralNetwork::test(mic::types::MatrixXfPtr encoded_batch_, mic::types::MatrixXfPtr encoded_targets_) {
	// skip dropout layers at test time
	bool skip_dropout = true;

	forward(*encoded_batch_, skip_dropout);

	return countCorrectPredictions(*(getPredictions()), *encoded_targets_);

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

mic::types::MatrixXfPtr MultiLayerNeuralNetwork::getPredictions() {
	return layers.back()->s['y'];
}



} /* namespace mlnn */
} /* namespace mic */
