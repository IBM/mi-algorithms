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

