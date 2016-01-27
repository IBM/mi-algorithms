#ifndef __BAYES_CLASSIFIER_HPP__
#define __BAYES_CLASSIFIER_HPP__

#include <classifiers/classifier.hpp>
#include <vector>

namespace mic {

namespace classifiers {


#define DEFAULT_LAST_ITERATIONS 1000

class BayesClassifier : public Classifier {

public:

	BayesClassifier() : Classifier() { lastIterations = DEFAULT_LAST_ITERATIONS; }
	BayesClassifier(unsigned i, unsigned c) : Classifier(i, c) { lastIterations = DEFAULT_LAST_ITERATIONS; init(); }
	~BayesClassifier();

	virtual int classify(bool* input_pattern);
	virtual void update(bool* input_pattern, unsigned size, unsigned category);
	void reset(void);

protected:

	void init(void);
	double getCategoryProbability(unsigned category);
	unsigned long occurencesCategory(unsigned category);
	double getFeatureProbability(unsigned feature);
	double getFeatureProbabilityGivenCategory(unsigned feature, unsigned category);

	unsigned long lastIterations;
	std::vector<std::vector<unsigned> > occurences;
	unsigned long totalOccurences;

};


}//: namespace classifiers

}//: namespace mic


#endif

