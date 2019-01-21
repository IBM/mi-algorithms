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
#ifndef __COLUMN_CLASSIFIER_HPP__
#define __COLUMN_CLASSIFIER_HPP__

#include <vector>

namespace mic {

namespace classifiers {

#define HISTORY_WEIGHT 0.99f

class ColumnClassifier {

	public:

		ColumnClassifier() { historyWeight = HISTORY_WEIGHT; newWeight = (1.0f - HISTORY_WEIGHT); }
		ColumnClassifier(unsigned size) { score.resize(size); historyWeight = HISTORY_WEIGHT; newWeight = (1.0f - HISTORY_WEIGHT); }
		~ColumnClassifier() {}

		void update(unsigned int category);
		void normalize(void);
		unsigned getHighest(void);

		std::vector<float> score;

	private:

		float historyWeight;
		float newWeight;

};


}//: namespace classifiers

}//: namespace mic


#endif
