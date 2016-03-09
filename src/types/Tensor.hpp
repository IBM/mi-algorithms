/*!
 * \file Tensor.hpp
 * \brief File contaning a template class being a View on tensor.
 * \author tkornuta
 * \date Mar 7, 2016
 */

#ifndef SRC_TYPES_TENSOR_HPP_
#define SRC_TYPES_TENSOR_HPP_

#include <stdio.h>
#include <vector>

#include <data_utils/functions.h>

namespace mic {
namespace types {

/*!
 * \brief Template class representing an nD (n-Dimensional) tensor.
 * \author tkornuta/kmrocki
 * \tparam T template parameter denoting data type stored in tensor.
 */
template<class T = float>
class Tensor {
public:

	/*!
	 * Default constructor (empty).
	 */
	Tensor() = default;

	/*!
	 * Constructor with a variable number of parameters in { }, denoting the tensor dimensions.
	 * @param args Tensor dimensions ( in { }, e.g. { width height depth ... })
	 */
	Tensor(std::initializer_list<size_t> args) {
		// Set dimensions.
		elements = 1;
		for (auto ith_dimension : args) {
			// Every dimension must be greater than 0!.
			assert(ith_dimension > 0);
			// Add dimension.
			dimensions.push_back(ith_dimension);
			elements *= ith_dimension;
		}

		// Allocate memory.
		data_ptr = new T[elements];
		// Check whether data was allocated.
		assert(data_ptr != nullptr);

		// Initialize: set all elements to zeros.
		zeros();
	}

	/*!
	 * Copying constructor - copies the values of the given tensor, including tensor dimensions and data.
	 * @param t The original tensor to be copied.
	 */
	Tensor(const Tensor& t) {
		// Copy dimensions.
		elements = t.elements;
		dimensions.reserve(t.dimensions.size());
		std::copy(t.dimensions.begin(), t.dimensions.end(),
				std::back_inserter(dimensions));

		// Copy data.
		if (data_ptr != nullptr)
			delete (data_ptr);
		data_ptr = new T[t.elements];
		memcpy(data_ptr, t.data_ptr, sizeof(T) * elements);
	}

	/*!
	 * Assign operator - copies the values of the given tensor, including tensor dimensions and data.
	 * @param t The original tensor to be copied.
	 */
	Tensor<T>& operator=(const Tensor<T>& t) {
		// Copy dimensions.
		elements = t.elements;
		dimensions.reserve(t.dimensions.size());
		std::copy(t.dimensions.begin(), t.dimensions.end(),
				std::back_inserter(dimensions));

		// Copy data.
		memcpy(data_ptr, t.data_ptr, sizeof(T) * elements);
	}

	/*!
	 * Sets all element values to zero.
	 */
	void zeros() {
		memset(data_ptr, 0, elements * sizeof(T));
	}

	/*!
	 * Sets all element values to one.
	 */
	void ones() {
		memset(data_ptr, 1, elements * sizeof(T));
	}

	/*!
	 * Enumerates - sets values of elements to their indices.
	 */
	void enumerate() {
		for (int i = 0; i < elements; i++)
			data_ptr[i] = i;
	}

	/*!
	 * Destructor. Frees memory (if it was assigned).
	 */
	~Tensor() {
		// Free memory.
		if (data_ptr)
			delete (data_ptr);
	}

	/*!
	 * Stream operator enabling to print tensor dimensions and all values.
	 * @param os Ostream object.
	 * @param tv Tensor object.
	 * @return Ostream containing tensor description.
	 */
	friend std::ostream& operator<<(std::ostream& os, const Tensor& t) {
		// Display dimensions.
		os << "[";
		for (size_t i = 0; i < t.dimensions.size() - 1; i++)
			os << t.dimensions[i] << " x ";
		os << t.dimensions.back() << "]: [";

		// Display elements.
		for (size_t i = 0; i < t.elements - 1; i++)
			os << t.data_ptr[i] << ", ";
		os << t.data_ptr[t.elements - 1] << "]\n";

		return os;
	}

	/*!
	 * Flattens the tensor - sets new dimensions [ n x 1 x 1 x ...].
	 */
	void flatten() {
		for (size_t i = 0; i < dimensions.size(); i++) {
			dimensions[i] = 1;
		} //: for
		dimensions[0] = elements;
	}

	/*!
	 * Resizes the tensor - sets new dimensions, however total number of elements must remain unchanged.
	 * @param dims_ New dimensions.
	 */
	void resize(std::vector<size_t> dims_) {
		// Check whether new dimensions are ok.
		size_t new_size = 1;
		for (auto ith_dimension : dims_) {
			new_size *= ith_dimension;
		}
		assert(new_size == elements);

		// Set new dimensions.
		dimensions.clear();
		dimensions.reserve(dims_.size());
		std::copy(dims_.begin(), dims_.end(), std::back_inserter(dimensions));
	}

