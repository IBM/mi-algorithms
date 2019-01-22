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
#ifndef __LAYER_CLASSIFIER_HPP__
#define __LAYER_CLASSIFIER_HPP__

#include <classifiers/ColumnClassifier.hpp>
#include <vector>

namespace mic {

namespace classifiers {

typedef enum { AVERAGE, VOTING } selectionMethod;

class LayerClassifier {

	public:

		LayerClassifier() {}
		LayerClassifier(unsigned size, unsigned categories);
		~LayerClassifier() {}

		void update(bool* activityPattern, unsigned columns, unsigned category);
		std::vector<float> calculateSum(bool* activityPattern, unsigned columns, unsigned categories);
		std::vector<float> calculateVotes(bool* activityPattern, float* overlapPattern, unsigned columns, unsigned categories);
		std::vector<ColumnClassifier> partialScores;

};


}//: namespace classifiers

}//: namespace mic


#endif
