/*
* @File: tensor.h
* @Author: kamilrocki
* @Email: kmrocki@us.ibm.com
* @Created:	2015-04-26 10:16:07
* @Last Modified by:   kamilrocki
* @Last Modified time: 2015-05-14 19:42:38
*/

#ifndef __TENSOR_H__
#define __TENSOR_H__

#include <stdio.h>
#include <vector>

#include <data_utils/functions.h>

namespace mic {
namespace types {


/**
 * \brief Class representing an n-D tensor.
 * \author krocki
 * \tparam T template parameter denoting data type stored in tensor.
 */
template <class T = float>
class Tensor {
public:

	Tensor() = default;

	Tensor(const Tensor& t) {
		// Copy dimensions.
		elements = t.elements;
		dim.reserve(t.dim.size());
		std::copy(t.dim.begin(),t.dim.end(),std::back_inserter(dim));


		// Copy data.
		if (data != nullptr) delete(data);
		data = new T[t.elements];
		memcpy(data, t.data, sizeof(T) * elements);

		// ????
		//this = t;
	}

	Tensor<T>& operator=(const Tensor<T>& t) {
		// Copy dimensions.
		elements = t.elements;
		dim.reserve(t.dim.size());
		std::copy(t.dim.begin(),t.dim.end(),std::back_inserter(dim));

		// Copy data.
		memcpy(data, t.data, sizeof(T) * elements);
	}

	Tensor(std::initializer_list<size_t> args) {

		// Set dimensions.
		elements = 1;
		for (auto i: args) {

			dim.push_back(i);
			elements *= i;
		}

		// Allocate memory.
		data = new T[elements];

		if (!data) {
			//couldn't allocate
			std::cout << "could not allocate mem: " << __PRETTY_FUNCTION__ << std::endl;
			// TODO: EXIT??
		}//: if
		else
			reset();
	}

	~Tensor() {
		// Free memory.
		if (data)
			delete(data);
	}

	virtual void serialize(std::ostream& os) const {

		os << "[";

		for (size_t i = 0; i < dim.size() - 1; i++)
			os << dim[i] << " x ";

		os << dim.back() << " Tensor] ";
		os << elements << " elements";

		for (size_t i = 0; i < elements; i++) {
			os << data[i] << ", ";
		}
		os << std::endl;

	}

	friend std::ostream& operator<<(std::ostream& os, const Tensor& t) {

		t.serialize(os);

		return os;

	}

	void reset() {

		memset(data, 0, elements * sizeof(T));

	}

	void elementwise_function(T(*func)(T)) {

		#pragma omp parallel for
		for (size_t i = 0; i < elements; i++) {

			data[i] = (*func)(data[i]);

		}

	}

	void elementwise_function_scalar(T(*func)(T, T), T scalar) {

		#pragma omp parallel for
		for (size_t i = 0; i < elements; i++) {

			data[i] = (*func)(data[i], scalar);

		}

	}

	void flatten(void) {

		for (size_t i = 0; i < dim.size(); i++) {

			dim[i] = 1;

		}

		dim[0] = elements;

	}

	T sum(void) {

		T total = (T)0;

		for (size_t i = 0; i < elements; i++) {

			total += data[i];

		}

		return total;
	}

	/*!
	 * Set values of all matrix elements to random with a normal distribution.
	 * @param mean Mean
	 * @param stddev Variance
	 */
	void normRandReal(float mean = 0, float stddev = 1){

		// Initialize random number generator with normal distribution.
		std::random_device rd;
		std::mt19937 mt(rd());
		std::normal_distribution<> dist(mean, stddev);

		#pragma omp parallel for
		for (size_t i = 0; i < elements; i++) {
			data[i] = dist(mt);
		}
	}

//private:
		/*!
		 * Dimensions.
		 */
		std::vector<size_t> dim;

		/*!
		 * Table of elements of data types.
		 */
		T* data;

		/*!
		 * Number of elements.
		 */
		size_t elements;


};

}//: namespace types
}//: namespace mic

#endif /*__TENSOR_H__*/
