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
