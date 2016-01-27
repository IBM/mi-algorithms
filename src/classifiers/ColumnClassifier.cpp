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

