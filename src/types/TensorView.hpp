/*!
 * \file TensorView.hpp
 * \brief 
 * \author tkornut
 * \date Mar 7, 2016
 */

#ifndef SRC_TYPES_TENSORVIEW_HPP_
#define SRC_TYPES_TENSORVIEW_HPP_

#include <iostream>
#include <iomanip>

#include <limits>

#include <types/tensor.h>

#include <types/MatrixTypes.hpp>

namespace mic {
namespace types {

/*!
 * \brief Class representing ranges as triplets similar to MATLAB min:step:max.
 * \author tkornuta
 */
class ViewRange {
private:
	/// Minimum range limit (equal toor grater from 0).
	size_t min;
	/// Maximum range limit.
	size_t max;
	/// Size of step.
	size_t step;

public:

	/*!
	 * Constructs a range from (0:INF) (equivalent of : in MATLAB).
	 */
	ViewRange() {
		min = 0;
		max = std::numeric_limits<size_t>::max();
		step = 1;
	}

	/*!
	 * Constructs a range consisting of a single element.
	 * @param value_ The value.
	 */
	ViewRange(size_t value_) {
		min = max = value_;
		step = 1;
	}

	/*!
	 * Constructs a range (min-max) with step equal to 1. Truncates min to 0.
	 * @param min_ Min range value.
	 * @param max_ Max range value
	 */
	ViewRange(size_t min_, size_t max_) {
		if (min_ <= max_) {
			min = min_;
			max = max_;
		} else {
			min = max_;
			max = min_;
		}
		// Truncate min to zero.
		min = (min > 0) ? min : 0;
		step = 1;
	}

	/*!
	 * Constructs a range (min-max) with step equal toa given value. Truncates min to 0.
	 * @param min_ Min range value.
	 * @param step_ Step.
	 * @param max_ Max range value
	 */
	ViewRange(int min_, int step_, int max_) {
		if (min <= max) {
			min = min_;
			max = max_;
		} else {
			min = max_;
			max = min_;
		}
		// Truncate min to zero.
		min = (min > 0) ? min : 0;
		step = step_;
	}

	/*!
	 * Copying constructor.
	 * @param range_ Range object to be copied.
	 */
	ViewRange(const ViewRange& range_) {
		min = range_.min;
		max = range_.max;
		step = range_.step;
	}

	/// Forward declaration of a friend class TensorView.
	template<typename T>
	friend class TensorView;

};



/*!
 * \brief Class defining view - a subset of data of an existing tensor.
 * TensorView does not stores the real data. It stores the ranges and offers methods for returning subsets as tensors (nD), matrices (2D), vectors (1D) and single values (scalars).
 *
 * \tparam T Template parameter denoting elementary type of data used in the associated tensor (int, float, double etc.).
 * \date Mar 7, 2016
 * \author tkornuta
 */
template <class T>
class TensorView {
public:
	/*!
	 * Constructor.
	 * @param tensor_ The tensor associated with a given view.
	 * @param ranges_ A list of ranges - must have the same dimensions as tensor.
	 */
	TensorView(mic::types::Tensor<T> & tensor_, std::initializer_list<ViewRange> ranges_) {
		//static_assert((ranges_.size() == tensor_.dim.size()), "Wrong number of dimensions");
		assert(ranges_.size() == tensor_.dim.size());

		// Set pointer.
		tensor = &tensor_;

		// Set ranges.
	    size_t r = 0;
		for (ViewRange vr: ranges_) {
			//std::cout << vr.min << ":" << vr.step << ":" << vr.max << std::endl;
			std::vector<size_t> rrange;
			for(size_t i = vr.min; (i <= vr.max) && (i < tensor->dim[r]); i+=vr.step) {
				rrange.push_back(i);
//				std::cout << i << "\n";
			}//: for
			// Push ranges for given dimension to container.
			ranges.push_back(rrange);
			// Move to next range.
			r++;
		}//: for
	}

