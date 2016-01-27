/*
 * \file get_average.hpp
 * \brief 
 * \author: tkornut
 * \date Nov 20, 2015
 */

#ifndef SRC_DATA_UTILS_GET_AVERAGE_HPP_
#define SRC_DATA_UTILS_GET_AVERAGE_HPP_

#include <vector>
#include <numeric>

namespace mic {
namespace data_utils {

/*!
 * @brief Computes average value of a given data taking into account the last number of elements.
 * @author krocki/tkornuta
 * @param data Data vector.
 * @param period Number of previous data used.
 * @tparam Type Template parameter denoting datatype.
 * @return Average.
 */
template<typename Type> Type get_average(std::vector<Type> data, unsigned period) {
	// Truncate size.
    if (period > data.size())
        period = (unsigned)data.size();

    // Return 0 if there are no data.
    if (period < 1)
        return (Type)0.0f;

    // Return average otherwise.
    return (Type)(std::accumulate(data.end() - period, data.end(), (Type)0.0f) / (Type)period);
}



}//: namespace data_utils
}//: namespace mic

#endif /* SRC_DATA_UTILS_GET_AVERAGE_HPP_ */
