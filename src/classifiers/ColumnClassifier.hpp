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