	/*!
	 * Copying constructor. Defines a subview - view on the same tensor but with yet another ranges associated.
	 * @param view_ Previous view.
	 * @param ranges_ A list of ranges - must have the same dimensions as tensor view (thus the original tensor as well).
	 */
	TensorView(mic::types::TensorView<T> & view_, std::initializer_list<ViewRange> ranges_) {
		//static_assert((ranges_.size() == tensor_.dim.size()), "Wrong number of dimensions");
		assert(ranges_.size() == view_.ranges.size());

		// Set pointer.
		tensor = view_.tensor;

		// Set ranges.
	    size_t r = 0;
		for (ViewRange vr: ranges_) {
			//std::cout << vr.min << ":" << vr.step << ":" << vr.max << std::endl;
			std::vector<size_t> rrange;
			for(size_t i = vr.min; (i <= vr.max) && (i < view_.ranges[r].size()); i+=vr.step) {
				rrange.push_back(view_.ranges[r][i]);
//				std::cout << i << "\n";
			}//: for
			// Push ranges for given dimension to container.
			ranges.push_back(rrange);
			// Move to next range.
			r++;
		}//: for
	}


	/*!
	 * Stream operator enabling to print tensor view ranges for all dimensions.
	 * @param os
	 * @param tv
	 * @return
	 */
	friend std::ostream& operator<<(std::ostream& os, const TensorView<T>& tv) {
		// For all ranges/dimensions.
		for (size_t i=0; i < tv.ranges.size(); i++) {
			// For all values in a given dimension.
			os << "[";
			for (size_t v=0; v < tv.ranges[i].size(); v++) {
				os << tv.ranges[i][v] << ",";
			}//: for
			os << "]\n";
		}//: for

		return os;
	}

	 /*!
	  * Returns a new (sub)tensor taking into account the view ranges.
	  * @return (Sub)tensor.
	  */
	 mic::types::Tensor<T> getTensor() {
		 // Compute dimensions.

		// Do the magic.
		//...

		 mic::types::Tensor<T> t;
		 return t;
	 }


	 /*!
	  * Returns a (sub)tensor in the form of 2D matrix taking into account the view ranges.
	  * @return Matrix being a 2D (sub)tensor.
	  */
	 mic::types::Matrix<T> getMatrix() {
		 // Check how many dimensions are active.
		 int act_dims = 0;
		for (size_t i=0; i < ranges.size(); i++) {
			if (ranges[i].size() > 1)
				act_dims++;
		}//: for
		assert(act_dims==2);// "getMatrix(): number of Tensor View ranges having more than one element must be equal to 2"
		// Assign memory to returned object.
		mic::types::Matrix<T> ret_m(11,11);
		// Do the magic - depending on the tensor dimensions.
		//...

		// Return matrix.
		return ret_m;
	 }


	 /*!
	  * Returns a (sub)tensor in the form of 1D vector taking into account the view ranges.
	  * @return Vector being a 1D (sub)tensor.
	  */
	 mic::types::VectorX<T> getVector() {
		// Check how many dimensions are active.
		int act_dims = 0;
		for (size_t i=0; i < ranges.size(); i++) {
			if (ranges[i].size() > 1)
				act_dims++;
		}//: for
		assert(act_dims==1);// "getVector(): number of Tensor View ranges having more than one element must be equal to 1"
		// Assign memory to returned object.
		mic::types::VectorX<T> ret_v(11);
		// Do the magic.
		//...

		// Return matrix.
		return ret_v;
	 }


	 /*!
	  * Returns a (sub)tensor in the form of a scalar taking into account the view ranges.
	  * @return A single value of a given (template) type.
	  */
	 T getScalar() {
		 // Check how many dimensions are active.
		 int act_dims = 0;
		for (size_t i=0; i < ranges.size(); i++) {
			if (ranges[i].size() > 1)
				act_dims++;
		}//: for
		assert(act_dims==0); // "getScalar(): all Tensor View ranges must have only one element"
		// Assign memory to returned object.
		T ret_v = 0;

		// Do the magic - iterate through all dimensions in order to compute the index.
		size_t index=0;
		//...


		// Return matrix.
		return ret_v;
	 }

private:
	/*!
	 * Pointer to the tensor structure (memory).
	 */
	mic::types::Tensor<T> * tensor;

	/*!
	 *
	 */
	std::vector<std::vector<size_t> > ranges;
};

/*!
 * \brief Define for short View Range definitions.
 * \author tkornuta
 */
#define VR mic::types::ViewRange


}//: namespace types
}//: namespace mic


#endif /* SRC_TYPES_TENSORVIEW_HPP_ */
