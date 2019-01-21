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
#include <classifiers/softmax.hpp>

// Dependency on data utils.
#include <iostream>
#include <cmath>
#include <numeric>
#include <data_utils/rand.hpp>

#include <logger/Log.hpp>


using namespace mic::data_utils;

namespace mic {
namespace classifiers {

SoftMaxClassifier::~SoftMaxClassifier() {
	LOG(LTRACE) << "~SoftMaxClassifier";
}

int SoftMaxClassifier::classify(bool* input_pattern) {

	CHECK(input_pattern != NULL);

	int result = -1;

	std::vector<bool> input;
	input.resize(inputs);

	double highest = 0.0;

	for (unsigned i = 0; i < inputs; i++)
		input[i] = input_pattern[i];

	optimize(); 

	std::vector<double> currentPredictions = computeOutputs(input);

	for (unsigned j = 0; j < categories; j++) {

		if (currentPredictions[j] > highest) {

			highest = currentPredictions[j];
			result = (int)j;

		}

		//printf("softmax, %d = %f\n", j, currentPredictions[j]);

	}

	//if result < 0 -> error

	CHECK(result >= 0);

	return result;
}

void SoftMaxClassifier::reset(void) {

	init();
}

void SoftMaxClassifier::init(void) {

	trainingInputs.clear();
	trainingClasses.clear();
	trainingOutputs.clear();
	thetas.clear();
	thetas.resize(categories);
	gradients.resize(categories);

	// // zero all elements

	for (unsigned c = 0; c < categories; c++) {

		thetas[c].resize(inputs + 1);
		gradients[c].assign(inputs + 1, 0.0);

		for (unsigned i = 0; i < inputs + 1; i++) {

			thetas[c][i] = 0.005 * rand_real01();

		}

	}

	lambda = 1e-4;
	increment = 1.0;

}

void SoftMaxClassifier::update(bool* input_pattern, unsigned size, unsigned category) {

	CHECK(input_pattern != NULL);
	CHECK(size <= inputs);
	CHECK(category < size);

	if (input_pattern != NULL && size <= inputs) {

		std::vector<bool> temp;
		temp.resize(size);

		for (unsigned i = 0; i < size; i++) {

			temp[i] = input_pattern[i];
		}

		trainingClasses.push_back(category);
		trainingInputs.push_back(temp);

		if (trainingInputs.size() > lastIterations) {

			trainingInputs.pop_front();
			trainingClasses.pop_front();

		}


	}

	trainingOutputs.resize(trainingInputs.size());

	//printf("softmax: new sample: I: %lu C: %lu O: %lu\n", trainingInputs.size(), trainingClasses.size(), trainingOutputs.size());

}

double SoftMaxClassifier::computeCost(void) {

	//printf("compute cost, m = %lu\n", trainingInputs.size());

	double cost = 0.0;
	unsigned m = (unsigned)trainingInputs.size();

	std::vector<double> cost_examples;
	
	cost_examples.assign(m, 0.0);
	
	//TODO: add weight decay to cost: + lambda/2 * sum_ij of theta(i,j)^2

	for (unsigned i = 0; i < m; i++) {

		trainingOutputs[i] = computeOutputs(trainingInputs[i]);

		for (unsigned j = 0; j < categories; j++) {

			double indicator = (j == trainingClasses[i]) ? 1.0 : 0.0;
			
			cost_examples[i] += indicator * log(trainingOutputs[i][j]);

	//		printf("%d, %f\n", i, cost_examples[i]);

		}

	}

	cost = std::accumulate(cost_examples.begin(), cost_examples.end(), 0.0);

	cost /= -(double)m;

	c_cost = (float) cost;

	LOG(LDEBUG) << "SoftMax cost:" << cost;

	return cost;
}

void SoftMaxClassifier::computeGradients(void) {

	//TODO: add weight decay to gradient: + lambda * theta(j)

	for (unsigned c = 0; c < categories; c++) {

		gradients[c].assign(inputs + 1, 0);
	}
	
	unsigned m = (unsigned)trainingInputs.size();

	for (unsigned j = 0; j < categories; j++) {

		for (unsigned i = 0; i < m; i++) {

			double indicator = (j == trainingClasses[i]) ? 1.0 : 0.0;
			
			for (unsigned t = 0; t < inputs; t++) {

				gradients[j][t] += -(trainingInputs[i][t] * (indicator - trainingOutputs[i][j]))/(double)m;

			}

			// bias
			gradients[j][inputs] += -(1.0 * (indicator - trainingOutputs[i][j]))/(double)m;
			
		}

	}

}

void SoftMaxClassifier::updateThetas(void) {

	for (unsigned j = 0; j < categories; j++) {
			
		for (unsigned t = 0; t < inputs + 1; t++) {
		
			thetas[j][t] -= gradients[j][t] * increment;
		}

	}

}

void SoftMaxClassifier::optimize(unsigned iterations) {

	for (unsigned i = 0; i < iterations; i++) {

		double cost0 = computeCost();
		
		computeGradients();
		//checkGradient(0);
		//printf("Updating Thetas\n");
		updateThetas();

		double cost1 = computeCost();

		if (cost1 >= cost0)
			break;
		//computeCost();

	}
}

void SoftMaxClassifier::checkGradient(unsigned j) {

	for (unsigned t = 0; t < inputs; t++) {

		double epsilon = 1e-4;

		double temp = thetas[j][t];

		thetas[j][t] = temp + epsilon;

		// double costPlus = computeCost(); // TK unused variable

		thetas[j][t] = temp - epsilon;

		// double costMinus = computeCost(); // TK unused variable

		thetas[j][t] = temp;

//		printf("j %u, i %u = %f ~ %f, theta[j][k] %f\n", j, t, gradients[j][t], (costPlus - costMinus)/(2.0 * epsilon), thetas[j][t]);
	}

}

std::vector<double> SoftMaxClassifier::computeOutputs(std::vector<bool> &input) {

	CHECK(input.size() > 0);

	outputSum = 0.0;

	std::vector<double> theta_x;
	std::vector<double> outputs;

	theta_x.assign(categories, 0.0);
	outputs.assign(categories, 0.0);

	double constant = 0.0;

	// theta_x
	for (unsigned j = 0; j < categories; j++) {

		for (unsigned t = 0; t < inputs; t++) {

			theta_x[j] += thetas[j][t] * input[t];

		}

		// bias
		theta_x[j] += thetas[j][inputs];

		// prevent overflows
		if (theta_x[j] > constant)
			constant = theta_x[j];
	}

	// hypotheses
	for (unsigned j = 0; j < categories; j++) {

		outputs[j] = exp(theta_x[j] - constant);

		outputSum += outputs[j];

	}

	// normalize, probabilities
	for (unsigned j = 0; j < categories; j++) {

		outputs[j] /= outputSum;

		//printf("outputs[j] %u, %f\n", j, outputs[j]);

	}

	return outputs;
}



}//: namespace classifiers

}//: namespace mic

