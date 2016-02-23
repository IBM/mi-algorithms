#ifndef __SOFTMAX_CLASSIFIER_HPP__
#define __SOFTMAX_CLASSIFIER_HPP__

#include <classifiers/classifier.hpp>
#include <deque>
#include <vector>

namespace mic {

namespace classifiers {

#define DEFAULT_LAST_ITERATIONS_SM 100

class SoftMaxClassifier : public Classifier {

public:

	SoftMaxClassifier() : Classifier() { lastIterations = DEFAULT_LAST_ITERATIONS_SM; }
	SoftMaxClassifier(unsigned i, unsigned c) : Classifier(i, c) { lastIterations = DEFAULT_LAST_ITERATIONS_SM; init(); }
	~SoftMaxClassifier();

	virtual int classify(bool* input_pattern);
	virtual void update(bool* input_pattern, unsigned size, unsigned category);
	void reset(void);
	
	float getCurrentCost(void) { return c_cost; }

protected:

	void init(void);
	void updateThetas(void);
	double computeCost(void);
	void computeGradients(void);
	void checkGradient(unsigned category);
	void optimize(unsigned iterations = 3);
	std::vector<double> computeOutputs(std::vector<bool> &input);
	unsigned long lastIterations;
	std::deque<std::vector<bool> > trainingInputs;
	std::deque<unsigned> trainingClasses;
	std::vector<std::vector<double> > trainingOutputs;
	std::vector<std::vector<double> > thetas;
	std::vector<std::vector<double> > gradients;
	double outputSum;
	double lambda;
	double increment;
	float c_cost;
};



}//: namespace classifiers

}//: namespace mic



#endif