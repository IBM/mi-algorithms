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



template <class T>
class TensorView {
public:
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

	 mic::types::Matrix<T> eval() {
		 mic::types::Matrix<T> ret_m(11,11);
		 return ret_m;
	 }

private:
	/*!
	 * Pointer to the tensor structure (memory).
	 */
	mic::types::Tensor<T> * tensor;

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
