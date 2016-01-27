#ifndef __CLASSIFIER_HPP__
#define __CLASSIFIER_HPP__


namespace mic {

/**
 * \namespace mic::classifiers
 * \brief Namespace containing functions useful for image processing, memory allocation/deallocation etc.
 * \author tkornuta
 */
namespace classifiers {



/**
 * \brief Abstract class representing a classifier.
 * \author krocki
 */
class Classifier {

public:

	Classifier() {}
	Classifier(unsigned _inputs, unsigned _categories);
	virtual ~Classifier();

	virtual int classify(bool* input_pattern) = 0;
	virtual void update(bool* input_pattern, unsigned size, unsigned category) = 0;

protected:

	unsigned inputs;
	unsigned categories;

};


}//: namespace classifiers

}//: namespace mic


#endif

