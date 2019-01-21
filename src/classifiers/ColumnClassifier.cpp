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
#include <classifiers/ColumnClassifier.hpp>

namespace mic {

namespace classifiers {

void ColumnClassifier::update(unsigned int category) {

	if (score.size() < (category + 1))
		score.resize(category + 1);

	for (unsigned i = 0; i < score.size(); i++) {
		
		score[i] *= historyWeight;
	
	}

	score[category] += newWeight;

	// normalize();
}

void ColumnClassifier::normalize(void) {

	float sum = 0.0f;

	for (unsigned i = 0; i < score.size(); i++) {

		sum += score[i];

	}

	for (unsigned i = 0; i < score.size(); i++) {

		score[i] /= sum;

	}

}

unsigned ColumnClassifier::getHighest(void) {

	float best = 0.0f;
	unsigned idx = 0;

	for (unsigned i = 0; i < score.size(); i++) {

		if (best < score[i] || i == 0) {

			best = score[i];
			idx = i;
			
		}

	}

	return idx;
}



}//: namespace classifiers

}//: namespace mic

