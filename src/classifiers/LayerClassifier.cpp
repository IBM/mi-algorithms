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