	/*!
	 * Applies the function to all tensor elements.
	 * @param func The function to be applied. This must be a function with a single argument.
	 */
	void elementwiseFunction(T (*func)(T)) {
#pragma omp parallel for
		for (size_t i = 0; i < elements; i++) {
			data_ptr[i] = (*func)(data_ptr[i]);
		}//: for
	}

	/*!
	 * Applies the function to all tensor elements. Additionally passes scalar to the function as second argument.
	 * @param func The function to be applied. This must be a function with exactly two arguments.
	 * @param scalar Scalar passed as second function argument.
	 */
	void elementwiseFunctionScalar(T (*func)(T, T), T scalar) {
#pragma omp parallel for
		for (size_t i = 0; i < elements; i++) {
			data_ptr[i] = (*func)(data_ptr[i], scalar);
		}//: for
	}

	/*!
	 * Sums all tensor elements.
	 * @return Sum of elements.
	 */
	T sum(void) {
		T total = (T) 0;
		for (size_t i = 0; i < elements; i++) {
			total += data_ptr[i];
		}//: for
		return total;
	}

	/*!
	 * Set values of all matrix elements to random with a normal distribution.
	 * @param mean Mean
	 * @param stddev Variance
	 */
	void normRandReal(float mean = 0, float stddev = 1) {
		// Initialize random number generator with normal distribution.
		std::random_device rd;
		std::mt19937 mt(rd());
		std::normal_distribution<> dist(mean, stddev);
		// Set value of all elements to random.
#pragma omp parallel for
		for (size_t i = 0; i < elements; i++) {
			data_ptr[i] = dist(mt);
		}
	}

	/*!
	 * Returns pointer to data.
	 */
	T* data() {
		return data_ptr;
	}

	/*!
	 * Returns k-th element.
	 */
	T data(size_t k) {
		return data_ptr[k];
	}

	/*!
	 * Returns dimensions.
	 */
	std::vector<size_t> dims() {
		return dimensions;
	}

	/*!
	 * Returns k-th dimension.
	 */
	size_t dims(size_t k) {
		return dimensions[k];
	}

	/*!
	 * Returns size - number of elements.
	 * @return Number of elements.
	 */
	size_t size() {
		return elements;
	}

	/*!
	 * Returns the index of an element in nD matrix.
	 * @param coordinates_ nD vector of element coordinates.
	 * @return Index of the element.
	 */
	size_t getIndex(std::vector<size_t> coordinates_) {
		// Do the magic - iterate through all dimensions in order to compute the index.
		// 1 - x
		// 2 - y*width + x
		// 3 - z*height*width + y*width + x = (z*height +y)*width + x
		// 4 - v*depth*height*width + z*height*width + y*width + x = (v*d +z)(h +y)*w +x
		// ...

		// But first: solve the simple 1d-2d-3d cases.
		switch (coordinates_.size()){
			case 1: return data_ptr[coordinates_[0]];
			case 2: return data_ptr[coordinates_[1]*dimensions[0] + coordinates_[0]];
			case 3: return data_ptr[(coordinates_[2]*dimensions[1] + coordinates_[1])*dimensions[0] + coordinates_[0]];
			default: return recursiveIndex(0, coordinates_);
		}//: switch
	}

	/*!
	 * Returns a single element of an nD matrix.
	 * @param coordinates_ nD vector of element coordinates.
	 * @return The value of the element.
	 */
	T getValue(std::vector<size_t> coordinates_) {
		return data_ptr[getIndex(coordinates_)];
	}

	/*!
	 * Returns a single element of an nD matrix.
	 * @param index_ Index (a single value).
	 * @return The value of the element.
	 */
	T getValue(size_t index_) {
		return data_ptr[index_];
	}

private:
	/*!
	 * Dimensions.
	 */
	std::vector<size_t> dimensions;

	/*!
	 * Number of elements.
	 */
	size_t elements;

	/*!
	 * Table of elements of data types.
	 */
	T* data_ptr;

	/*!
	 * Recursive method computing the index of element in nD matrix.
	 * @param dim_ Dimension considered at the moment.
	 * @param coordinates_ nD vector of coordinates.
	 * @return Index of the element.
	 */
	size_t recursiveIndex(size_t dim_, std::vector<size_t> coordinates_) {
		// Do the magic - iterate through all dimensions in order to compute the index.
		// 1 - x
		// 2 - y*width + x
		// 3 - z*height*width + y*width + x = (z*height +y)*width + x
		// 4 - v*depth*height*width + z*height*width + y*width + x = (v*d +z)(h +y)*w +x
		// ...

		if (dim_ == coordinates_.size() - 1)
			return coordinates_[dim_];
		else
			return recursiveIndex(dim_ + 1, coordinates_) * dimensions[dim_] + coordinates_[dim_];
	}
};

} //: namespace types
} //: namespace mic

#endif /* SRC_TYPES_TENSOR_HPP_ */
