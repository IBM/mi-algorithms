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
#ifndef __SOFTMAX_CLASSIFIER_HPP__
#define __SOFTMAX_CLASSIFIER_HPP__

#include <classifiers/classifier.hpp>
#include <deque>
#include <vector>

namespace mic {

namespace classifiers {

#define DEFAULT_LAST_ITERATIONS_SM 100

class SoftMaxClassifier : public Classifier {

public:

	SoftMaxClassifier() : Classifier() { lastIterations = DEFAULT_LAST_ITERATIONS_SM; }
	SoftMaxClassifier(unsigned i, unsigned c) : Classifier(i, c) { lastIterations = DEFAULT_LAST_ITERATIONS_SM; init(); }
	~SoftMaxClassifier();

	virtual int classify(bool* input_pattern);
	virtual void update(bool* input_pattern, unsigned size, unsigned category);
	void reset(void);
	
	float getCurrentCost(void) { return c_cost; }

protected:

	void init(void);
	void updateThetas(void);
	double computeCost(void);
	void computeGradients(void);
	void checkGradient(unsigned category);
	void optimize(unsigned iterations = 3);
	std::vector<double> computeOutputs(std::vector<bool> &input);
	unsigned long lastIterations;
	std::deque<std::vector<bool> > trainingInputs;
	std::deque<unsigned> trainingClasses;
	std::vector<std::vector<double> > trainingOutputs;
	std::vector<std::vector<double> > thetas;
	std::vector<std::vector<double> > gradients;
	double outputSum;
	double lambda;
	double increment;
	float c_cost;
};



}//: namespace classifiers

}//: namespace mic



#endif
