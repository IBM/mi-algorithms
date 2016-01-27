#include <classifiers/classifier.hpp>

namespace mic {
namespace classifiers {

Classifier::Classifier(unsigned _inputs, unsigned _categories) {
	inputs = _inputs;
	categories = _categories;
}

Classifier::~Classifier() {
}



}//: namespace classifiers
}//: namespace mic

