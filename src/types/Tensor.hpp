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
 * \author tkornuta
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
	 * Constructor - sets the tensor dimension and assigns memory.
	 * @param dims_ Tensor dimensions ({ }, vector<size_t> etc.).
	 */
	Tensor(std::vector<size_t> dims_) {
		// Set dimensions.
		elements = 1;
		for (auto ith_dimension : dims_) {
			// Every dimension must be greater than 0!
			assert(ith_dimension > 0);
			// Add dimension.
			dimensions.push_back(ith_dimension);
			elements *= ith_dimension;
		}//: for

		// Allocate memory.
		if (data_ptr != nullptr)
			delete (data_ptr);
		data_ptr = new T[elements];
		// Check whether data was allocated. :]
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

		// Allocate memory.
		if (data_ptr != nullptr)
			delete (data_ptr);
		data_ptr = new T[t.elements];
		// Copy data.
		memcpy(data_ptr, t.data_ptr, sizeof(T) * elements);
	}

	/*!
	 * Assign operator - copies the values of the given tensor, including tensor dimensions and data.
	 * @param t The original tensor to be copied.
	 */
	Tensor<T>& operator=(const Tensor<T>& t) {
		// Check the dimensions.
		if (elements != t.elements) {
			elements = t.elements;
			// Allocate memory.
			if (data_ptr != nullptr)
				delete (data_ptr);
			data_ptr = new T[t.elements];
		}//: if

		// Copy dimensions.
		dimensions.clear();
		dimensions.reserve(t.dimensions.size());
		std::copy(t.dimensions.begin(), t.dimensions.end(),
				std::back_inserter(dimensions));

		// Copy data.
		memcpy(data_ptr, t.data_ptr, sizeof(T) * elements);
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
	 * Flattens the tensor - sets dimensions to [ n ].
	 */
	void flatten() {
		dimensions.clear();
		dimensions.push_back(elements);
	}

	/*!
	 * Resizes the tensor - sets new tensor dimensions.
	 * Important note: the total number of elements must remain unchanged.
	 * @param dims_ New dimensions.
	 */
	void resize(std::vector<size_t> dims_) {
		// Check whether new dimensions are ok.
		size_t new_size = 1;
		for (auto ith_dimension : dims_) {
			// Every dimension must be greater than 0!
			assert(ith_dimension > 0);
			new_size *= ith_dimension;
		}
		assert(new_size == elements);

		// Set new dimensions.
		dimensions.clear();
		dimensions.reserve(dims_.size());
		std::copy(dims_.begin(), dims_.end(), std::back_inserter(dimensions));
	}

	/*!
	 * Resizes the tensor - sets new tensor dimensions. FORCED version!
	 * Important note: if the total number of elements change, a new memory block will be allocated and the method will copy as much elements from the old memory block as possible.
	 * If the new block will be bigger than the old one the values of "new elements" will be set to zeros.
	 * @param dims_ New dimensions.
	 */
	void resizeForced(std::vector<size_t> dims_) {
		// Check whether new dimensions are ok.
		size_t new_size = 1;
		for (auto ith_dimension : dims_) {
			// Every dimension must be greater than 0!
			assert(ith_dimension > 0);
			new_size *= ith_dimension;
		}

		// Set new dimensions.
		dimensions.clear();
		dimensions.reserve(dims_.size());
		std::copy(dims_.begin(), dims_.end(), std::back_inserter(dimensions));

		// Copy data.
		if (new_size != elements) {
			T* old_prt = data_ptr;
			// Allocate memory.
			data_ptr = new T[new_size];
			// Estimate the size of block that must be copied.
			size_t block_size = (new_size < elements) ? new_size : elements;
			// Change number of elements.
			elements = new_size;
			// Zero elements.
			zeros();
			// Copy data.
			memcpy(data_ptr, old_prt, sizeof(T) * block_size);
			// Free the old block.
			delete (old_prt);
		} //: if
		//: else: do nothing;)
	}

	/*!
	 * Applies the function to all tensor elements.
	 * @param func The function to be applied. This must be a function with a single argument.
	 */
	void elementwiseFunction(T (*func)(T)) {
#pragma omp parallel for
		for (size_t i = 0; i < elements; i++) {
			data_ptr[i] = (*func)(data_ptr[i]);
		} //: for
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
		} //: for
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
	 * Stream operator enabling to print tensor dimensions and all values.
	 * @param os_ Ostream object.
	 * @param obj_ Tensor object.
	 */
	friend std::ostream& operator<<(std::ostream& os_, const Tensor& obj_) {
		// Display dimensions.
		os_ << "[";
		for (size_t i = 0; i < obj_.dimensions.size() - 1; i++)
			os_ << obj_.dimensions[i] << " x ";
		os_ << obj_.dimensions.back() << "]: [";

		// Display elements.
		for (size_t i = 0; i < obj_.elements - 1; i++)
			os_ << obj_.data_ptr[i] << ", ";
		os_ << obj_.data_ptr[obj_.elements - 1] << "]\n";

		return os_;
	}

	/*
	 * Stream operator enabling writing values to tensor.
	 * @param is_ Istream object.
	 * @param obj_ Tensor object.
	 */
/*	friend std::istream& operator>>(std::istream& is_, Tensor& obj_)
	{
		// TODO

		return is_;
	}*/


	/*!
	 * Operator used for setting the value of a given element of nD tensor.
	 * @param index_ Index (a single value).
	 * @return The value of the element.
	 */
	inline T& operator()(size_t index_) {
		return data_ptr[index_];
	}

	/*!
	 * Operator used for setting the value of a given element of nD tensor.
	 * @param coordinates_ nD vector of element coordinates ({ }, vector<size_t> etc.).
	 * @return The value of the element.
	 */
	inline T& operator()(std::vector<size_t> coordinates_) {
		return data_ptr[getIndex(coordinates_)];
	}

	/*!
	 * Operator returning the value of a given element of nD tensor.
	 * @param index_ Index (a single value).
	 * @return The value of the element.
	 */
	inline const T& operator()(size_t index_) const {
		return data_ptr[index_];
	}

	/*!
	 * Operator returning the value of a given element of nD tensor.
	 * @param coordinates_ nD vector of element coordinates ({ }, vector<size_t> etc.).
	 * @return The value of the element.
	 */
	inline const T& operator()(std::vector<size_t> coordinates_) const {
		return (T) data_ptr[getIndex(coordinates_)];
	}

	/*!
	 * Returns the index of an element in nD matrix.
	 * For dimensions higher than 3 it uses recursive function.
	 * @param coordinates_ nD vector of element coordinates ({ }, vector<size_t> etc.).
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
		switch (coordinates_.size()) {
		case 1:
			return coordinates_[0];
		case 2:
			return coordinates_[1] * dimensions[0] + coordinates_[0];
		case 3:
			return (coordinates_[2] * dimensions[1] + coordinates_[1]) * dimensions[0] + coordinates_[0];
		default:
			return recursiveIndex(0, coordinates_);
		}		//: switch
	}


	/*!
	 * Returns the (sub)tensor, a new tensor with data being a subtraction from the original tensor taking into account the ranges given as pairs (lower, higher).
	 * For dimensions higher than 3 it uses recursive block copy.
	 * @param lower_ Ranges given as pairs (lower, higher) e <0,size-1> for each dimension. If given range consists of a single value, then it is treated as (lower=higher).
	 * @return The created subtensor
	 */
	Tensor<T> block(std::vector< std::vector<size_t> > ranges_) {
		// All dimensions (tensor and lower and higher) must be equal!
		assert(dimensions.size() == ranges_.size());

		// Set dimensions.
		std::vector<size_t> new_dims;
		for (size_t i=0; i < ranges_.size(); i++) {
			// Every range must be given.
			assert((ranges_[i].size() >0) && (ranges_[i].size() < 3));
			// Add second range - just duble the first one.
			if (ranges_[i].size() == 1)
				ranges_[i].push_back(ranges_[i][0]);
			// Calculate the dimension.
			size_t ith_dimension = ranges_[i][1] - ranges_[i][0] +1;
			// Every dimension must be greater than 0!
			assert(ranges_[i][0] >= 0);
			assert(ranges_[i][1] < dimensions[i]);
			assert(ith_dimension > 0);
			// Add dimension.
			new_dims.push_back(ith_dimension);
		}//: for
		// Create tensor of a required size.
		mic::types::Tensor<T> new_tensor(new_dims);

		// Do the magic.
		// Get block by block and copy it in the right places.
		// But first: solve the simple 1d-2d-3d cases.
		switch (new_dims.size()) {
		case 1: {
			// Copy data from lower to higher.
			memcpy(new_tensor.data_ptr, (data_ptr + ranges_[0][0]), new_dims[0]* sizeof(T));
			break;
			}
		case 2: {
			// Iterate through blocks.
			for (size_t i=ranges_[1][0], j=0; i<=ranges_[1][1]; i++, j++) {
				// Copy data from lower to higher.
				memcpy(new_tensor.data_ptr + j* new_dims[0], (data_ptr + i * dimensions[0] + ranges_[0][0]), new_dims[0]* sizeof(T));
			}//: for
			break;
			}
		case 3: {
			// Iterate through blocks.
			for (size_t i2=ranges_[2][0], j2=0; i2<=ranges_[2][1]; i2++, j2++) {
				for (size_t i1=ranges_[1][0], j1=0; i1<=ranges_[1][1]; i1++, j1++) {
					// Copy data from lower to higher.
					memcpy(new_tensor.data_ptr + (j2* new_dims[1] + j1)* new_dims[0], (data_ptr + (i2 * dimensions[1] + i1) * dimensions[0] + ranges_[0][0]), new_dims[0]* sizeof(T));
					}//: for
				}//: for
			break;
			}
		default:
			// Vector of indices - empty for now, will be filled by recursive block copy.
			std::vector<size_t> is;
			std::vector<size_t> js;
			// Recursively copy block by block - starting from 1st dimension, as 0th is treated as the size of block.
			recursiveBlockCopy(1, ranges_, is, js, new_dims, new_tensor.data_ptr);
		}//: switch

		return new_tensor;
	}

	/*!
	 * Concatenates two tensors - attaches the tensor passed as argument "to the back".
	 * Note: both tensors must have exactly the same dimensions except the 0th dimension.
	 * @param obj_ Tensor to be attached.
	 */
	void concatenate(const Tensor& obj_) {
		// All dimensions (except 0th) must be equal!
		assert(dimensions.size() == obj_.dimensions.size());
		for (size_t d=1; d<dimensions.size(); d++) {
			assert(dimensions[d] == obj_.dimensions[d]);
		}//: for

		// Copy data.
		T* old_prt = data_ptr;
		// Allocate a new block of memory.
		data_ptr = new T[elements + obj_.elements];

		// Copy data.
		memcpy(data_ptr, old_prt, sizeof(T) * elements);
		memcpy(data_ptr, obj_.data_ptr, sizeof(T) * obj_.elements);

		// Free the old block.
		delete (old_prt);

		// Adjust the dimensions.
		dimensions[0] += obj_.dimensions[0];
		elements += obj_.elements;
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
		if (dim_ == coordinates_.size() - 1)
			return coordinates_[dim_];
		else
			return recursiveIndex(dim_ + 1, coordinates_) * dimensions[dim_] + coordinates_[dim_];
	}


	/*!
	 * A recursive function copying the blocks of data from source (current) tensor to the new one.
	 * 0th dimension (of the new tensor) is treated as the size of a block.
	 * The function calls itself (recursively) in order to generate a vector of indices used subsequently for calculation of positions in both source and target data tables.
	 * It uses two helper functions (also recursive) for computation of those positions.
	 * WARNING: Tensor dimensions must be > 1!
	 * @param dim_ The currently analyzed dimension of tensor.
	 * @param ranges_ nD vector of pairs of ranges.
	 * @param is_ nD vector of indices in the source tensor.
	 * @param js_ nD vector of indices in the new/target tensor.
	 * @param new_dims_ nD vector of dimensions of the new tensor.
	 * @param tgt_data_ptr_ Pointer to the memory block of the new tensor.
	 */
	void recursiveBlockCopy (size_t dim_, std::vector< std::vector<size_t> > ranges_, std::vector<size_t> is_, std::vector<size_t> js_, std::vector<size_t> new_dims_, T* tgt_data_ptr_) {
		// Check dimensions!
		assert(new_dims_.size()>1);
		//
		if (dim_ == new_dims_.size()){
			// Calculate destination index.
			size_t tgt_index = recursiveCalculateTargetIndex(0, js_, new_dims_);
			//std::cout << "recursive tgt_index = " << tgt_index << std::endl;
			// Calculate destination index.
			size_t src_index = recursiveCalculateSourceIndex(0, is_, ranges_[0][0]);
			//std::cout << "recursive src_index = " << src_index << std::endl;
			// Copy data from source to target.
			memcpy(tgt_data_ptr_ + tgt_index, (data_ptr + src_index), new_dims_[0]* sizeof(T));
			return;
		}
		// For all ranges collect indices list for source and targets.
		for (size_t i=ranges_[dim_][0], j=0; i<=ranges_[dim_][1]; i++, j++) {
			// Add indices to lists.
			is_.push_back(i);
			js_.push_back(j);
			// Better call Saul! Otherwise call block copy ;)
			recursiveBlockCopy(dim_ + 1, ranges_, is_, js_, new_dims_, tgt_data_ptr_);
			// Remove recently added indices from lists. ;)
			is_.pop_back();
			js_.pop_back();
		}
	}

	/*!
	 * A recursive function responsible for the calculation of position (index) in target (i.e. new) data table.
	 * @param dim_ The currently analyzed dimension of tensor.
	 * @param js_ nD vector of indices in the new/target tensor.
	 * @param dims_ nD vector of dimensions of the (new) tensor.
	 * @return Index in target (new) data table.
	 */
	size_t recursiveCalculateTargetIndex(size_t dim_, std::vector<size_t> js_, std::vector<size_t> dims_) {
		if (dim_ == js_.size()) {
			//std::cout << " returning js_["<< dim_-1 <<"] = " << js_[dim_-1] << std::endl;
			return js_[dim_-1];
		} else if (dim_ == 0) {
			//std::cout << "calling recursiveCalculateTargetIndex(dim_+1 ("<< dim_ + 1 << "), js_, dims_)"<< std::endl;
			size_t tmp = recursiveCalculateTargetIndex(dim_ + 1, js_, dims_);
			size_t tmp2 = tmp * dims_[dim_];
			//std::cout <<  " returning tmp2 ("<< tmp2 <<") = tmp (" << tmp << ") * dims_[dim_ ("<< dim_<< ")] ("<< dims_[dim_] << ") "<< std::endl;
			return tmp2;
		} else {

			//std::cout << "calling recursiveCalculateTargetIndex(dim_+1 ("<< dim_ + 1 << "), js_, dims_)"<< std::endl;
			size_t tmp = recursiveCalculateTargetIndex(dim_ + 1, js_, dims_);
			size_t tmp2 = tmp * dims_[dim_] + js_[dim_-1];
			//std::cout <<  " returning tmp2 ("<< tmp2 <<") = tmp (" << tmp << ") * dims_[dim_ ("<< dim_<< ")] ("<< dims_[dim_] << ") + js_[dim_-1] ("<< js_[dim_-1]  << ") "<< std::endl;
			return tmp2;
		}//: else
	}

	/*!
	 * A recursive function responsible for the calculation of position (index) in source (i.e. this) data table.
	 * @param dim_ The currently analyzed dimension of tensor.
	 * @param is_ nD vector of indices in the source tensor.
	 * @param offset_ Offset added aat the end(always const = range[0][0]).
	 * @return Index in source (this) data table.
	 */
	size_t recursiveCalculateSourceIndex(size_t dim_, std::vector<size_t> is_, size_t offset_) {
		if (dim_ == is_.size()) {
			//std::cout << " returning is_["<< dim_-1 <<"] = " << is_[dim_-1] << std::endl;
			return is_[dim_-1];
		} else if (dim_ == 0) {
			//std::cout << "calling recursiveCalculateSourceIndex(dim_+1 ("<< dim_ + 1 << "), is_, dimensions)"<< std::endl;
			size_t tmp = recursiveCalculateSourceIndex(dim_ + 1, is_, offset_);
			size_t tmp2 = tmp * dimensions[dim_] + offset_;
			//std::cout <<  " returning tmp2 ("<< tmp2 <<") = tmp (" << tmp << ") * dimensions[dim_ ("<< dim_<< ")] ("<< dimensions[dim_] << ") + offset (" << offset_ <<")"<< std::endl;
			return tmp2;
		} else {
			//std::cout << "calling recursiveCalculateSourceIndex(dim_+1 ("<< dim_ + 1 << "), is_, dims_)"<< std::endl;
			size_t tmp = recursiveCalculateSourceIndex(dim_ + 1, is_, offset_);
			size_t tmp2 = tmp * dimensions[dim_] + is_[dim_-1];
			//std::cout <<  " returning tmp2 ("<< tmp2 <<") = tmp (" << tmp << ") * dimensions[dim_ ("<< dim_<< ")] ("<< dimensions[dim_] << ") + is_[dim_-1] ("<< is_[dim_-1]  << ") "<< std::endl;
			return tmp2;
		}//: else
	}


};

} //: namespace types
} //: namespace mic

#endif /* SRC_TYPES_TENSOR_HPP_ */
