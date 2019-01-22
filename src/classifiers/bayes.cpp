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
// Dependencies on utils.
#include <classifiers/bayes.hpp>

#include <iostream>
#include <cmath>

#include <logger/Log.hpp>


namespace mic {


namespace classifiers {


BayesClassifier::~BayesClassifier() {
}

#define USE_LOGS

int BayesClassifier::classify(bool* input_pattern) {

	CHECK(input_pattern != NULL);

	int result = -1;

	std::vector<double> categoryProb;

	categoryProb.resize(categories);

	if (input_pattern != NULL) {

		for (unsigned i = 0; i < categories; i++) {

// instead of calculating the product of probabilities, we maximize the sum of logarithms (due to vey small values with the original one)
#ifndef USE_LOGS
			double probGivenClass = 1.0;
#else
			double probGivenClass = 0.0;
#endif
			for (unsigned f = 0; f < inputs; f++) {

				if (input_pattern[f] == true) {

#ifndef USE_LOGS
					probGivenClass *= getFeatureProbabilityGivenCategory(f, i);
#else
					probGivenClass += log(getFeatureProbabilityGivenCategory(f, i));// / getFeatureProbability(f);
#endif				
				}
			}

#ifndef USE_LOGS
			categoryProb[i] = probGivenClass * getCategoryProbability(i);
#else
			categoryProb[i] = probGivenClass + log(getCategoryProbability(i));
#endif
		}


		double highestProbability = 0.0;

		for (unsigned i = 0; i < categories; i++) {

			if (categoryProb[i] > highestProbability || i == 0) {

				highestProbability = categoryProb[i];
				result = (int)i;

			}

			//std::cout << i << ": " << categoryProb[i] << std::endl;

		}

	}

	//if result < 0 -> error

	CHECK(result >= 0);

	return result;
}

void BayesClassifier::reset(void) {

	occurences.clear();
	init();
}

void BayesClassifier::init(void) {

	occurences.resize(inputs);

	for (unsigned i = 0; i < inputs; i++) {

		occurences[i].resize(categories);

		for (unsigned c = 0; c < categories; c++) {

			occurences[i][c] = 0;

		}

		// totalOccurences += categories;
	}
}

void BayesClassifier::update(bool* input_pattern, unsigned size, unsigned category) {

	CHECK(input_pattern != NULL);
	CHECK(size <= inputs);

	if (input_pattern != NULL && size <= inputs) {

		if (category < categories) {

			for (unsigned i = 0; i < size; i++) {

				if (input_pattern[i] == true) {

					occurences[i][category]++;

				}

			}

		}

	}

	totalOccurences++;

}

double BayesClassifier::getCategoryProbability(unsigned category) {

	unsigned long sum = 0L;

	for (unsigned f = 0; f < inputs; f++) {

		sum += occurences[f][category];

	}

	return (double)(sum + 1)/(double)(totalOccurences + 1);

}

unsigned long BayesClassifier::occurencesCategory(unsigned category) {

	unsigned long sum = 0L;

	for (unsigned f = 0; f < inputs; f++) {

		sum += occurences[f][category];

	}

	return sum;

}

double BayesClassifier::getFeatureProbability(unsigned feature) {

	unsigned long sum = 0L;

	for (unsigned c = 0; c < categories; c++) {

		sum += occurences[feature][c];

	}

	return (double)sum/(double)totalOccurences;

}

double BayesClassifier::getFeatureProbabilityGivenCategory(unsigned feature, unsigned category) {

	return (double)(occurences[feature][category] + 1)/(double)(occurencesCategory(category) + inputs);

}


}//: namespace classifiers

}//: namespace mic

