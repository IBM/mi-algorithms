/*!
 * \file DataLabelPair.hpp
 * \brief 
 * \author tkornut
 * \date Mar 30, 2016
 */

#ifndef SRC_TYPES_PAIR_HPP_
#define SRC_TYPES_PAIR_HPP_

namespace mic {
namespace types {

template<typename DATA, typename LABEL>
struct Pair {
public:
	/*!
	 * Constructor.
	 * @param data_
	 * @param label_
	 */
	Pair(DATA data_, LABEL label_) : data(data_), label(label_) { }

	virtual ~Pair() { };

	/// Stores pair data.
	DATA data;

	/// Stores pair label.
	LABEL label;
};

template<typename DATA, typename LABEL>
mic::types::Pair<DATA, LABEL> makePair(DATA data_, LABEL label_) {
	return Pair<DATA, LABEL>(data_, label_);
}


} /* namespace types */
} /* namespace mic */

#endif /* SRC_TYPES_PAIR_HPP_ */
