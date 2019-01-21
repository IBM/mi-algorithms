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
// Dependencies to utils.
#include <classifiers/LayerClassifier.hpp>
#include <cmath>

#include <logger/Log.hpp>

namespace mic {

namespace classifiers {

void LayerClassifier::update(bool* activityPattern, unsigned columns, unsigned category) {

	CHECK(activityPattern != NULL);

	if (activityPattern != NULL) {

		if (partialScores.size() < (columns + 1)) 
			partialScores.resize(columns + 1);

		for (unsigned i = 0; i < columns; i++) {

			if (activityPattern[i])
				partialScores[i].update(category);

		}

	}
}

LayerClassifier::LayerClassifier(unsigned size, unsigned categories) {

	partialScores.resize(size);

	for (unsigned i = 0; i < size; i++) {

		partialScores[i].score.resize(categories);

	}
}

std::vector<float> LayerClassifier::calculateSum(bool* activityPattern, unsigned columns, unsigned categories) {

	ColumnClassifier sum;

	sum.score.resize(categories);

	for (unsigned i = 0; i < columns; i++) {

		if (activityPattern[i] == true)
			for (unsigned c = 0; c < categories; c++) {

				sum.score[c] += (partialScores[i].score[c]);
				
			}
		
	}

	sum.normalize();

	return sum.score;

}

std::vector<float> LayerClassifier::calculateVotes(bool* activityPattern, float* overlapPattern, unsigned columns, unsigned categories) {

	ColumnClassifier sum;

	sum.score.resize(categories);
	
	for (unsigned i = 0; i < columns; i++) {

		if (activityPattern[i] == true)
				sum.score[partialScores[i].getHighest()] += overlapPattern[i];
		
	}

	return sum.score;
}


}//: namespace classifiers

}//: namespace mic


