/*!
 * \file DataLabelPair.hpp
 * \brief 
 * \author tkornut
 * \date Mar 30, 2016
 */

#ifndef SRC_TYPES_SAMPLE_HPP_
#define SRC_TYPES_SAMPLE_HPP_

namespace mic {
namespace types {

/*!
 * \brief Structure for storing the data-label pairs. Additionally stores the number of sample.
 * \author tkornuta
 */
template<typename DATA, typename LABEL>
struct Sample {
public:
	/*!
	 * Constructor.
	 * @param data_
	 * @param label_
	 */
	Sample(DATA data_, LABEL label_, size_t number_= -1) : data(data_), label(label_), sample_number(number_) { }

	virtual ~Sample() { };

	/// Stores the data.
	DATA data;

	/// Stores the label.
	LABEL label;

	/// The sample number (thr position in original dataset).
	size_t sample_number;
};

/*!
 * A function returning the DataLabelPair for given sample (analog to std::make_pair).
 * @param data_
 * @param label_
 * @param number_
 * @return
 */
template<typename DATA, typename LABEL>
mic::types::Sample<DATA, LABEL> makeSample(DATA data_, LABEL label_, size_t number_ = -1) {
	return Sample<DATA, LABEL>(data_, label_, number_);
}


} /* namespace types */
} /* namespace mic */

#endif /* SRC_TYPES_SAMPLE_HPP_ */
